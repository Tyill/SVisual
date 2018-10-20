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
#include "SVAuxFunc/auxFunc.h"
#include <fstream>
#include <time.h>
#include <thread>
#include <chrono>
#include <cstring>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
   #include <windows.h>
#endif

using namespace std;

namespace SV_Aux {

    // тек дата %Y%m%d
    string CurrDateS() {

        time_t ct = time(nullptr);
        tm* lct = localtime(&ct);

        char curDate[32];
        strftime(curDate, 32, "%Y%m%d", lct);

        return curDate;
    }

    // тек дата-время %Y%m%d_%H%M
    string CurrDateTimeEx() {

        time_t ct = time(nullptr);
        tm* lct = localtime(&ct);

        char curDate[32];
        strftime(curDate, 32, "%Y%m%d_%H%M", lct);

        return curDate;
    }

    // тек дата-время %Y-%m-%d %H:%M:%S
    string CurrDateTime() {

        time_t ct = time(nullptr);
        tm* lct = localtime(&ct);

        char curDate[32];
        strftime(curDate, 32, "%d-%m-%y %H:%M:%S", lct);

        return curDate;
    }

    // тек дата-время %Y-%m-%d %H:%M:%S:%MS
    string CurrDateTimeMs() {

        time_t ct = time(nullptr);
        tm* lct = localtime(&ct);

        uint64_t ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
        uint64_t mspr = ms / 1000;
        ms -= mspr * 1000;

        char curDate[32];
        strftime(curDate, 32, "%Y-%m-%d %H:%M:%S:", lct);

        sprintf(curDate, "%s%03d", curDate, ms);

        return curDate;
    }

     // тек дата-время %Y-%m-%d %H:%M:%S
    std::string CurrDateTimeSQL() {

        time_t ct = time(nullptr);
        tm* lct = localtime(&ct);

        char curDate[32];
        strftime(curDate, 32, "%Y-%m-%d %H:%M:%S", (const tm *) &lct);

        return curDate;

    }

    uint64_t CurrDateTimeSinceEpochMs(){

        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

       return now_ms.time_since_epoch().count();
    }

    int HourOffsFromUTC() {

        time_t rawtime = time(NULL);
        struct tm *ptm = gmtime(&rawtime);
        time_t gmt = mktime(ptm);
        ptm = localtime(&rawtime);

        return (rawtime - gmt + (ptm->tm_isdst ? 3600 : 0)) / 3600;
    }

    vector<string> split(string str, const char *sep) {

        char *cstr = (char *) str.c_str();

        vector<string> res;
        char *pch = strtok(cstr, sep);
        while (pch != NULL) {
            res.push_back(string(pch));
            pch = strtok(NULL, sep);
        }

        return res;
    }


    bool is_fileExist(const std::string &name) {
        ifstream f(name.c_str());
        return f.good();
    }

    // автосоздание субдиректорий
    bool CreateSubDirectory(string strDirs) {
        if (is_fileExist(strDirs)) return true;

        int sz = strDirs.size(), ret = 0;
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

    void SleepMs(int ms){
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }    
}