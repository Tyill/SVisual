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

namespace SV_Misc {
  class Logger {
  public:
    explicit Logger(const std::string& pathFile = "") :
      pathFile_(pathFile) {

      createSubDirectory(pathFile);
      deqMess_.resize(MAX_CNT_MESS);
      thrWriteMess_ = std::thread(&Logger::writeCycle, this);
    }

    ~Logger() {
      {
        std::lock_guard<std::mutex> lck(mtx_);
        fStop_ = true;
        cval_.notify_one();
      }
      if (thrWriteMess_.joinable())
        thrWriteMess_.join();
    }

    void setPathFile(const std::string& pathFile) {
      createSubDirectory(pathFile);
      std::lock_guard<std::mutex> lck(mtx_);
      pathFile_ = pathFile;
    }

    void writeLine(const std::string& mess) {
      std::lock_guard<std::mutex> lck(mtx_);

      const int nextWrite = (writeMessCnt_ + 1) % MAX_CNT_MESS;
      if (nextWrite == readMessCnt_)
        return;

      deqMess_[writeMessCnt_] = Message{ currDateTimeMs(), mess };
      writeMessCnt_ = nextWrite;
      cval_.notify_one();
    }

  private:
    static const int MAX_CNT_MESS = 100;

    struct Message {
      std::string cTime;
      std::string mess;
    };

    std::string pathFile_;
    int readMessCnt_ = 0;
    int writeMessCnt_ = 0;

    std::vector<Message> deqMess_;

    std::mutex mtx_;
    std::thread thrWriteMess_;
    std::condition_variable cval_;
    bool fStop_ = false;

    std::vector<Message> drainLocked() {
      std::vector<Message> mess;
      while (readMessCnt_ != writeMessCnt_) {
        mess.push_back(deqMess_[readMessCnt_]);
        readMessCnt_ = (readMessCnt_ + 1) % MAX_CNT_MESS;
      }
      return mess;
    }

    static void writeBatch(const std::string& path, const std::vector<Message>& mess) {
      if (mess.empty() || path.empty())
        return;

      std::ofstream slg(path.c_str(), std::ios::app);
      if (!slg.good())
        return;

      for (const auto& m : mess)
        slg << "[" << m.cTime << "] " << m.mess << std::endl;
    }

    void writeCycle() {
      while (true) {
        std::vector<Message> batch;
        std::string path;
        bool stop = false;

        {
          std::unique_lock<std::mutex> lck(mtx_);
          cval_.wait(lck, [&] {
            return fStop_ || readMessCnt_ != writeMessCnt_;
          });

          batch = drainLocked();
          path = pathFile_;
          stop = fStop_;
        }

        writeBatch(path, batch);

        if (stop) {
          std::vector<Message> tail;
          std::string tailPath;
          {
            std::lock_guard<std::mutex> lck(mtx_);
            tail = drainLocked();
            tailPath = pathFile_;
          }
          writeBatch(tailPath, tail);
          break;
        }
      }
    }
  };
}
