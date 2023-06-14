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
#include "archive.h"
#include "clickhouse_db.h"
#include "SVMisc/misc.h"
#include "SVBase/sv_limits.h"
#include "zlib/zlib.h"

#include <chrono>
#include <cstdio>
#include <fstream>
#include <cstring>

using namespace std;

extern SV_Srv::statusCBack pfStatusCBack;

void Archive::init(const SV_Srv::Config& cng_) {

  cng = cng_;
  _copyStartTime = SV_Misc::currDateTimeEx();
  _copyDateMem = SV_Misc::currDateS();
  _copySz = 600000 / SV_CYCLESAVE_MS; // 10мин

  if(cng.outDataBaseEna && !cng.outDataBaseName.empty() && !cng.outDataBaseAddr.empty()){
      _chdb = new ClickHouseDB(cng);
  }
}

void Archive::setConfig(const SV_Srv::Config& cng_){
    
  cng.outArchivePath = cng_.outArchivePath;
  cng.outArchiveName = cng_.outArchiveName;
  cng.outArchiveHourCnt = cng_.outArchiveHourCnt;

  if (!_chdb && cng_.outDataBaseEna && !cng_.outDataBaseName.empty() && !cng_.outDataBaseAddr.empty()){
      _chdb = new ClickHouseDB(cng);
  }
}

void Archive::addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype) {

  std::string sign = sname + module;
  if (_archiveData.find(sign) != _archiveData.end()) return;

  _archiveData[sign] = vector<SV_Base::RecData>(_copySz);

  _valPos[sign] = 0;

  SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * _copySz];
  memset(buff, 0, SV_PACKETSZ * _copySz * sizeof(SV_Base::Value));
  for (size_t i = 0; i < _copySz; ++i){
    _archiveData[sign][i].vals = &buff[i * SV_PACKETSZ];
  }
  if (_chdb){
      _chdb->addSignal(sname, module, stype);
  }
}

void Archive::addValue(const string& sign, const SV_Base::RecData& rd) {

  int vp = _valPos[sign];

  _archiveData[sign][vp].beginTime = rd.beginTime;
  memcpy(_archiveData[sign][vp].vals, rd.vals, SV_PACKETSZ * sizeof(SV_Base::Value));

  ++_valPos[sign];

  if (_valPos[sign] == _copySz) {

    copyToDisk(false);
  }
}

bool Archive::copyToDisk(bool isStop){

    const size_t dataSz = _archiveData.size();
    if (dataSz == 0){
      return true;
    }

    if (cng.outArchiveEna){
        const size_t SMAXCNT = 100; // макс кол-во сигналов в посылке

        const size_t intSz = sizeof(int32_t),
          tmSz = sizeof(uint64_t),
          vlSz = sizeof(SV_Base::Value) * SV_PACKETSZ;

        //                     name        module      group       comment      type    vCnt
        const size_t headSz = SV_NAMESZ + SV_NAMESZ + SV_NAMESZ + SV_COMMENTSZ + intSz + intSz;

        vector<char> inArr((tmSz + vlSz) * _copySz * SMAXCNT + headSz * SMAXCNT);
        vector<char> compArr;

        vector<string> keys;
        keys.reserve(dataSz);
        for (auto &it : _archiveData){
            keys.push_back(it.first);
        }

        fstream file(getOutPath(isStop), std::fstream::binary | std::fstream::app);
        if (!file.good()){
            return false;
        }

        size_t sCnt = 0, csize = 0;
        for (size_t i = 0; i < dataSz; ++i) {

          auto sign = SV_Srv::getSignalData(keys[i]);

          string sn = sign->name + sign->module;

          char* pIn = inArr.data();

          int vCnt = _valPos[sn];
          if (vCnt > 0) {
            memcpy(pIn + csize, sign->name.c_str(), SV_NAMESZ);       csize += SV_NAMESZ;
            memcpy(pIn + csize, sign->module.c_str(), SV_NAMESZ);     csize += SV_NAMESZ;
            memcpy(pIn + csize, sign->group.c_str(), SV_NAMESZ);      csize += SV_NAMESZ;
            memcpy(pIn + csize, sign->comment.c_str(), SV_COMMENTSZ); csize += SV_COMMENTSZ;
            memcpy(pIn + csize, &sign->type, intSz);                  csize += intSz;
            memcpy(pIn + csize, &vCnt, intSz);                        csize += intSz;

            for (int j = 0; j < vCnt; ++j) {
              memcpy(pIn + csize, &_archiveData[sn][j].beginTime, tmSz); csize += tmSz;
              memcpy(pIn + csize, _archiveData[sn][j].vals, vlSz);       csize += vlSz;
            }

            ++sCnt;
          }

          if ((sCnt > 0) && ((sCnt >= SMAXCNT) || (i == (dataSz - 1)))) {
            sCnt = 0;

            size_t compSz = 0;

            if (!compressData(csize, inArr, compSz, compArr)) {
              if (pfStatusCBack) pfStatusCBack("Archive::copyToDisk compressData error");
              file.close();
              return false;
            };

            file.write((char *)&compSz, sizeof(int));
            file.write((char *)&csize, sizeof(int));
            file.write((char *)compArr.data(), compSz);

            csize = 0;
          }
        }
        file.close();
    }

    if (_chdb && cng.outDataBaseEna){
      _chdb->saveSData(isStop, _valPos, _archiveData);
    }

    for(auto& v : _valPos){
      v.second = 0;
    }

    return true;
}

bool Archive::compressData(size_t inSz, const vector<char>& inArr, size_t& outsz, vector<char>& outArr) {

  try {

    uLong compressedSz = compressBound(uLong(inSz));

    if (outArr.size() < compressedSz)
      outArr.resize(compressedSz);

    int res = compress((Bytef*)outArr.data(), &compressedSz, (Bytef*)inArr.data(), uLong(inSz));

    outsz = compressedSz;

    return res == Z_OK;

  }
  catch (exception e) {
    if (pfStatusCBack) pfStatusCBack("Archive::compressData exception " + string(e.what()));
    return false;
  }
}

string Archive::getOutPath(bool isStop) {

  string cDate = SV_Misc::currDateS();
  if (cDate != _copyDateMem) {
    cDate = _copyDateMem;
    _copyDateMem = SV_Misc::currDateS();
  }

  string path = cng.outArchivePath + cDate + "/";

  SV_Misc::createSubDirectory(path);

  int utcOffs = SV_Misc::hourOffsFromUTC();

  string fName = cng.outArchiveName + "_temp" + "UTC" + to_string(utcOffs) + ".dat";

  if (isCopyTimeHour() || isStop) {

    string templ = path + fName;
    fName = cng.outArchiveName + "_" + _copyStartTime + "UTC" + to_string(utcOffs) + ".dat";
    rename(templ.c_str(), (path + fName).c_str());

    _copyStartTime = SV_Misc::currDateTimeEx().c_str();
  }

  return path + fName;
}

bool Archive::isCopyTimeHour() {

  time_t t = time(nullptr);
  tm* lct = localtime(&t);

  if (_front.PosFront(lct->tm_min == 0, 0)) ++_crtFileHour;

  bool req = false;
  bool isCheck = cng.outArchiveHourCnt % 2 == 0;    // кратно 2м часам?
  bool isHourCheck = lct->tm_hour % 2 == 0;
  bool isNDay = _front.PosFront(lct->tm_hour == 0, 1);
  if (((int(_crtFileHour) >= cng.outArchiveHourCnt) || isNDay) && (!isCheck || isHourCheck)) {
    _crtFileHour = 0;
    req = true;
  }

  return req;
}

