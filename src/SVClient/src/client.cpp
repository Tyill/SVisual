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
#include "SVBase/sv_limits.h"
#include "SVBase/base.h"
#include "SVMisc/tcp_client.h"
#include "SVMisc/misc.h"

#include <string>
#include <mutex>
#include <thread>
#include <map>
#include <vector>
#include <cstring>
#include <atomic>

struct Config {

  int cycleRecMs;
  int packetSz;

  Config(int cycleRecMs_ = 100, int packetSz_ = 10) :
    cycleRecMs(cycleRecMs_), packetSz(packetSz_){}
};

struct ValueRec {
  bool isActive{};
  bool isOnlyFront{};
  char name[SV_NAMESZ]{};
  SV_Base::ValueType type{};
  std::vector<SV_Base::Value> vals;
};

std::atomic_bool _isConnect{false};
std::atomic_bool _thrStop{false};

std::thread _thr;
std::mutex _mtx;
std::mutex _sessionMtx;
std::mutex _tcpMtx;

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

      if (!moduleName || !ipAddr)
        return false;

      if ((strlen(moduleName) == 0) || (strlen(moduleName) >= SV_NAMESZ) ||
           strstr(moduleName, messEnd) || strstr(moduleName, messBegin)) {
        return false;
      }

      std::lock_guard<std::mutex> sessionLck(_sessionMtx);

      if (_thr.joinable())
        return _isConnect.load(std::memory_order_acquire);

      _thrStop.store(false);

      {
        std::lock_guard<std::mutex> dataLck(_mtx);
        _curCycle = 0;
      }

      _module = moduleName;
      _addrServ = ipAddr;
      _portServ = port;

      bool connected = false;
      {
        std::lock_guard<std::mutex> tcpLck(_tcpMtx);
        connected = SV_Misc::TCPClient::connect(ipAddr, port);
        _isConnect.store(connected, std::memory_order_release);
      }

      _thr = std::thread(sendCycle);

      return connected;
    }

    void svDisconnect() {

      std::lock_guard<std::mutex> sessionLck(_sessionMtx);

      _thrStop.store(true);
      if (_thr.joinable())
        _thr.join();

      {
        std::lock_guard<std::mutex> tcpLck(_tcpMtx);
        SV_Misc::TCPClient::disconnect();
        _isConnect.store(false, std::memory_order_release);
      }

      _thrStop.store(false);
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

        std::lock_guard<std::mutex> sessionLck(_sessionMtx);
        const bool sessionActive = _thr.joinable();

        std::lock_guard<std::mutex> dataLck(_mtx);
        if (!_values.empty() || sessionActive)
          return false;
        if (cycleRecMs < 1 || packetSz < 1 || packetSz > SV_PACKETSZ_MAX)
          return false;

        cng = Config(cycleRecMs, packetSz);
        return true;
    }

    bool addValue(const char* name, SV_Base::ValueType type, SV_Base::Value val, bool onlyPosFront) {

      if (!name)
        return false;
      if (strlen(name) == 0 || strlen(name) >= SV_NAMESZ ||
          strstr(name, messEnd) || strstr(name, messBegin)) {
        return false;
      }

      std::lock_guard<std::mutex> lck(_mtx);

      auto [it, inserted] = _values.try_emplace(name);
      if (inserted) {
        ValueRec& vr = it->second;
        strncpy(vr.name, name, SV_NAMESZ - 1);
        vr.type = type;
        vr.isOnlyFront = onlyPosFront;
        vr.isActive = false;
        vr.vals.resize(cng.packetSz);
      } else if (it->second.type != type) {
        return false;
      }

      it->second.vals[_curCycle] = val;
      it->second.isActive = true;
      return true;
    }

    std::string prepareData() {

      if (_values.empty()) return "";

      const int packetSz = cng.packetSz;
      const size_t szInt = sizeof(int32_t);
      const size_t valSz = SV_NAMESZ + sizeof(SV_Base::ValueType) +
                           sizeof(SV_Base::Value) * static_cast<size_t>(packetSz);
      const size_t dataSz = SV_NAMESZ + valSz * _values.size();
      const size_t startSz = strlen(messBegin);
      const size_t endSz = strlen(messEnd);
      const size_t messSz = startSz + szInt + dataSz + endSz;

      std::string data(messSz, '\0');

      char* dptr = data.data();
      size_t offs = 0;
      memcpy(dptr, messBegin, startSz);               offs += startSz;

      const int32_t dataSzField = static_cast<int32_t>(dataSz);
      memcpy(dptr + offs, &dataSzField, szInt);       offs += szInt;

      char modBuf[SV_NAMESZ] = {};
      strncpy(modBuf, _module.c_str(), SV_NAMESZ - 1);
      memcpy(dptr + offs, modBuf, SV_NAMESZ);         offs += SV_NAMESZ;

      const size_t valsBytes = sizeof(SV_Base::Value) * static_cast<size_t>(packetSz);
      for (const auto& v : _values) {
        memcpy(dptr + offs, v.second.name, SV_NAMESZ);
        offs += SV_NAMESZ;
        memcpy(dptr + offs, &v.second.type, sizeof(SV_Base::ValueType));
        offs += sizeof(SV_Base::ValueType);
        memcpy(dptr + offs, v.second.vals.data(), valsBytes);
        offs += valsBytes;
      }

      memcpy(dptr + offs, messEnd, endSz);

      return data;
    }

    void sendCycle() {

      uint64_t cTm = SV_Misc::currDateTimeSinceEpochMs(),
               prevTm = cTm;

      int cDelay = 0;

      while (!_thrStop.load(std::memory_order_acquire)) {

        if (!_isConnect.load(std::memory_order_acquire)) {
          std::lock_guard<std::mutex> tcpLck(_tcpMtx);
          const bool ok = SV_Misc::TCPClient::connect(_addrServ, _portServ);
          _isConnect.store(ok, std::memory_order_release);
        }

        cTm = SV_Misc::currDateTimeSinceEpochMs();
        int tmDiff = int(cTm - prevTm) - cDelay;
        prevTm = cTm;

        std::string data;
        int cycleRecMs = 0;
        const bool isConnected = _isConnect.load(std::memory_order_acquire);

        {
          std::lock_guard<std::mutex> lck(_mtx);

          cycleRecMs = cng.cycleRecMs;
          const int packetSz = cng.packetSz;

          int prevCyc = _curCycle - 1;
          if (prevCyc < 0)
            prevCyc = packetSz - 1;

          for (auto it = _values.begin(); it != _values.end(); ++it) {
            if (!it->second.isActive) {
              it->second.vals[_curCycle] = it->second.vals[prevCyc];

              if ((it->second.type == SV_Base::ValueType::BOOL) && it->second.isOnlyFront)
                it->second.vals[_curCycle].vBool = false;
            }
            it->second.isActive = false;
          }

          if (_curCycle < packetSz - 1) {
            ++_curCycle;
          } else {
            _curCycle = 0;
            if (isConnected && !_values.empty()) {
              data = prepareData();
            }
          }
        }

        if (isConnected && !data.empty()) {
          std::lock_guard<std::mutex> tcpLck(_tcpMtx);
          std::string out;
          const bool ok = SV_Misc::TCPClient::sendData(data, out, false, true);
          if (!ok)
            _isConnect.store(false, std::memory_order_release);
        }

        cDelay = (cycleRecMs - tmDiff) > 0 ? (cycleRecMs - tmDiff) : 0;
        if (cDelay > 0) {
          SV_Misc::sleepMs(cDelay);
        }
      }
    }
}
