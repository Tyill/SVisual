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

#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "auxFunc.h"

namespace SV_Aux {
  class Logger {  
  public:
     
    Logger(const std::string& pathFile = ""):
      _pathFile(pathFile){

      createSubDirectory(pathFile);

      _deqMess.resize(MAX_CNT_MESS);
      
      _thrWriteMess = std::thread(&Logger::writeCycle, this);
    }
    
    ~Logger() {
      _fStop = true;
      _cval.notify_one();
      if (_thrWriteMess.joinable()) _thrWriteMess.join();
    }
    
    bool writeLine(const std::string &mess) {

      _mtxWr.lock();

      _deqMess[_writeMessCnt] = Message{ true, currDateTimeMs(), mess };
      ++_writeMessCnt;
      if (_writeMessCnt >= MAX_CNT_MESS) _writeMessCnt = 0;

      _mtxWr.unlock();

      _cval.notify_one();

      return true;
    }

  private:

    const int MAX_CNT_MESS = 100;

    std::string _pathFile;

    int _readMessCnt = 0, _writeMessCnt = 0;

    struct Message {
      bool activ;
      std::string cTime;
      std::string mess;
        
    };
    std::vector<Message> _deqMess;

    std::mutex _mtxWr, _mtxRd;
    std::thread _thrWriteMess;
    std::condition_variable _cval;
    bool _fStop = false;
    
    void writeCycle() {

      while (!_fStop) {

        std::unique_lock<std::mutex> lck(_mtxRd);
        _cval.wait(lck);
                
        std::ofstream slg(_pathFile.c_str(), std::ios::app);
        while (_deqMess[_readMessCnt].activ) {

          slg << "[" << _deqMess[_readMessCnt].cTime << "] " << _deqMess[_readMessCnt].mess << std::endl;

          _deqMess[_readMessCnt].activ = false;
          ++_readMessCnt;
          if (_readMessCnt >= MAX_CNT_MESS) _readMessCnt = 0;
        }
        slg.close();
      }
    }
  };
}