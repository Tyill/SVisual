//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//

#include "buffer_data.h"

#include <algorithm>
#include <cstring>

using namespace std;

namespace {
  size_t calcBuffSz(int packetSz) {
    size_t buffSz = SV_VALUE_MAX_CNT * 10;
    if (packetSz > 100000) {
      buffSz /= 100;
    } else if (packetSz > 10000) {
      buffSz /= 10;
    }
    return buffSz;
  }

  size_t boundedCStrLen(const char* s, size_t maxLen) {
    const void* end = std::memchr(s, '\0', maxLen);
    return end ? static_cast<const char*>(end) - s : maxLen;
  }
}

void BufferData::init(const SV_Srv::Config& cng_) {
  
  cng = cng_;
  m_buffSz = calcBuffSz(cng.packetSz);
  m_buffReadPos = 0;
  m_buffWritePos = 0;
  m_buffWritePosForReader = 0;
  m_timeOffsetMs.clear();

  m_valueStorage = std::make_unique<SV_Base::Value[]>(SV_PACKETSZ * m_buffSz);
  memset(m_valueStorage.get(), 0, SV_PACKETSZ * m_buffSz * sizeof(SV_Base::Value));

  m_buffer.resize(m_buffSz);
  for (size_t i = 0; i < m_buffSz; ++i){
    m_buffer[i].data.vals = &m_valueStorage[i * SV_PACKETSZ];
  }
}

void BufferData::updateDataSignals(std::string_view indata, uint64_t bTm){

  const size_t dsz = indata.size();
  if (dsz < SV_NAMESZ) return;

  const size_t valSz = SV_NAMESZ + sizeof(SV_Base::ValueType) + sizeof(SV_Base::Value) * SV_PACKETSZ;
  const size_t maxBySize = (dsz - SV_NAMESZ) / valSz;
  const size_t valCnt = std::min(maxBySize, m_buffSz / 10);

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
  size_t cvalCnt = 0;

  const size_t modLen = boundedCStrLen(indata.data(), SV_NAMESZ);
  const std::string moduleStr(indata.data(), modLen);

  while (cPos + valSz <= dsz && cvalCnt < valCnt){
    m_buffer[wPos].module = moduleStr;
    m_buffer[wPos].name = std::string(indata.data() + cPos, SV_NAMESZ);
    memcpy(&m_buffer[wPos].type, indata.data() + cPos + SV_NAMESZ, sizeof(int32_t));
    memcpy(m_buffer[wPos].data.vals, indata.data() + cPos + SV_NAMESZ + sizeof(SV_Base::ValueType), vlsz);
    m_buffer[wPos].data.beginTime = bTm;
    ++wPos;
    if (wPos == m_buffSz){
      wPos = 0;
    }
    cPos += valSz;
    ++cvalCnt;
  }

  if (cvalCnt == 0) {
    return;
  }

  while (true){ 
    std::lock_guard<std::mutex> lck(m_mtxRead);
    if (wPosMem == m_buffWritePosForReader){
      m_buffWritePosForReader += cvalCnt;
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
