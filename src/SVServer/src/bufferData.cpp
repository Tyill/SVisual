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
#include <mutex>

#include "stdafx.h"
#include "SVConfig/SVConfigLimits.h"
#include "SVConfig/SVConfigData.h"
#include "bufferData.h"

using namespace std;
using namespace SV_Cng;

bufferData::bufferData(bufferData::config cng_){

    cng = cng_;

	value* buff = new value[SV_PACKETSZ * buffSz_];
    memset(buff, 0, SV_PACKETSZ * buffSz_ * sizeof(value));
	for (int i = 0; i < buffSz_; ++i)
		buffer_[i].data.vals = &buff[i * SV_PACKETSZ];
}

void bufferData::updDataSignals(const std::string& in, uint64_t bTm){

	size_t sz = in.size(), 
	       clSz = SV_NAMESZ + sizeof(valueType) + sizeof(value) * SV_PACKETSZ,
		   cPos = SV_NAMESZ;
	
	string module = in.c_str();

    int valCnt = std::max(size_t(0), std::min((sz - cPos) / clSz, size_t(SV_VALUE_MAX_CNT * 10))); // 10 - запас

	mtx_.lock();

	int buffWr = buffWritePos_;
	buffWritePos_ += valCnt; 
    
    if (buffWritePos_ >= buffSz_) buffWritePos_ -= buffSz_;

	mtx_.unlock();

	size_t vlsz = sizeof(value) * SV_PACKETSZ,
		   offs = SV_NAMESZ + sizeof(valueType);

	while (cPos < sz){

		valueData* vr = (valueData*)(in.data() + cPos);
				
		buffer_[buffWr].name = vr->name;
		buffer_[buffWr].module = module;
		buffer_[buffWr].type = vr->type;
		buffer_[buffWr].data.beginTime = bTm;
							
		memcpy(buffer_[buffWr].data.vals, in.data() + cPos + offs, vlsz);

		buffer_[buffWr].isActive = true;

		++buffWr; 
        if (buffWr >= buffSz_)
            buffWr = 0;

		cPos += clSz;
	}
}

void bufferData::incReadPos(){

	buffer_[buffReadPos_].isActive = false;
	++buffReadPos_; 
    
    if (buffReadPos_ >= buffSz_) buffReadPos_ = 0;
}

int bufferData::getBuffSize(){

	return buffSz_;
}

bufferData::inputData bufferData::getDataByReadPos(){

	return buffer_[buffReadPos_];
}


