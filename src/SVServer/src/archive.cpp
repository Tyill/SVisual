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
#include <thread>

using namespace std;

void statusMessage(const std::string& mess);

void Archive::init(const SV_Srv::Config& cng_) {

  cng = cng_;
  m_copyStartTime = SV_Misc::currDateTimeEx();
  m_copyDateMem = SV_Misc::currDateS();
  m_copySz = std::max(10, 600000 / SV_CYCLESAVE_MS); // 10мин

  if(cng.outDataBaseEna && !cng.outDataBaseName.empty() && !cng.outDataBaseAddr.empty()){
      m_chdb = new ClickHouseDB(cng);
  }
}

void Archive::setConfig(const SV_Srv::Config& cng_){
    
  cng.outArchivePath = cng_.outArchivePath;
  cng.outArchiveName = cng_.outArchiveName;
  cng.outArchiveHourCnt = cng_.outArchiveHourCnt;

  if (!m_chdb && cng_.outDataBaseEna && !cng_.outDataBaseName.empty() && !cng_.outDataBaseAddr.empty()){
      m_chdb = new ClickHouseDB(cng);
  }
}

void Archive::addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype) {

  for (int aIndex = 0; aIndex < 2; ++aIndex){
    std::string sign = sname + module;
    auto& archiveData = m_archiveData[aIndex];
    if (archiveData.find(sign) != archiveData.end()) return;

    archiveData[sign] = vector<SV_Base::RecData>(m_copySz);

    auto& vpos = m_valPos[aIndex];
    vpos[sign] = 0;

    SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * m_copySz];
    memset(buff, 0, SV_PACKETSZ * m_copySz * sizeof(SV_Base::Value));
    for (size_t i = 0; i < m_copySz; ++i){
      archiveData[sign][i].vals = &buff[i * SV_PACKETSZ];
    }
    if (m_chdb){
        m_chdb->addSignal(sname, module, stype);
    }
  }
}

void Archive::addValue(const string& sign, const SV_Base::RecData& rd) {

  auto& valPos = m_valPos[m_archiveIndex];
  int vp = valPos[sign];
  auto& archiveData = m_archiveData[m_archiveIndex];
  archiveData[sign][vp].beginTime = rd.beginTime;
  memcpy(archiveData[sign][vp].vals, rd.vals, SV_PACKETSZ * sizeof(SV_Base::Value));

  ++valPos[sign];

  if (valPos[sign] == m_copySz) {   
    copyToDisk(false);
  }
}

void Archive::copyToDisk(bool isStop){
  if (m_saveThread && m_saveThread->joinable()){
    m_saveThread->join();
  }
  if (!isStop){    
    m_saveThread = std::make_shared<std::thread>([this, aIndex = m_archiveIndex]{
      copyToDiskImpl(false, aIndex);
    });
    m_archiveIndex = m_archiveIndex ? 0 : 1; 
  }else{
    copyToDiskImpl(isStop, m_archiveIndex);
  }
}

void Archive::copyToDiskImpl(bool isStop, int archiveIndex){

  auto& valPos = m_valPos[archiveIndex];
  auto& archiveData = m_archiveData[archiveIndex];
  const size_t dataSz = archiveData.size();
  if (dataSz == 0){
    return;
  }

  if (cng.outArchiveEna){
      size_t SMAXCNT = 100; // макс кол-во сигналов в посылке
      if (SV_PACKETSZ > 100000){
          SMAXCNT = 1;
      }else if (SV_PACKETSZ > 10000){
          SMAXCNT = 10;
      }

      const size_t intSz = sizeof(int32_t),
        tmSz = sizeof(uint64_t),
        vlSz = sizeof(SV_Base::Value) * SV_PACKETSZ;

      //                     name        module      group       comment      type    vCnt
      const size_t headSz = SV_NAMESZ + SV_NAMESZ + SV_NAMESZ + SV_COMMENTSZ + intSz + intSz;

      vector<char> inArr((tmSz + vlSz) * m_copySz * SMAXCNT + headSz * SMAXCNT);
      vector<char> compArr;

      const auto fpath = getOutPath(isStop);
      fstream file(fpath, std::fstream::binary | std::fstream::app);
      if (!file.good()){
          statusMessage("Archive::copyToDisk file not open for write, fpath " + fpath);
          return;
      }

      size_t sCnt = 0, csize = 0, ix = 0;
      for (const auto& ad : archiveData) {

        const auto sign = SV_Srv::getSignalData(ad.first);

        char* pIn = inArr.data();       
        
        int vCnt = valPos[ad.first];
        if (vCnt > 0) {
          memcpy(pIn + csize, sign->name.c_str(), SV_NAMESZ);       csize += SV_NAMESZ;
          memcpy(pIn + csize, sign->module.c_str(), SV_NAMESZ);     csize += SV_NAMESZ;
          memcpy(pIn + csize, sign->group.c_str(), SV_NAMESZ);      csize += SV_NAMESZ;
          memcpy(pIn + csize, sign->comment.c_str(), SV_COMMENTSZ); csize += SV_COMMENTSZ;
          memcpy(pIn + csize, &sign->type, intSz);                  csize += intSz;
          memcpy(pIn + csize, &vCnt, intSz);                        csize += intSz;

          for (int j = 0; j < vCnt; ++j) {
            memcpy(pIn + csize, &ad.second[j].beginTime, tmSz); csize += tmSz;
            memcpy(pIn + csize, ad.second[j].vals, vlSz);       csize += vlSz;
          }
          ++sCnt;
        }
        if (sCnt > 0 && (sCnt == SMAXCNT || ix == dataSz - 1)) {
          sCnt = 0;
          size_t compSz = 0;
          if (!compressData(csize, inArr, compSz, compArr)) {
            statusMessage("Archive::copyToDisk compressData error");
            file.close();
            return;
          };

          file.write((char *)&compSz, sizeof(int));
          file.write((char *)&csize, sizeof(int));
          file.write((char *)compArr.data(), compSz);

          csize = 0;
        }
        ++ix;
      }
      file.close();
  }

  if (m_chdb && cng.outDataBaseEna){
    m_chdb->saveSData(isStop, valPos, archiveData);
  }

  for(auto& v : valPos){
    v.second = 0;
  }

  return;
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
    statusMessage("Archive::compressData exception " + string(e.what()));
    return false;
  }
}

string Archive::getOutPath(bool isStop) {

  string cDate = SV_Misc::currDateS();
  if (cDate != m_copyDateMem) {
    cDate = m_copyDateMem;
    m_copyDateMem = SV_Misc::currDateS();
  }

  string path = cng.outArchivePath + cDate + "/";

  SV_Misc::createSubDirectory(path);

  int utcOffs = SV_Misc::hourOffsFromUTC();

  string fName = cng.outArchiveName + "_temp" + "UTC" + to_string(utcOffs) + ".dat";

  if (isCopyTimeHour() || isStop) {

    string templ = path + fName;
    fName = cng.outArchiveName + "_" + m_copyStartTime + "UTC" + to_string(utcOffs) + ".dat";
    rename(templ.c_str(), (path + fName).c_str());

    m_copyStartTime = SV_Misc::currDateTimeEx().c_str();
  }

  return path + fName;
}

bool Archive::isCopyTimeHour() {

  time_t t = time(nullptr);
  tm* lct = localtime(&t);

  if (m_front.PosFront(lct->tm_min == 0, 0)) ++m_crtFileHour;

  bool req = false;
  bool isCheck = cng.outArchiveHourCnt % 2 == 0;    // кратно 2м часам?
  bool isHourCheck = lct->tm_hour % 2 == 0;
  bool isNDay = m_front.PosFront(lct->tm_hour == 0, 1);
  if (((int(m_crtFileHour) >= cng.outArchiveHourCnt) || isNDay) && (!isCheck || isHourCheck)) {
    m_crtFileHour = 0;
    req = true;
  }

  return req;
}

