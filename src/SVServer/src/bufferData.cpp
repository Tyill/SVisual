
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

	size_t sz = in.size(); 
	int clSz = SV_NAMESZ + sizeof(valueType) + sizeof(value) * SV_PACKETSZ,
		cPos = SV_NAMESZ;
	
	string module = in.c_str();

	int valCnt = std::max(0, std::min(int(sz - cPos) / clSz, SV_VALUE_MAX_CNT));

	mtx_.lock();

	int buffWr = buffWritePos_;
	buffWritePos_ += valCnt; if (buffWritePos_ >= buffSz_) buffWritePos_ -= buffSz_;

	mtx_.unlock();

	int vlsz = sizeof(value) * SV_PACKETSZ,
		offs = SV_NAMESZ + sizeof(valueType);

	while (cPos < sz){

		valueData* vr = (valueData*)(in.data() + cPos);
				
		buffer_[buffWr].name = vr->name;
		buffer_[buffWr].module = module;
		buffer_[buffWr].type = vr->type;
		buffer_[buffWr].data.beginTime = bTm;
							
		memcpy(buffer_[buffWr].data.vals, in.data() + cPos + offs, vlsz);

		buffer_[buffWr].isActive = true;

		++buffWr; if (buffWr >= buffSz_) buffWr = 0;
		cPos += clSz;
	}
}

void bufferData::incReadPos(){

	buffer_[buffReadPos_].isActive = false;
	++buffReadPos_; if (buffReadPos_ >= buffSz_) buffReadPos_ = 0;
}

int bufferData::getBuffSize(){

	return buffSz_;
}

bufferData::inputData bufferData::getDataByReadPos(){

	return buffer_[buffReadPos_];
}


