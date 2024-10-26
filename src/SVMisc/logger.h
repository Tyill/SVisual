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

#include "misc.h"

#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

namespace SV_Misc {
  class Logger {  
  public:
     
    Logger(const std::string& pathFile = ""):
      pathFile_(pathFile){

      createSubDirectory(pathFile);

      deqMess_.resize(MAX_CNT_MESS);
      
      thrWriteMess_ = std::thread(&Logger::writeCycle, this);
    }
    
    ~Logger() {
      {
        std::lock_guard<std::mutex> lck (mtxRd_);
        fStop_ = true;
        cval_.notify_one();
      }      
      if (thrWriteMess_.joinable()){
        thrWriteMess_.join();
      }
    }

    void setPathFile(const std::string& pathFile) {
      createSubDirectory(pathFile);

      std::lock_guard<std::mutex> lck(mtxRd_);
      
      pathFile_ = pathFile;
    }
    
    void writeLine(const std::string &mess) {

      std::lock_guard<std::mutex> lck(mtxWr_);

      deqMess_[writeMessCnt_] = Message{ currDateTimeMs(), mess };
      ++writeMessCnt_;
      if (writeMessCnt_ >= MAX_CNT_MESS){
        writeMessCnt_ = 0;
      }
      cval_.notify_one();
    }

  private:

    const int MAX_CNT_MESS = 100;

    std::string pathFile_;

    int readMessCnt_ = 0, writeMessCnt_ = 0;

    struct Message { 
      std::string cTime;
      std::string mess;        
    };
    std::vector<Message> deqMess_;

    std::mutex mtxWr_, mtxRd_;
    std::thread thrWriteMess_;
    std::condition_variable cval_;
    std::atomic_bool fStop_{};
    
    void writeCycle() {

      while (!fStop_) {

        std::unique_lock<std::mutex> lck(mtxRd_);
        cval_.wait(lck);

        std::ofstream slg(pathFile_.c_str(), std::ios::app);
        std::vector<Message> mess;
        while (readMess(mess)) {
          for (const auto& m : mess){
            slg << "[" << m.cTime << "] " << m.mess << std::endl;
          }
        }
        slg.close();
      }
    }
    bool readMess(std::vector<Message>& mess){
      std::lock_guard<std::mutex> lck(mtxWr_);
      
      mess.clear();
      while (readMessCnt_ != writeMessCnt_){
        mess.push_back(deqMess_[readMessCnt_]);
     
        ++readMessCnt_;
        if (readMessCnt_ >= MAX_CNT_MESS){
          readMessCnt_ = 0;
        } 
      }
      return !mess.empty();
    }
  };
}