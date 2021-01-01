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

using namespace std;
using namespace SV_Base;

void BufferData::init(const SV_Srv::Config& cng_) {
  
  cng = cng_;

  Value* buff = new Value[SV_PACKETSZ * BUFF_SZ];
  memset(buff, 0, SV_PACKETSZ * BUFF_SZ * sizeof(Value));
  for (size_t i = 0; i < BUFF_SZ; ++i)
    _buffer[i].data.vals = &buff[i * SV_PACKETSZ];
}

void BufferData::updateDataSignals(const std::string& indata, uint64_t bTm){

  size_t dsz = indata.size(),
    valSz = SV_NAMESZ + sizeof(ValueType) + sizeof(Value) * SV_PACKETSZ,
    cPos = SV_NAMESZ;
  
  size_t valCnt = std::max(size_t(0), std::min((dsz - cPos) / valSz, BUFF_SZ));

  _mtx.lock();

  size_t buffWr = _buffWritePos;
  _buffWritePos += valCnt;

  if (_buffWritePos >= BUFF_SZ) _buffWritePos -= BUFF_SZ;

  _mtx.unlock();

  size_t vlsz = sizeof(Value) * SV_PACKETSZ;
  while (cPos < dsz){

    _buffer[buffWr].name = indata.data() + cPos;
    _buffer[buffWr].module = indata.c_str();
    _buffer[buffWr].type = SV_Base::ValueType(*(indata.data() + cPos + SV_NAMESZ));
    _buffer[buffWr].data.beginTime = bTm;

    memcpy(_buffer[buffWr].data.vals, indata.data() + cPos + SV_NAMESZ + sizeof(ValueType), vlsz);

    _buffer[buffWr].isActive = true;

    ++buffWr;
    if (buffWr == BUFF_SZ) buffWr = 0;

    cPos += valSz;
  }
}

void BufferData::incReadPos(){

  _buffer[_buffReadPos].isActive = false;
  ++_buffReadPos;

  if (_buffReadPos == BUFF_SZ) _buffReadPos = 0;
}

BufferData::InputData BufferData::getDataByReadPos(){

  return _buffer[_buffReadPos];
}


