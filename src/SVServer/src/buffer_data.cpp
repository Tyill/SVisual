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
  if (SV_PACKETSZ > 100000){
      m_buffSz /= 100;
  }else if (SV_PACKETSZ > 10000){
      m_buffSz /= 10;
  }
  m_buffer.resize(m_buffSz);
  SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * m_buffSz];
  memset(buff, 0, SV_PACKETSZ * m_buffSz * sizeof(SV_Base::Value));
  for (size_t i = 0; i < m_buffSz; ++i){
    m_buffer[i].data.vals = &buff[i * SV_PACKETSZ];
  }
}

namespace {
  size_t boundedCStrLen(const char* s, size_t maxLen) {
    const void* end = std::memchr(s, '\0', maxLen);
    return end ? static_cast<size_t>(static_cast<const char*>(end) - s) : maxLen;
  }
}

void BufferData::updateDataSignals(std::string_view indata, uint64_t bTm){

  const size_t dsz = indata.size();
  if (dsz < SV_NAMESZ) return;

  const size_t valSz = SV_NAMESZ + sizeof(SV_Base::ValueType) + sizeof(SV_Base::Value) * SV_PACKETSZ;
  const size_t valCnt = std::min((dsz - SV_NAMESZ) / valSz, m_buffSz / 10);
  if (valCnt == 0) return;

  size_t wPos, wPosMem;
  {
    std::lock_guard<std::mutex> lck(m_mtxWrite);
    wPos = wPosMem = m_buffWritePos;
    m_buffWritePos += valCnt;
    if (m_buffWritePos >= m_buffSz){
      m_buffWritePos -= m_buffSz;
    }
    if (cng.offsetMs > 0){
      const size_t modLen = boundedCStrLen(indata.data(), SV_NAMESZ);
      const std::string module(indata.data(), modLen);
      if (m_timeOffsetMs.count(module)){
        m_timeOffsetMs[module] += cng.offsetMs;
      }else{
        m_timeOffsetMs[module] = 0;
      }
      bTm += m_timeOffsetMs[module];
    }
  }
  const size_t vlsz = sizeof(SV_Base::Value) * SV_PACKETSZ;
  size_t cPos = SV_NAMESZ;

  const size_t modLen = boundedCStrLen(indata.data(), SV_NAMESZ);
  const std::string moduleStr(indata.data(), modLen);

  for (size_t i = 0; i < valCnt; ++i){
    m_buffer[wPos].module = moduleStr;
    m_buffer[wPos].name = std::string(indata.data() + cPos, boundedCStrLen(indata.data() + cPos, SV_NAMESZ));
    memcpy(&m_buffer[wPos].type, indata.data() + cPos + SV_NAMESZ, sizeof(int32_t));
    memcpy(m_buffer[wPos].data.vals, indata.data() + cPos + SV_NAMESZ + sizeof(SV_Base::ValueType), vlsz);
    m_buffer[wPos].data.beginTime = bTm;
    ++wPos;
    if (wPos == m_buffSz){
      wPos = 0;
    }
    cPos += valSz;
  }

  while (true){ 
    std::lock_guard<std::mutex> lck(m_mtxRead);
    if (wPosMem == m_buffWritePosForReader){ // для защиты от гонки, если из др потока сюда дойдут раньше
      m_buffWritePosForReader += valCnt;
      if (m_buffWritePosForReader >= m_buffSz){
        m_buffWritePosForReader -= m_buffSz;
      }
      break;
    }
  }
}

bool BufferData::getDataByReadPos(std::vector<InputData>& out){

  size_t buffWritePos;
  {
    std::lock_guard<std::mutex> lck(m_mtxRead);
    buffWritePos = m_buffWritePosForReader;
  }  
  out.clear();
  while(m_buffReadPos != buffWritePos){
    out.push_back(m_buffer[m_buffReadPos]);

    ++m_buffReadPos;
    if (m_buffReadPos == m_buffSz){
      m_buffReadPos = 0;
    }
  }
  return !out.empty();
}
