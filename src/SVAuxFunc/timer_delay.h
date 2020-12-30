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
#pragma once

#include <vector>
#include <ctime>
#include "aux_func.h"

namespace SV_Aux {

  /// Таймер задержки однопоточный
  class TimerDelay {

  public:

    TimerDelay() {

      _cycleTm = _prevCycTm = currDateTimeSinceEpochMs();
      time_t t = time(nullptr);
      _prevTm = *localtime(&t);
    }
        
    void update() {

      uint64_t ct = currDateTimeSinceEpochMs();
      _cycleTm = ct - _prevCycTm;
      _prevCycTm = ct;
            
      for (int i = 0; i < _tmrCount; ++i) {

        if (!tmrs[i].active) tmrs[i].count = 0;

        tmrs[i].active = false;
      }

      // импульсы времени
      time_t t = time(nullptr);
      tm* lct = localtime(&t);
      _hourOnc = (lct->tm_hour != _prevTm.tm_hour);
      _minOnc = (lct->tm_min != _prevTm.tm_min);
      _secOnc = (lct->tm_sec != _prevTm.tm_sec);
      _prevTm = *lct;
    }

    uint64_t getCTime(){

      return currDateTimeSinceEpochMs() - _prevCycTm;
    }
        
    bool onDelaySec(bool start, uint32_t delay, int id) {

      if (id >= _tmrCount){
        tmrs.resize(id + 1, TmBase{ 0, false });
        _tmrCount = id + 1;
      }

      bool res = false;
      if (start) {
        tmrs[id].count += _cycleTm;
        if (tmrs[id].count >= delay * 1000) res = true;
      }
      else tmrs[id].count = 0;

      tmrs[id].active = true;

      return res;
    }
        
    bool offDelaySec(bool start, uint32_t delay, int id) {
           
      if (id >= _tmrCount){
        tmrs.resize(id + 1, TmBase{ 0, false });
        _tmrCount = id + 1;
      }

      bool res = false;
      if (start){
        tmrs[id].count = delay * 1000;
      } else {
        if (tmrs[id].count > 0) {
          res = true;
          tmrs[id].count -= _cycleTm;
        }
      }

      tmrs[id].active = true;

      return (start || res);
    }
        
    bool onDelayMS(bool start, uint32_t delay, int id) {
            
      if (id >= _tmrCount){
        tmrs.resize(id + 1, TmBase{ 0, false });
        _tmrCount = id + 1;
      }

      bool res = false;
      if (start) {
        tmrs[id].count += _cycleTm;
        if (tmrs[id].count >= delay) res = true;
      }
      else tmrs[id].count = 0;

      tmrs[id].active = true;

      return res;
    }
        
    bool offDelayMS(bool start, uint32_t delay, int id) {
           
      if (id >= _tmrCount){
        tmrs.resize(id + 1, TmBase{ 0, false });
        _tmrCount = id + 1;
      }

      bool res = false;
      if (start){
        tmrs[id].count = delay;
      } else {
        if (tmrs[id].count > 0) {
          res = true;
          tmrs[id].count -= _cycleTm;
        }
      }

      tmrs[id].active = true;

      return (start || res);
    }
        
    bool secOnc() {
      return _secOnc;
    }

    bool minOnc() {
      return _minOnc;
    }

    bool hourOnc() {
      return _hourOnc;
    }

  private:

    struct TmBase{
      uint64_t count;         
      bool active;           
    };

    std::vector<TmBase> tmrs;
    int _tmrCount = 0;

    uint64_t _prevCycTm;            
    uint64_t _cycleTm;            

    tm _prevTm;

    bool _secOnc = false;
    bool _minOnc = false;
    bool _hourOnc = false;
  };
}