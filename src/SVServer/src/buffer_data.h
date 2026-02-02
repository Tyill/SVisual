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

#include "SVServer/sv_server.h"
#include "SVBase/sv_limits.h"

#include <mutex>

class BufferData
{
public:
   
  BufferData() = default;

  void init(const SV_Srv::Config&);

  struct InputData{
    std::string name;          
    std::string module;        
    SV_Base::ValueType type;   
    SV_Base::RecData data;     
  };

  void updateDataSignals(const std::string& in, uint64_t bTm);

  bool getDataByReadPos(std::vector<InputData>&);

private:

  SV_Srv::Config cng;

  size_t m_buffSz = SV_VALUE_MAX_CNT * 10; // 10 сек - запас
  std::vector<InputData> m_buffer;

  size_t m_buffReadPos = 0;
  size_t m_buffWritePos = 0;
  size_t m_buffWritePosForReader = 0;

  std::map<std::string, uint64_t> m_timeOffsetMs;

  std::mutex m_mtxRead, m_mtxWrite;
};
