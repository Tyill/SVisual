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
#include "SVAuxFunc/aux_func.h"
#include <fstream>
#include <ctime>
#include <thread>
#include <chrono>
#include <cmath>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

using namespace std;

namespace SV_Aux {

  // тек дата %Y%m%d
  string currDateS() {

    time_t ct = time(nullptr);
    tm* lct = localtime(&ct);

    char curDate[32];
    strftime(curDate, 32, "%Y%m%d", lct);

    return curDate;
  }

  // тек дата-время %Y%m%d_%H%M
  string currDateTimeEx() {

    time_t ct = time(nullptr);
    tm* lct = localtime(&ct);

    char curDate[32];
    strftime(curDate, 32, "%Y%m%d_%H%M", lct);

    return curDate;
  }

  // тек дата-время %Y-%m-%d %H:%M:%S
  string currDateTime() {

    time_t ct = time(nullptr);
    tm* lct = localtime(&ct);

    char curDate[32];
    strftime(curDate, 32, "%d-%m-%y %H:%M:%S", lct);

    return curDate;
  }

  // тек дата-время %Y-%m-%d %H:%M:%S:%MS
  string currDateTimeMs() {

    time_t ct = time(nullptr);
    tm* lct = localtime(&ct);

    uint64_t ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    uint64_t mspr = ms / 1000;
    ms -= mspr * 1000;

    char curDate[32];
    strftime(curDate, 32, "%Y-%m-%d %H:%M:%S:", lct);
   
    return string(curDate) + to_string(ms);
  }

  // тек дата-время %Y-%m-%d %H:%M:%S
  std::string currDateTimeSQL() {

    time_t ct = time(nullptr);
    tm* lct = localtime(&ct);

    char curDate[32];
    strftime(curDate, 32, "%Y-%m-%d %H:%M:%S", (const tm *)&lct);

    return curDate;
  }

  uint64_t currDateTimeSinceEpochMs(){

    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

    return now_ms.time_since_epoch().count();
  }

  int hourOffsFromUTC() {

    time_t rawtime = time(NULL);
    struct tm *ptm = gmtime(&rawtime);
    time_t gmt = mktime(ptm);
    ptm = localtime(&rawtime);

    return int((rawtime - gmt + (ptm->tm_isdst ? 3600 : 0)) / 3600);
  }
  
  bool isFileExist(const std::string &name) {
    ifstream f(name.c_str());
    return f.good();
  }

  // автосоздание субдиректорий
  bool createSubDirectory(string strDirs) {
    if (isFileExist(strDirs)) return true;

    int sz = int(strDirs.size()), ret = 0;
    string strTmp = "";
    for (int i = 0; i < sz; ++i) {
      char ch = strDirs[i];
      if (ch != '\\' && ch != '/') strTmp += ch;
      else {
#if defined(_WIN32)
        strTmp += "\\";
        ret = CreateDirectoryA(strTmp.c_str(), NULL);
#else
        strTmp += "/";
        ret = mkdir(strTmp.c_str(), 0733);
#endif
      }
    }
    return ret == 0;
  }

  void sleepMs(uint64_t ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }
}