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
#include "SVServer/sv_server.h"

#include <map>
#include <thread>

class ClickHouseDB;

// копирование архива на диск
class Archive
{
public:
    
  Archive() = default;

  void init(const SV_Srv::Config&);

  void copyToDisk(bool isStop);

  void addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype);
  void addValue(const std::string& sign, const SV_Base::RecData& rd);

  void setConfig(const SV_Srv::Config&);

private:
  void copyToDiskImpl(bool isStop, int archiveIndex);
  bool isCopyTimeHour();
  std::string getOutPath(bool isStop);
  bool compressData(size_t insz, const std::vector<char>& inArr, size_t& outsz, std::vector<char>& outArr);

private:

  SV_Srv::Config cng;

  std::string m_copyStartTime = "";
  std::string m_copyDateMem = "";

  int m_crtFileHour = 0;
  int m_archiveIndex = 0;
  size_t m_copySz = 0;
  std::map<std::string, int> m_valPos[2];

  SV_Misc::Front m_front;

  std::map<std::string, std::vector<SV_Base::RecData>> m_archiveData[2];

  ClickHouseDB* m_chdb{};
  std::shared_ptr<std::thread> m_saveThread;

};
