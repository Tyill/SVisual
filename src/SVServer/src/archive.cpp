//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
#include "archive.h"
#include "clickhouse_db.h"
#include "SVMisc/misc.h"
#include "SVBase/sv_limits.h"
#include "SVServer/sv_server.h"
#include "zlib/zlib.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <thread>

using namespace std;

void statusMessage(const std::string& mess);

namespace {
  void writeFixedField(char* dst, size_t fieldSz, const std::string& s) {
    memset(dst, 0, fieldSz);
    if (!s.empty()) {
      memcpy(dst, s.c_str(), std::min(s.size(), fieldSz - 1));
    }
  }
}

Archive::~Archive() {
  joinSaveThread();
#ifdef USE_ClickHouseDB
  delete m_chdb;
  m_chdb = nullptr;
#endif
}

void Archive::joinSaveThread() {
  if (m_saveThread && m_saveThread->joinable()) {
    m_saveThread->join();
  }
}

void Archive::init(const SV_Srv::Config& cng_) {

  cng = cng_;
  cng.outArchiveHourCnt = std::max(1, cng.outArchiveHourCnt);
  m_copyStartTime = SV_Misc::currDateTimeEx();
  m_copyDateMem = SV_Misc::currDateS();
  m_copySz = std::max(size_t(10), size_t(600000 / SV_CYCLESAVE_MS));

#ifdef USE_ClickHouseDB
  if(cng.outDataBaseEna && !cng.outDataBaseName.empty() && !cng.outDataBaseAddr.empty()){
      m_chdb = new ClickHouseDB(cng);
  }
#endif
}

void Archive::addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype) {

  for (int aIndex = 0; aIndex < 2; ++aIndex){
    std::string sign = sname + module;
    auto& archiveData = m_archiveData[aIndex];
    if (archiveData.find(sign) != archiveData.end()) return;

    archiveData[sign] = vector<SV_Base::RecData>(m_copySz);

    auto& vpos = m_valPos[aIndex];
    vpos[sign] = 0;

    auto storage = std::make_unique<SV_Base::Value[]>(SV_PACKETSZ * m_copySz);
    memset(storage.get(), 0, SV_PACKETSZ * m_copySz * sizeof(SV_Base::Value));
    SV_Base::Value* buff = storage.get();
    for (size_t i = 0; i < m_copySz; ++i){
      archiveData[sign][i].vals = &buff[i * SV_PACKETSZ];
    }
    m_archiveValueStorage[aIndex][sign] = std::move(storage);
#ifdef USE_ClickHouseDB
    if (m_chdb){
        m_chdb->addSignal(sname, module, stype);
    }
#endif
  }
}

void Archive::addValue(const string& sign, const SV_Base::RecData& rd) {

  auto& valPos = m_valPos[m_archiveIndex];
  int vp = valPos[sign];
  auto& archiveData = m_archiveData[m_archiveIndex];
  archiveData[sign][vp].beginTime = rd.beginTime;
  memcpy(archiveData[sign][vp].vals, rd.vals, SV_PACKETSZ * sizeof(SV_Base::Value));

  ++valPos[sign];

  if (valPos[sign] == static_cast<int>(m_copySz)) {   
    copyToDisk(false);
  }
}

void Archive::copyToDisk(bool isStop){
  joinSaveThread();
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
      size_t SMAXCNT = 100;
      if (SV_PACKETSZ > 100000){
          SMAXCNT = 1;
      }else if (SV_PACKETSZ > 10000){
          SMAXCNT = 10;
      }

      const size_t intSz = sizeof(int32_t),
        tmSz = sizeof(uint64_t),
        vlSz = sizeof(SV_Base::Value) * SV_PACKETSZ;

      const size_t headSz = SV_NAMESZ + SV_NAMESZ + SV_NAMESZ + SV_COMMENTSZ + intSz + intSz;

      vector<char> inArr((tmSz + vlSz) * m_copySz * SMAXCNT + headSz * SMAXCNT);
      vector<char> compArr;

      const auto fpath = getOutPath(isStop);
      fstream file(fpath, std::fstream::binary | std::fstream::app);
      if (!file.good()){
        statusMessage("Archive::copyToDisk file not open for write, fpath " + fpath);
        return;
      }
      if (file.tellp() == 0){
        file << "{\"packetSz\":" << SV_PACKETSZ 
             << ",\"cycleRecMs\":" << SV_CYCLEREC_MS << "}\n";
      }

      size_t sCnt = 0, csize = 0, ix = 0;
      for (const auto& ad : archiveData) {

        const auto sign = SV_Srv::getSignalData(ad.first);
        if (!sign) {
          ++ix;
          continue;
        }

        char* pIn = inArr.data();       
        
        int vCnt = valPos[ad.first];
        if (vCnt > 0) {
          writeFixedField(pIn + csize, SV_NAMESZ, sign->name);       csize += SV_NAMESZ;
          writeFixedField(pIn + csize, SV_NAMESZ, sign->module);     csize += SV_NAMESZ;
          writeFixedField(pIn + csize, SV_NAMESZ, sign->group);      csize += SV_NAMESZ;
          writeFixedField(pIn + csize, SV_COMMENTSZ, sign->comment); csize += SV_COMMENTSZ;
          memcpy(pIn + csize, &sign->type, intSz);                   csize += intSz;
          memcpy(pIn + csize, &vCnt, intSz);                         csize += intSz;

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
#ifdef USE_ClickHouseDB
  if (m_chdb && cng.outDataBaseEna){
    m_chdb->saveSData(isStop, valPos, archiveData);
  }
#endif

  for(auto& v : valPos){
    v.second = 0;
  }
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
  catch (const exception& e) {
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
  tm lctBuf{};
#ifdef _WIN32
  localtime_s(&lctBuf, &t);
  tm* lct = &lctBuf;
#else
  tm* lct = localtime_r(&t, &lctBuf);
  if (!lct) return false;
#endif

  if (m_front.PosFront(lct->tm_min == 0, 0)) ++m_crtFileHour;

  bool req = false;
  bool isCheck = cng.outArchiveHourCnt % 2 == 0;
  bool isHourCheck = lct->tm_hour % 2 == 0;
  bool isNDay = m_front.PosFront(lct->tm_hour == 0, 1);
  if (((int(m_crtFileHour) >= cng.outArchiveHourCnt) || isNDay) && (!isCheck || isHourCheck)) {
    m_crtFileHour = 0;
    req = true;
  }

  return req;
}
