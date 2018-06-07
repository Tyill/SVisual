
#include <chrono>
#include <cstdio>
#include <fstream>
#include "stdafx.h"
#include "archive.h"
#include "server.h"
#include "sharedFunc.h"
#include "SVAuxFunc/auxFunc.h"
#include "SVConfig/SVConfigData.h"
#include "SVConfig/SVConfigLimits.h"
#include "Lib/zlib/zlib.h"

using namespace std;
using namespace SV_Cng;
using namespace SV_Aux;

void archive::statusMess(const std::string& mess){

	if (pServ_->pfStatusCBack) pServ_->pfStatusCBack(CurrDateTime() + " " + mess);
}

archive::archive(config cng_, server* serv){

	pServ_ = serv;
    cng = cng_;

    copyStartTime_ = CurrDateTimeEx();
    copyDateMem_ = CurrDateS();

    cpySz_ = archCycleMs / SV_CYCLESAVE_MS; // 10мин
}

void archive::setConfig(archive::config cng_){

	cng.outArchivePath = cng_.outArchivePath;
	cng.outArchiveName = cng_.outArchiveName;
	cng.outArchiveHourCnt = cng_.outArchiveHourCnt;
}

bool archive::isCopyTimeHour(){

    time_t t = time(nullptr);
	tm* lct = localtime(&t);

    if (front_.PosFront(lct->tm_min == 0, 0)) ++crtFileHour_;

	bool req = false;

	bool isCheck = cng.outArchiveHourCnt % 2 == 0;    // кратно 2м часам?
	bool isHourCheck = lct->tm_hour % 2 == 0; 
	bool isNDay = front_.PosFront(lct->tm_hour == 0, 1);
	if (((crtFileHour_ >= cng.outArchiveHourCnt) || isNDay) && (!isCheck || isHourCheck)){
		crtFileHour_ = 0;
		req = true;
	}

	return req;
}

string archive::getOutPath(bool isStop){

	string cDate = CurrDateS();
	if (cDate != copyDateMem_){
		cDate = copyDateMem_;
		copyDateMem_ = CurrDateS();
	}

	string path = cng.outArchivePath + cDate + "/";

	CreateSubDirectory(path);

   	int utcOffs = HourOffsFromUTC();

	string fName = cng.outArchiveName + "_temp" + "UTC" + to_string(utcOffs) + ".dat";

	if (isCopyTimeHour() || isStop){

		string templ = path + fName;
		fName = cng.outArchiveName + "_" + copyStartTime_ + "UTC" + to_string(utcOffs) + ".dat";
		rename(templ.c_str(), (path + fName).c_str());

		copyStartTime_ = CurrDateTimeEx().c_str();
	}

	return path + fName;
}

bool archive::compressData(uint8_t* arr, size_t sz, size_t& outsz, uint8_t** outArr){

    FUNC_BEGIN

    uLongf compress_buff_size = compressBound(sz);
    Bytef *compress_buff = new Bytef[compress_buff_size];

    uLongf compressed_size = compress_buff_size;
    int res = compress(compress_buff, &compressed_size, arr, sz);

    outsz = compressed_size;
    *outArr = compress_buff;

    return (res == Z_OK) && compress_buff;

    FUNC_END
}

bool archive::copyToDisk(bool isStop){

	FUNC_BEGIN

	size_t dsz = archiveData_.size();
	if (dsz == 0) return true;

	int SMAXCNT = 100; // посылка жестко

	int sint = sizeof(int), tmSz = sizeof(uint64_t), vlSz = sizeof(value) * SV_PACKETSZ;
		//       name        module      group       comment       type    vCnt
	int	headSz = SV_NAMESZ + SV_NAMESZ + SV_NAMESZ + SV_COMMENTSZ + sint + sint;
	
	uint8_t *inArr = new uint8_t[(tmSz + vlSz) * cpySz_ * SMAXCNT + headSz * SMAXCNT];

	vector<string> keys; keys.reserve(dsz);
	for (auto &it : archiveData_) keys.push_back(it.first);
	
	fstream file(getOutPath(isStop), std::fstream::binary | std::fstream::app);

	if (!file.good()){ delete[] inArr; return false;}

	size_t sCnt = 0, csize = 0;
	for (int i = 0; i < dsz; ++i) {

		auto sign = pServ_->getSignalData(keys[i]);

		string sn = sign->name + sign->module;
		int vCnt = valPos_[sn];
		if (vCnt > 0) {
			memcpy(inArr + csize, sign->name.c_str(), SV_NAMESZ);       csize += SV_NAMESZ;
			memcpy(inArr + csize, sign->module.c_str(), SV_NAMESZ);     csize += SV_NAMESZ;
			memcpy(inArr + csize, sign->group.c_str(), SV_NAMESZ);      csize += SV_NAMESZ;
			memcpy(inArr + csize, sign->comment.c_str(), SV_COMMENTSZ); csize += SV_COMMENTSZ;
			memcpy(inArr + csize, &sign->type, sint);                   csize += sint;
			memcpy(inArr + csize, &vCnt, sint);                         csize += sint;

			for (int j = 0; j < vCnt; ++j) {
				memcpy(inArr + csize, &archiveData_[sn][j].beginTime, tmSz);  csize += tmSz;
				memcpy(inArr + csize, archiveData_[sn][j].vals, vlSz);       csize += vlSz;
			}

			valPos_[sn] = 0;

			++sCnt;
		}

		if ((sCnt > 0) && ((sCnt >= SMAXCNT) || (i == (dsz - 1)))) {
			sCnt = 0;

			size_t outSz = 0;
			uint8_t *compArr = nullptr;
			if (!compressData(inArr, csize, outSz, &compArr)) {
				statusMess("archive::copyToDisk compressData error");
				delete[] inArr;
				file.close();
				return false;
			};

			file.write((char *) &outSz, sizeof(int));
			file.write((char *) &csize, sizeof(int));
			file.write((char *) compArr, outSz);

			delete[] compArr;
			csize = 0;
		}

	}
	delete[] inArr;

	file.close();

	FUNC_END
}

void archive::addSignal(const string& sign){

	if (archiveData_.find(sign) != archiveData_.end()) return;

    archiveData_[sign] = vector<recData>(cpySz_);

	valPos_[sign] = 0;

	value* buff = new value[SV_PACKETSZ * cpySz_];
	memset(buff, 0, SV_PACKETSZ * cpySz_ * sizeof(value));
	for (int i = 0; i < cpySz_; ++i)
        archiveData_[sign][i].vals = &buff[i * SV_PACKETSZ];

}

void archive::addValue(const string& sign, const recData& rd){

	int vp = valPos_[sign];

    archiveData_[sign][vp].beginTime = rd.beginTime;
	memcpy(archiveData_[sign][vp].vals, rd.vals, SV_PACKETSZ * sizeof(value));

	++valPos_[sign];
	
	if (valPos_[sign] >= cpySz_){
		valPos_[sign] = cpySz_ - 1;

		copyToDisk(false);
	}
}
