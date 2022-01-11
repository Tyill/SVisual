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
#include "SVConfig/config_limits.h"
#include "SVConfig/config_data.h"
#include "SVAuxFunc/tcp_client.h"
#include "SVAuxFunc/aux_func.h"

#include <string>
#include <mutex>
#include <thread>
#include <map>
#include <cstring>

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

volatile bool _isConnect = false,
              _thrStop = false;

std::thread _thr;
std::mutex _mtxUpdValue;

std::map<std::string, ValueRec> _values;

std::string _module, _addrServ;
int _portServ = 0;
int _curCycle = 0;

Config cng;

namespace SV {

    void sendCycle();

    bool svConnect(const char *moduleName, const char *ipAddr, int port) {
           
      if (_isConnect) return true;

      if ((strlen(moduleName) == 0) || (strlen(moduleName) >= SV_NAMESZ) ||
           strstr(moduleName, "=end=") || strstr(moduleName, "=begin=")) {
        return false;
      }

      _module = moduleName;
      _addrServ = ipAddr;
      _portServ = port;

      _isConnect = SV_Aux::TCPClient::connect(ipAddr, port);

      if (_isConnect) {
        _thr = std::thread(sendCycle);
      }

      return _isConnect;
    }

    void svDisconnect() {
          
      if (_isConnect)
        SV_Aux::TCPClient::disconnect();

      _thrStop = true;
      if (_thr.joinable()) _thr.join();
    }

    bool addValue(const char* name, SV_Base::ValueType type, SV_Base::Value val, bool onlyPosFront);

    bool svAddBoolValue(const char *name, bool value, bool onlyPosFront) {

        SV_Base::Value val;
        val.vBool = value;
        return addValue(name, SV_Base::ValueType::BOOL, val, false);
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
            
        std::lock_guard<std::mutex> lck(_mtxUpdValue);

        cng = Config(cycleRecMs, packetSz);

        return true;
    }
           
    bool addValue(const char* name, SV_Base::ValueType type, SV_Base::Value val, bool onlyPosFront) {

      if (!_isConnect) return false;

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

        std::lock_guard<std::mutex> lck(_mtxUpdValue);
        _values.insert({ name, vr });
      }

      ValueRec& vr = _values[name];
           
      std::lock_guard<std::mutex> lck(_mtxUpdValue);
      vr.vals[_curCycle] = val;
      vr.isActive = true;
     
      return true;
    }

    bool sendData() {

      if (_values.empty()) return true;

      size_t SINT = sizeof(int32_t),
             /*      val name    type      vals          */
             valSz = SV_NAMESZ + SINT + SINT * SV_PACKETSZ,
             /*       mod name            vals           */
             dataSz = SV_NAMESZ + valSz * _values.size(),
             
             startSz = 7, endSz = 5, offs = 0, 
             /*                dataSz                    */
             messSz = startSz + SINT + dataSz + endSz;

      std::string data(messSz, '\0');

      memcpy((char*)data.c_str(), "=begin=", startSz);               offs += startSz;
      memcpy((char*)data.c_str() + offs, &dataSz, SINT);             offs += SINT;
      memcpy((char*)data.c_str() + offs, _module.data(), SV_NAMESZ); offs += SV_NAMESZ;

      for (const auto& v : _values) {
        memcpy((char*)data.c_str() + offs, v.second.name, SV_NAMESZ);
        memcpy((char*)data.c_str() + offs + SV_NAMESZ, &v.second.type, SINT);
        memcpy((char*)data.c_str() + offs + SV_NAMESZ + SINT, v.second.vals, SV_PACKETSZ * SINT);
        offs += valSz;
      }

      memcpy((char*)data.c_str() + offs, "=end=", endSz);

      std::string out;
      return SV_Aux::TCPClient::sendData(data, out, false, true);
    }

    void sendCycle() {

      uint64_t cTm = SV_Aux::currDateTimeSinceEpochMs(),
               prevTm = cTm;

      int tmDiff = SV_CYCLEREC_MS;

      while (!_thrStop) {

        if (!_isConnect)
          _isConnect = SV_Aux::TCPClient::connect(_addrServ, _portServ);

        cTm = SV_Aux::currDateTimeSinceEpochMs();
        tmDiff = int(cTm - prevTm) - (SV_CYCLEREC_MS - tmDiff);
        prevTm = cTm;
               
        _mtxUpdValue.lock();
        
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
        } else {
          _curCycle = 0;
          _isConnect = (_isConnect) ? sendData() : false;
        }
        _mtxUpdValue.unlock();
                
        if ((SV_CYCLEREC_MS - tmDiff) > 0)
          SV_Aux::sleepMs(SV_CYCLEREC_MS - tmDiff);
      }
    }
}