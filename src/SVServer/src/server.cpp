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

#include "buffer_data.h"
#include "thread_update.h"
#include "SVAuxFunc/aux_func.h"

#include "Lib/rapidjson/document.h"
#include "Lib/rapidjson/stringbuffer.h"
#include "Lib/rapidjson/writer.h"


using namespace std;

SV_Srv::statusCBack pfStatusCBack = nullptr;
SV_Srv::onUpdateSignalsCBack pfUpdateSignalsCBack = nullptr;
SV_Srv::onAddSignalsCBack pfAddSignalsCBack = nullptr;
SV_Srv::onModuleConnectCBack pfModuleConnectCBack = nullptr;
SV_Srv::onModuleDisconnectCBack pfModuleDisconnectCBack = nullptr;

const int BUFF_SIGN_HOUR_CNT = 2;  // жестко размер буфера, час

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

  void jsonRequestData(std::string &inout, std::string &out);

  void receiveData(std::string& inout, std::string& out){
    
    vector<pair<size_t, size_t>> bePos;
    size_t stPos = inout.find("=begin="), endPos = inout.find("=end=");
    while ((stPos != std::string::npos) && (endPos != std::string::npos)){

      int allSz = *(int*)(inout.c_str() + stPos + 7); // 7 - sizeof("=begin=")

      if (allSz == (endPos - stPos - 11)) // 11 - sizeof("=begin=") + sizeof(int32)
        bePos.push_back(pair<size_t, size_t>(stPos + 11, endPos));

      stPos = inout.find("=begin=", endPos + 5); // 5 - sizeof("=end=")
      if (stPos != std::string::npos){
        endPos = inout.find("=end=", stPos + 11);
      }
    };

    uint64_t bTm = SV_Aux::currDateTimeSinceEpochMs();
    auto psz = bePos.size();
    for (size_t i = 0; i < psz; ++i){

      stPos = bePos[i].first;
      endPos = bePos[i].second;

      _buffData.updateDataSignals(string(inout.data() + stPos, inout.data() + endPos),
          bTm - (psz - i) * SV_CYCLESAVE_MS);
    }
       
    if (psz > 0)  
      inout = std::string(inout.data() + endPos + 5);
    else
      jsonRequestData(inout, out);
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
  

  //////////////////////////////////////////////////////////

  std::string jsonGetError(){

    using namespace rapidjson;

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("Command");
    writer.String("Error");

    writer.EndObject();

    return sb.GetString();
  }

  bool jsonCheckRequest(rapidjson::Document& doc){

    if (!doc.IsObject()) return false;

    if (!doc.HasMember("Command") || !doc["Command"].IsString()) return false;

    string cmd = doc["Command"].GetString();

    if (cmd == "getAllSignals"){
      return true;
    }
    else if (cmd == "getSignalData"){
      if (!doc.HasMember("Signal") || !doc["Signal"].IsString()) return false;
      if (!doc.HasMember("Module") || !doc["Module"].IsString()) return false;
    }
    else return false;

    return true;
  }

  std::string jsonGetAllSignals(){

    using namespace rapidjson;

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    writer.StartObject();

    writer.Key("Signals");
    writer.StartArray();

    auto sref = getCopySignalRef();
    for (auto& sign : sref){

      writer.StartObject();

      writer.Key("Name");
      writer.String(sign.second->name.c_str());
      writer.Key("Module");
      writer.String(sign.second->module.c_str());
      writer.Key("Group");
      writer.String(sign.second->group.c_str());
      writer.Key("Comment");
      writer.String(sign.second->comment.c_str());
      writer.Key("Type");
      writer.String(SV_Base::getSVTypeStr(sign.second->type).c_str());

      string state = "isActive";
      if (!sign.second->isActive) state = "noActive";
      writer.Key("State");
      writer.String(state.c_str());

      writer.EndObject();
    }
    writer.EndArray();

    writer.EndObject();

    return sb.GetString();
  }

  std::string jsonGetSignalData(std::string sign, std::string mod){

    using namespace rapidjson;

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    writer.StartObject();
    writer.Key("Command");
    writer.String("SignalData");

    auto sref = getCopySignalRef();

    string out;

    sign += mod;
    if (sref.find(sign) != sref.end()){

      auto sd = getSignalData(sign);

      writer.Key("Signal");
      writer.String(sd->name.c_str());
      writer.Key("Module");
      writer.String(sd->module.c_str());
      writer.Key("ValueTime");
      writer.String(to_string(sd->lastData.beginTime).c_str());
      writer.Key("Value");

      switch (sd->type) {
      case SV_Base::ValueType::BOOL:
        writer.String(to_string(sd->lastData.vals[0].vBool).c_str());
        break;
      case SV_Base::ValueType::INT:
        writer.String(to_string(sd->lastData.vals[0].vInt).c_str());
        break;
      case SV_Base::ValueType::FLOAT:
        writer.String(to_string(sd->lastData.vals[0].vFloat).c_str());
        break;
      }

      writer.EndObject();

      out = sb.GetString();
    }
    else{

      writer.Key("Signal");
      writer.String("");
      writer.Key("Module");
      writer.String("");
      writer.Key("ValueTime");
      writer.String("");
      writer.Key("Value");
      writer.String("");

      writer.EndObject();

      out = sb.GetString();
    }

    return out;
  }

  void jsonRequestData(std::string &inout, std::string &out){

    using namespace rapidjson;

    if (inout.empty()) return;

    Document document;
    document.Parse(inout.c_str());

    if (!jsonCheckRequest(document)){

      if (document.IsObject()) {
        out = jsonGetError();
        inout.clear();
      }
      return;
    }

    string cmd = document["Command"].GetString();

    if (cmd == "getAllSignals")
      out = jsonGetAllSignals();
    else if (cmd == "getSignalData")
      out = jsonGetSignalData(document["Signal"].GetString(), document["Module"].GetString());

    inout.clear();
  }
}