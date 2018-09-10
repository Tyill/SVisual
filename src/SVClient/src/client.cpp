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
#include "stdafx.h"
#include "client.h"
#include "SVAuxFunc/clientTCP.h"
#include "SVAuxFunc/auxFunc.h"
#include <thread>


using namespace std;

client::~client(){

	if (isConnect_) SV_TcpCln::disconnect();

	thrStop_ = true;
	if (thr_.joinable()) thr_.join();
}

void client::setConfig(config cng_){

	cng = cng_;
}

bool client::connect(const char* moduleName, const char* ipAddr, int port){

	std::unique_lock<std::mutex> lck(mtxConnect_);

	if (isConnect_) return true;

	if ((strlen(moduleName) == 0) || (strlen(moduleName) >= SV_NAMESZ)){
		return false;
	}

	if (strstr(moduleName, "=end=") || strstr(moduleName, "=begin=")){
		return false;
	}

	module_ = moduleName;
	addrServ_ = ipAddr;
	portServ_ = port;

	isConnect_ = SV_TcpCln::connect(ipAddr, port);
			
	if (isConnect_){
        thr_ = std::thread([](client *lp) { lp->sendCyc(); }, this);
	}

	return isConnect_;
}

void client::disconnect(){

	if (isConnect_) SV_TcpCln::disconnect();

	thrStop_ = true;

	if (thr_.joinable()) thr_.join();
}

bool client::addValue(const char* name, SV_Cng::valueType type, SV_Cng::value val, bool onlyPosFront){

	if (!isConnect_) return false;

	bool ok = true;
		
	if (values_.find(name) == values_.end()){

		if ((strlen(name) == 0) || (strlen(name) >= SV_NAMESZ)){
			return false;
		}

		if (strstr(name, "=end=") || strstr(name, "=begin=")){
			return false;
		}
		valueRec* vd = new valueRec();
		memset(vd, 0, sizeof(valueRec));
		strcpy(vd->name, name);
		vd->type = type;
		vd->isOnlyFront = onlyPosFront;
		mtxUpdValue_.lock();
		values_.insert(pair<string, valueRec*>(name, vd));
		values_[name]->vals = new SV_Cng::value[SV_PACKETSZ];
		memset(values_[name]->vals, 0, sizeof(SV_Cng::value) * SV_PACKETSZ);
		mtxUpdValue_.unlock();
	}
	
	valueRec* vr = values_[name];
	
	if (!isWrite_){

		vr->vals[curCycCnt_] = val;
		vr->isActive = true;
	}
	else {
		mtxUpdValue_.lock();

		vr->vals[curCycCnt_] = val;
		vr->isActive = true;

		mtxUpdValue_.unlock();
	}
	
	return ok;
}

bool client::sendData(){
		
	if (values_.empty()) return true;

	int SINT = sizeof(int), vlSz = SV_NAMESZ + SINT + SINT * SV_PACKETSZ;

	int dataSz = SV_NAMESZ + vlSz * values_.size();

	int startSz = 7, endSz = 5;
	int arrSz = startSz + SINT + dataSz + endSz;
	char* arr = new char[arrSz];
    memset(arr, 0, arrSz);

	int offs = 0;
	memcpy(arr, "=begin=", startSz); offs += startSz;

	memcpy(arr + offs, &dataSz, SINT); offs += SINT;

	memcpy(arr + offs, module_.data(), SV_NAMESZ); offs += SV_NAMESZ;
		
	int cnt = 0;
	for (auto& it : values_){
				
		memcpy(arr + offs + vlSz*cnt, it.second->name, SV_NAMESZ);
		memcpy(arr + offs + vlSz*cnt + SV_NAMESZ, &it.second->type, SINT);
		memcpy(arr + offs + vlSz*cnt + SV_NAMESZ + SINT, it.second->vals, SV_PACKETSZ * SINT);

		++cnt;
	}
		
	memcpy(arr + offs + vlSz*cnt, "=end=", endSz);

    string out;
	bool ok = SV_TcpCln::sendData(string(arr, arrSz), out, false, true);

	delete[] arr;

	return ok;

}

void client::sendCyc(){


	uint64_t cTm = SV_Aux::CurrDateTimeSinceEpochMs(), prevTm = cTm, tmDiff = SV_CYCLEREC_MS;
		
	while (!thrStop_ ){
		
		if (!isConnect_)
			isConnect_ = SV_TcpCln::connect(addrServ_, portServ_);

		cTm = SV_Aux::CurrDateTimeSinceEpochMs();
		tmDiff = cTm - prevTm - (SV_CYCLEREC_MS - tmDiff); prevTm = cTm;

		mtxUpdValue_.lock();
		isWrite_ = true;

		int prevCyc = curCycCnt_ - 1; if (prevCyc < 0) prevCyc = SV_PACKETSZ - 1;
		for (auto it = values_.begin(); it != values_.end(); ++it){
						
			if (!it->second->isActive){
				it->second->vals[curCycCnt_] = it->second->vals[prevCyc];
								
				if ((it->second->type == SV_Cng::valueType::tBool) && it->second->isOnlyFront)
					it->second->vals[curCycCnt_].tBool = false;
			}
					
			it->second->isActive = false;
		}
		
		int next = curCycCnt_ + 1;
		
		if (next >= SV_PACKETSZ) {

			isConnect_ = (isConnect_) ? sendData() : false;

			curCycCnt_ = 0;
		}
		else ++curCycCnt_;

		isWrite_ = false;
		mtxUpdValue_.unlock();

		if ((SV_CYCLEREC_MS - tmDiff) > 0)
			SV_Aux::SleepMs(SV_CYCLEREC_MS - tmDiff);
	}
}




