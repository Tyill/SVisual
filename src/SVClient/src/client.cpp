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

#include "SVClient/SVClient.h"
#include "SVBase/limits.h"
#include "SVBase/base.h"
#include "SVMisc/tcp_client.h"
#include "SVMisc/misc.h"

#include <string>
#include <mutex>
#include <thread>
#include <map>
#include <cstring>
#include <atomic>

struct Config {

  int cycleRecMs;
  int packetSz;

  Config(int cycleRecMs_ = 100, int packetSz_ = 10) :
    cycleRecMs(cycleRecMs_), packetSz(packetSz_){}
};

struct ValueRec {
  bool isActive;
  bool isOnlyFront;
  char name[SV_NAMESZ];
  SV_Base::ValueType type;
  SV_Base::Value* vals;
};

std::atomic_bool _isConnect = false,
                 _thrStop = false;

std::thread _thr;
std::mutex _mtx;

std::map<std::string, ValueRec> _values;

std::string _module, _addrServ;
int _portServ = 0;
int _curCycle = 0;

Config cng;

namespace{
  const char* messBegin = "=begin=";
  const char* messEnd = "=end=";
}

namespace SV {

    void sendCycle();

    bool svConnect(const char *moduleName, const char *ipAddr, int port) {
           
      if (_isConnect) return true;

      if ((strlen(moduleName) == 0) || (strlen(moduleName) >= SV_NAMESZ) ||
           strstr(moduleName, messEnd) || strstr(moduleName, messBegin)) {
        return false;
      }

      _module = moduleName;
      _addrServ = ipAddr;
      _portServ = port;

      _isConnect = SV_Misc::TCPClient::connect(ipAddr, port);

      if (_isConnect) {
        _thr = std::thread(sendCycle);
      }

      return _isConnect;
    }

    void svDisconnect() {
          
      _thrStop = true;
      if (_thr.joinable()) _thr.join();

      if (_isConnect){
        SV_Misc::TCPClient::disconnect();
        _isConnect = false;
      }
    }

    bool addValue(const char* name, SV_Base::ValueType type, SV_Base::Value val, bool onlyPosFront);

    bool svAddBoolValue(const char *name, bool value, bool onlyPosFront) {

        SV_Base::Value val;
        val.vBool = value;
        return addValue(name, SV_Base::ValueType::BOOL, val, onlyPosFront);
    }

    bool svAddIntValue(const char *name, int value) {

        SV_Base::Value val;
        val.vInt = value;
        return addValue(name, SV_Base::ValueType::INT, val, false);
    }

    bool svAddFloatValue(const char *name, float value_in) {

        SV_Base::Value val;
        val.vFloat = value_in;
        return addValue(name, SV_Base::ValueType::FLOAT, val, false);
    }

    bool svSetParam(int cycleRecMs, int packetSz) {
            
        std::lock_guard<std::mutex> lck(_mtx);

        cng = Config(cycleRecMs, packetSz);

        return true;
    }
           
    bool addValue(const char* name, SV_Base::ValueType type, SV_Base::Value val, bool onlyPosFront) {
     
      if (_values.find(name) == _values.end()) {
        if ((strlen(name) == 0) || (strlen(name) >= SV_NAMESZ) || 
             strstr(name, "=end=") || strstr(name, "=begin=")){
          return false;
        }
        ValueRec vr;
        vr.vals = new SV_Base::Value[SV_PACKETSZ];
        memset(vr.vals, 0, sizeof(SV_Base::Value) * SV_PACKETSZ);
       
        strcpy(vr.name, name);
        vr.type = type;
        vr.isOnlyFront = onlyPosFront;
        vr.isActive = false;
        {  std::lock_guard<std::mutex> lck(_mtx);
            _values.insert({ name, vr });
        }
      }
      {  std::lock_guard<std::mutex> lck(_mtx);
          ValueRec& vr = _values[name];
          vr.vals[_curCycle] = val;
          vr.isActive = true;
      }
      return true;
    }

    std::string prepareData() {

      if (_values.empty()) return "";

      size_t SINT = sizeof(int32_t),
             /*      val name    type      vals          */
             valSz = SV_NAMESZ + SINT + SINT * SV_PACKETSZ,
             /*       mod name            vals           */
             dataSz = SV_NAMESZ + valSz * _values.size(),
             
             startSz = sizeof(messBegin), endSz = sizeof(messEnd), offs = 0, 
             /*                dataSz                    */
             messSz = startSz + SINT + dataSz + endSz;

      std::string data(messSz, '\0');

      char* dptr = (char*)data.c_str();
      memcpy(dptr, messBegin, startSz);               offs += startSz;
      memcpy(dptr + offs, &dataSz, SINT);             offs += SINT;
      memcpy(dptr + offs, _module.data(), SV_NAMESZ); offs += SV_NAMESZ;

      for (const auto& v : _values) {
        memcpy(dptr + offs, v.second.name, SV_NAMESZ);
        memcpy(dptr + offs + SV_NAMESZ, &v.second.type, SINT);
        memcpy(dptr + offs + SV_NAMESZ + SINT, v.second.vals, SV_PACKETSZ * SINT);
        offs += valSz;
      }

      memcpy(dptr + offs, messEnd, endSz);

      return data;
    }

    bool sendData(const std::string& data){
      std::string out;
      return SV_Misc::TCPClient::sendData(data, out, false, true);
    }

    void sendCycle() {

      uint64_t cTm = SV_Misc::currDateTimeSinceEpochMs(),
               prevTm = cTm;

      int tmDiff = SV_CYCLEREC_MS,
          cDelay = 0;

      while (!_thrStop) {

        if (!_isConnect){
          _isConnect = SV_Misc::TCPClient::connect(_addrServ, _portServ);
        }

        cTm = SV_Misc::currDateTimeSinceEpochMs();
        tmDiff = int(cTm - prevTm) - cDelay;
        prevTm = cTm;
               
        std::string data;
        {  std::lock_guard<std::mutex> lck(_mtx);
           
            int prevCyc = _curCycle - 1;
            if (prevCyc < 0)
                prevCyc = SV_PACKETSZ - 1;

            for (auto it = _values.begin(); it != _values.end(); ++it) {
                if (!it->second.isActive) {
                    it->second.vals[_curCycle] = it->second.vals[prevCyc];

                    if ((it->second.type == SV_Base::ValueType::BOOL) && it->second.isOnlyFront)
                        it->second.vals[_curCycle].vBool = false;
                }
                it->second.isActive = false;
            }

            if (_curCycle < SV_PACKETSZ - 1) {
                ++_curCycle;
            }else {
              _curCycle = 0;
              if (_isConnect && !_values.empty()){
                data = prepareData();
              }                  
            }
        }
        if (_isConnect && !data.empty()){
          _isConnect = sendData(data);
        }
          
        cDelay = (SV_CYCLEREC_MS - tmDiff) > 0 ? (SV_CYCLEREC_MS - tmDiff) : 0;
        if (cDelay > 0) {            
            SV_Misc::sleepMs(cDelay);
        }
      }
    }
}