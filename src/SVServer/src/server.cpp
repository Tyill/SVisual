//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "server.h"
#include "buffer_data.h"
#include "thread_update.h"
#include "SVMisc/misc.h"

#include <cstring>

using namespace std;

SV_Srv::statusCBack pfStatusCBack = nullptr;
SV_Srv::onUpdateSignalsCBack pfUpdateSignalsCBack = nullptr;
SV_Srv::onAddSignalsCBack pfAddSignalsCBack = nullptr;
SV_Srv::onModuleConnectCBack pfModuleConnectCBack = nullptr;
SV_Srv::onModuleDisconnectCBack pfModuleDisconnectCBack = nullptr;

const int BUFF_SIGN_HOUR_CNT = 2;  // жестко размер буфера, час

namespace{
    const char* beginMess = "=begin=";
    const char* endMess = "=end=";
}

namespace SV_Srv {

  Config cng;

  BufferData _buffData;
  ThreadUpdate* _pThrUpdSignal = nullptr;

  std::map <std::string, SV_Base::ModuleData*> _moduleData;
  std::map <std::string, SV_Base::SignalData*> _signalData;

  std::mutex _mtx;

    
  void setStatusCBack(statusCBack cback){

    pfStatusCBack = cback;
  }

  bool startServer(const Config& _cng){

    std::lock_guard<std::mutex> lck(_mtx);

    if (_pThrUpdSignal) return true;

    cng = _cng;

    _buffData.init(cng);

    _pThrUpdSignal = new ThreadUpdate(cng, _buffData);

    return true;
  }

  void stopServer(){

    if (_pThrUpdSignal)
      delete _pThrUpdSignal;
  }
    
  void setConfig(const Config& cng){
        
    if (_pThrUpdSignal)
      _pThrUpdSignal->setArchiveConfig(cng);    
  }

  void receiveData(std::string& inout, std::string& out){
    
    vector<pair<size_t, size_t>> bePos;
    size_t stPos = inout.find(beginMess), endPos = inout.find(endMess);
    while ((stPos != std::string::npos) && (endPos != std::string::npos)){

      int allSz = *(int*)(inout.c_str() + stPos + strlen(beginMess));

      if (allSz == (endPos - stPos - 11)) // 11 - strlen(beginMess) + sizeof(int32)
        bePos.push_back(pair<size_t, size_t>(stPos + 11, endPos));

      stPos = inout.find(beginMess, endPos + strlen(endMess));
      if (stPos != std::string::npos){
        endPos = inout.find(endMess, stPos + 11);
      }
    };

    uint64_t bTm = SV_Misc::currDateTimeSinceEpochMs();
    auto psz = bePos.size();
    for (size_t i = 0; i < psz; ++i){

      stPos = bePos[i].first;
      endPos = bePos[i].second;

      _buffData.updateDataSignals(string(inout.data() + stPos, inout.data() + endPos),
                                  bTm - (psz - i) * SV_CYCLESAVE_MS);
    }
       
    if (psz > 0){
      inout = std::string(inout.data() + endPos + strlen(endMess));
    }
  }

  void setOnUpdateSignalsCBack(onUpdateSignalsCBack cback){

    pfUpdateSignalsCBack = cback;
  }

  void setOnAddSignalsCBack(onAddSignalsCBack cback){

    pfAddSignalsCBack = cback;
  }

  void setOnModuleConnectCBack(onModuleConnectCBack cback){

    pfModuleConnectCBack = cback;
  }

  void setOnModuleDisconnectCBack(onModuleDisconnectCBack cback){

    pfModuleDisconnectCBack = cback;
  }

  std::map<std::string, SV_Base::ModuleData*> getCopyModuleRef(){

    std::lock_guard<std::mutex> lck(_mtx);

    map<string, SV_Base::ModuleData*> mref = _moduleData;

    return mref;
  };

  SV_Base::ModuleData* getModuleData(const std::string& module){

    std::lock_guard<std::mutex> lck(_mtx);

    return _moduleData.find(module) != _moduleData.end() ? _moduleData[module] : nullptr;
  }
   
  std::vector<std::string> getModuleSignals(const std::string& module){

    std::lock_guard<std::mutex> lck(_mtx);

    return _moduleData.find(module) != _moduleData.end() ? _moduleData[module]->signls : std::vector<std::string>();
  }

  std::map<std::string, SV_Base::SignalData*> getCopySignalRef(){

    std::lock_guard<std::mutex> lck(_mtx);

    map<string, SV_Base::SignalData*> sref = _signalData;

    return sref;
  };

  SV_Base::SignalData* getSignalData(const std::string& sign){

    std::lock_guard<std::mutex> lck(_mtx);

    return _signalData.find(sign) != _signalData.end() ? _signalData[sign] : nullptr;
  }

  bool addSignal(SV_Base::SignalData* sd){
    
    if (!sd) return false;
    
    std::lock_guard<std::mutex> lck(_mtx);
        
    bool ok = false;
    string sign = sd->name + sd->module;
    if (_signalData.find(sign) == _signalData.end()) {
      _signalData[sign] = sd;
      if (!_moduleData.count(sd->module)){
          _moduleData[sd->module] = new SV_Base::ModuleData(sd->module);
          _moduleData[sd->module]->isEnable = true;
      }
      _moduleData[sd->module]->signls.push_back(sign);
      ok = true;
    }
    return ok;
  }

  bool addModule(SV_Base::ModuleData* md){

    if (!md) return false;

    std::lock_guard<std::mutex> lck(_mtx);

    bool ok = false;
    if (md && (_moduleData.find(md->module) == _moduleData.end())) {
      _moduleData[md->module] = md;
      ok = true;
    }
    return ok;
  }

  bool signalBufferEna(const std::string& sign){

    std::lock_guard<std::mutex> lck(_mtx);

    if (_signalData.find(sign) == _signalData.end()) return false;

    if (!_signalData[sign]->isBuffEnable){

      int buffSz = BUFF_SIGN_HOUR_CNT * 3600000 / SV_CYCLESAVE_MS;

      _signalData[sign]->buffData.resize(buffSz);

      SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * buffSz];
      for (int i = 0; i < buffSz; ++i)
        _signalData[sign]->buffData[i].vals = &buff[i * SV_PACKETSZ];

      _signalData[sign]->isBuffEnable = true;
    }
    return true;
  }
}
