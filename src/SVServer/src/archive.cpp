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
#include <chrono>
#include <cstdio>
#include <fstream>
#include "archive.h"
#include "SVAuxFunc/aux_func.h"
#include "SVConfig/config_data.h"
#include "SVConfig/config_limits.h"
#include "Lib/zlib/zlib.h"

using namespace std;
using namespace SV_Base;
using namespace SV_Aux;

extern SV_Srv::statusCBack pfStatusCBack;

Archive::Archive(const SV_Srv::Config& _cng):
  cng(_cng){
  
  _copyStartTime = currDateTimeEx();
  _copyDateMem = currDateS();

  _cpySz = ARCH_CYCLE_MS / SV_CYCLESAVE_MS; // 10мин
}

void Archive::setConfig(const SV_Srv::Config& cng_){

  cng.outArchivePath = cng_.outArchivePath;
  cng.outArchiveName = cng_.outArchiveName;
  cng.outArchiveHourCnt = cng_.outArchiveHourCnt;
}

bool Archive::isCopyTimeHour(){

  time_t t = time(nullptr);
  tm* lct = localtime(&t);

  if (_front.PosFront(lct->tm_min == 0, 0)) ++_crtFileHour;

  bool req = false;

  bool isCheck = cng.outArchiveHourCnt % 2 == 0;    // кратно 2м часам?
  bool isHourCheck = lct->tm_hour % 2 == 0;
  bool isNDay = _front.PosFront(lct->tm_hour == 0, 1);
  if (((_crtFileHour >= cng.outArchiveHourCnt) || isNDay) && (!isCheck || isHourCheck)){
    _crtFileHour = 0;
    req = true;
  }

  return req;
}

string Archive::getOutPath(bool isStop){

  string cDate = currDateS();
  if (cDate != _copyDateMem){
    cDate = _copyDateMem;
    _copyDateMem = currDateS();
  }

  string path = cng.outArchivePath + cDate + "/";

  createSubDirectory(path);

  int utcOffs = hourOffsFromUTC();

  string fName = cng.outArchiveName + "_temp" + "UTC" + to_string(utcOffs) + ".dat";

  if (isCopyTimeHour() || isStop){

    string templ = path + fName;
    fName = cng.outArchiveName + "_" + _copyStartTime + "UTC" + to_string(utcOffs) + ".dat";
    rename(templ.c_str(), (path + fName).c_str());

    _copyStartTime = currDateTimeEx().c_str();
  }

  return path + fName;
}

bool Archive::compressData(size_t insz, const vector<char>& inArr, size_t& outsz, vector<char>& outArr){

  try{

    uLong compressed_size = compressBound(uLong(insz));

    if (outArr.size() < compressed_size)
      outArr.resize(compressed_size);

    int res = compress((Bytef*)outArr.data(), &compressed_size, (Bytef*)inArr.data(), uLong(insz));

    outsz = compressed_size;

    return res == Z_OK;

  }catch (exception e){
    if (pfStatusCBack) pfStatusCBack("Archive::compressData exception " + string(e.what()));
    return false;
  }
}

bool Archive::copyToDisk(bool isStop){

  try{

    size_t dsz = _archiveData.size();
    if (dsz == 0)
      return true;

    size_t SMAXCNT = 100; // макс кол-во сигналов в посылке

    size_t sint = sizeof(int),
      tmSz = sizeof(uint64_t),
      vlSz = sizeof(Value) * SV_PACKETSZ;

    //                name        module      group       comment      type    vCnt
    size_t headSz = SV_NAMESZ + SV_NAMESZ + SV_NAMESZ + SV_COMMENTSZ + sint + sint;

    vector<char> inArr((tmSz + vlSz) * _cpySz * SMAXCNT + headSz * SMAXCNT),
      compArr;

    vector<string> keys; keys.reserve(dsz);
    for (auto &it : _archiveData) keys.push_back(it.first);

    fstream file(getOutPath(isStop), std::fstream::binary | std::fstream::app);

    if (!file.good())
      return false;

    size_t sCnt = 0, csize = 0;
    for (size_t i = 0; i < dsz; ++i) {

      auto sign = SV_Srv::getSignalData(keys[i]);

      string sn = sign->name + sign->module;

      char* pIn = inArr.data();

      int vCnt = _valPos[sn];
      if (vCnt > 0) {
        memcpy(pIn + csize, sign->name.c_str(), SV_NAMESZ);       csize += SV_NAMESZ;
        memcpy(pIn + csize, sign->module.c_str(), SV_NAMESZ);     csize += SV_NAMESZ;
        memcpy(pIn + csize, sign->group.c_str(), SV_NAMESZ);      csize += SV_NAMESZ;
        memcpy(pIn + csize, sign->comment.c_str(), SV_COMMENTSZ); csize += SV_COMMENTSZ;
        memcpy(pIn + csize, &sign->type, sint);                   csize += sint;
        memcpy(pIn + csize, &vCnt, sint);                         csize += sint;

        for (int j = 0; j < vCnt; ++j) {
          memcpy(pIn + csize, &_archiveData[sn][j].beginTime, tmSz);  csize += tmSz;
          memcpy(pIn + csize, _archiveData[sn][j].vals, vlSz);       csize += vlSz;
        }

        _valPos[sn] = 0;

        ++sCnt;
      }

      if ((sCnt > 0) && ((sCnt >= SMAXCNT) || (i == (dsz - 1)))) {
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
    return true;

  } catch (exception e){
    if (pfStatusCBack) pfStatusCBack("Archive::copyToDisk exception " + string(e.what()));
    return false;
  }
}

void Archive::addSignal(const string& sign){

  if (_archiveData.find(sign) != _archiveData.end()) return;

  _archiveData[sign] = vector<RecData>(_cpySz);

  _valPos[sign] = 0;

  Value* buff = new Value[SV_PACKETSZ * _cpySz];
  memset(buff, 0, SV_PACKETSZ * _cpySz * sizeof(Value));
  for (int i = 0; i < _cpySz; ++i)
    _archiveData[sign][i].vals = &buff[i * SV_PACKETSZ];

}

void Archive::addValue(const string& sign, const RecData& rd){

  int vp = _valPos[sign];

  _archiveData[sign][vp].beginTime = rd.beginTime;
  memcpy(_archiveData[sign][vp].vals, rd.vals, SV_PACKETSZ * sizeof(Value));

  ++_valPos[sign];

  if (_valPos[sign] == _cpySz){

    copyToDisk(false);
  }
}
