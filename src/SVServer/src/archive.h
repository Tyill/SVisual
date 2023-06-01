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

#include "SVMisc/front.h"
#include "SVServer/server.h"

#include <map>

class ClickHouseDB;

// копирование архива на диск
class Archive
{
public:
    
  Archive() = default;

  void init(const SV_Srv::Config&);

  bool copyToDisk(bool isStop);

  void addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype);
  void addValue(const std::string& sign, const SV_Base::RecData& rd);

  void setConfig(const SV_Srv::Config&);

private:
  bool isCopyTimeHour();
  std::string getOutPath(bool isStop);
  bool compressData(size_t insz, const std::vector<char>& inArr, size_t& outsz, std::vector<char>& outArr);

private:

  SV_Srv::Config cng;

  std::string _copyStartTime = "";
  std::string _copyDateMem = "";

  int _crtFileHour = 0;
  const int ARCH_CYCLE_MS = 600000;     // 10мин
  size_t _copySz = 0;
  std::map<std::string, int> _valPos;

  SV_Misc::Front _front;

  std::map<std::string, std::vector<SV_Base::RecData>> _archiveData;

  ClickHouseDB* _chdb{};

};
