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

#include "buffer_data.h"

#include <algorithm>
#include <cstring>

using namespace std;

void BufferData::init(const SV_Srv::Config& cng_) {
  
  cng = cng_;

  SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * BUFF_SZ];
  memset(buff, 0, SV_PACKETSZ * BUFF_SZ * sizeof(SV_Base::Value));
  for (size_t i = 0; i < BUFF_SZ; ++i)
    m_buffer[i].data.vals = &buff[i * SV_PACKETSZ];
}

void BufferData::updateDataSignals(const std::string& indata, uint64_t bTm){

  size_t dsz = indata.size(),
         valSz = SV_NAMESZ + sizeof(SV_Base::ValueType) + sizeof(SV_Base::Value) * SV_PACKETSZ,
         cPos = SV_NAMESZ;
  size_t valCnt = std::max(size_t(0), std::min((dsz - cPos) / valSz, BUFF_SZ / 10));  // 10 сек - запас
  size_t wPos;
  {
    std::lock_guard<std::mutex> lck(m_mtxWrite);
    wPos = m_buffWritePos;
    m_buffWritePos += valCnt;
    if (m_buffWritePos >= BUFF_SZ){
      m_buffWritePos -= BUFF_SZ;
    }
  }
  size_t vlsz = sizeof(SV_Base::Value) * SV_PACKETSZ;
  while (cPos < dsz){
    m_buffer[wPos].module = indata.c_str();
    m_buffer[wPos].name = indata.data() + cPos;
    m_buffer[wPos].type = SV_Base::ValueType(*(indata.data() + cPos + SV_NAMESZ));
    m_buffer[wPos].data.beginTime = bTm;
    memcpy(m_buffer[wPos].data.vals, indata.data() + cPos + SV_NAMESZ + sizeof(SV_Base::ValueType), vlsz);
    ++wPos;
    if (wPos == BUFF_SZ){
      wPos = 0;
    }
    cPos += valSz;
  }
  {
    std::lock_guard<std::mutex> lck(m_mtxWrite);
    m_buffWritePosForReader += valCnt;
    if (m_buffWritePosForReader >= BUFF_SZ){
      m_buffWritePosForReader -= BUFF_SZ;
    }
  }
}

bool BufferData::getDataByReadPos(std::vector<InputData>& out){

  size_t buffWritePos;
  {
    std::lock_guard<std::mutex> lck(m_mtxWrite);
    buffWritePos = m_buffWritePosForReader;
  }
  out.clear();
  while(m_buffReadPos != buffWritePos){
    out.push_back(m_buffer[m_buffReadPos]);

    ++m_buffReadPos;
    if (m_buffReadPos == BUFF_SZ){
      m_buffReadPos = 0;
    }
  }
  return !out.empty();
}
