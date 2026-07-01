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
#include "SVMisc/misc.h"
#include <ctime>
#include <thread>
#include <chrono>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <errno.h>
#endif

namespace SV_Misc {
namespace {

bool localTm(time_t t, tm& out) {
#ifdef _WIN32
  return localtime_s(&out, &t) == 0;
#else
  return localtime_r(&t, &out) != nullptr;
#endif
}

bool gmTm(time_t t, tm& out) {
#ifdef _WIN32
  return gmtime_s(&out, &t) == 0;
#else
  return gmtime_r(&t, &out) != nullptr;
#endif
}

bool isDirectory(const std::string& path) {
#if defined(_WIN32)
  const DWORD attr = GetFileAttributesA(path.c_str());
  if (attr == INVALID_FILE_ATTRIBUTES)
    return false;
  return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
  struct stat st{};
  if (stat(path.c_str(), &st) != 0)
    return false;
  return S_ISDIR(st.st_mode);
#endif
}

bool mkdirSegment(const std::string& path) {
  if (path.empty())
    return true;
#if defined(_WIN32)
  if (CreateDirectoryA(path.c_str(), NULL))
    return true;
  if (GetLastError() == ERROR_ALREADY_EXISTS)
    return isDirectory(path);
  return false;
#else
  if (mkdir(path.c_str(), 0755) == 0)
    return true;
  if (errno == EEXIST)
    return isDirectory(path);
  return false;
#endif
}

std::string parentDirectoryPath(const std::string& path) {
  const auto pos = path.find_last_of("/\\");
  if (pos == std::string::npos)
    return {};
  return path.substr(0, pos);
}

} // namespace

  std::string currDateS() {
    time_t ct = time(nullptr);
    tm lct{};
    if (!localTm(ct, lct))
      return {};

    char curDate[32];
    strftime(curDate, sizeof(curDate), "%Y%m%d", &lct);
    return curDate;
  }

  std::string currDateTimeEx() {
    time_t ct = time(nullptr);
    tm lct{};
    if (!localTm(ct, lct))
      return {};

    char curDate[32];
    strftime(curDate, sizeof(curDate), "%Y%m%d_%H%M", &lct);
    return curDate;
  }

  std::string currDateTime() {
    time_t ct = time(nullptr);
    tm lct{};
    if (!localTm(ct, lct))
      return {};

    char curDate[32];
    strftime(curDate, sizeof(curDate), "%d-%m-%y %H:%M:%S", &lct);
    return curDate;
  }

  std::string currDateTimeMs() {
    time_t ct = time(nullptr);
    tm lct{};
    if (!localTm(ct, lct))
      return {};

    const uint64_t ms = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now()).time_since_epoch().count() % 1000;

    char curDate[32];
    strftime(curDate, sizeof(curDate), "%Y-%m-%d %H:%M:%S:", &lct);
    return std::string(curDate) + std::to_string(ms);
  }

  uint64_t currDateTimeSinceEpochMs() {
    const auto now = std::chrono::system_clock::now();
    return std::chrono::time_point_cast<std::chrono::milliseconds>(now)
        .time_since_epoch().count();
  }

  int hourOffsFromUTC() {
    const time_t rawtime = time(nullptr);
    tm gmt{};
    tm local{};
    if (!gmTm(rawtime, gmt) || !localTm(rawtime, local))
      return 0;

    const time_t gmtSec = mktime(&gmt);
    return int((rawtime - gmtSec + (local.tm_isdst ? 3600 : 0)) / 3600);
  }

  bool createSubDirectory(std::string path) {
    if (path.empty())
      return true;

    const std::string dirPath = parentDirectoryPath(path);
    if (dirPath.empty())
      return true;

    std::string accumulated;
    accumulated.reserve(dirPath.size());

    for (size_t i = 0; i < dirPath.size(); ++i) {
      const char ch = dirPath[i];
      if (ch != '\\' && ch != '/') {
        accumulated += ch;
        continue;
      }

#if defined(_WIN32)
      accumulated += '\\';
#else
      accumulated += '/';
#endif
      if (accumulated.size() <= 1)
        continue;

      if (!mkdirSegment(accumulated))
        return false;
    }

    if (!accumulated.empty() && !mkdirSegment(accumulated))
      return false;

    return true;
  }

  void sleepMs(uint64_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }
}
