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

#include "SVServer/sv_server.h"
#include "SVMisc/timer_delay.h"
#include "SVMisc/misc.h"
#include "thread_update.h"
#include "buffer_data.h"
#include "archive.h"

#include <algorithm>
#include <cstring>
#include <shared_mutex>

using namespace SV_Base;
using namespace SV_Misc;
using namespace std;

extern SV_Srv::onModuleConnectCBack pfModuleConnectCBack;
extern SV_Srv::onModuleDisconnectCBack pfModuleDisconnectCBack;
extern SV_Srv::onUpdateSignalsCBack pfUpdateSignalsCBack;
extern SV_Srv::onAddSignalsCBack pfAddSignalsCBack;

namespace SV_Srv {
extern std::shared_mutex m_mtxRW;
}

ThreadUpdate::ThreadUpdate(const SV_Srv::Config& _cng, BufferData& buff):
  cng(_cng), m_buffData(buff){
    
  m_archive.init(cng);
  m_thr = std::thread(&ThreadUpdate::updateCycle, this);
}

ThreadUpdate::~ThreadUpdate(){

  m_thrStop = true;
  if (m_thr.joinable()) m_thr.join();  
}

void ThreadUpdate::setArchiveConfig(const SV_Srv::Config& cng_){

  cng.outArchiveEna = cng_.outArchiveEna;
  cng.outDataBaseEna = cng_.outDataBaseEna;

  m_archive.setConfig(cng_);
}

void ThreadUpdate::addSignal(const BufferData::InputData& bp){

  SignalData* sd = new SignalData();

  sd->isActive = true;
  sd->isBuffEnable = false;
  sd->isDelete = false;

  sd->name = bp.name;
  sd->module = bp.module;
  sd->type = bp.type;

  sd->lastData.vals = new SV_Base::Value[cng.packetSz];
  sd->lastData.beginTime = SV_Misc::currDateTimeSinceEpochMs();
  memset(sd->lastData.vals, 0, sizeof(SV_Base::Value) * cng.packetSz);

  sd->buffMinTime = sd->lastData.beginTime - 5000;
  sd->buffMaxTime = sd->lastData.beginTime + 5000;
  sd->buffMaxValue = 1;
  sd->buffMinValue = 0;

  auto md = SV_Srv::getModuleData(bp.module);
  if (!md){
    md = new SV_Base::ModuleData(bp.module);
    md->isActive = false;
    md->isDelete = false;
    md->isEnable = true;
    SV_Srv::addModule(md);
  }

  SV_Srv::addSignal(sd);

  m_archive.addSignal(bp.name, bp.module, bp.type);
}

void ThreadUpdate::updateSignals(map<string, vector<LastSData>>& sLast, std::map<std::string, SV_Base::SignalData*>& sref){

    std::lock_guard lock(SV_Srv::m_mtxRW);

    const size_t buffSz = 2 * 3600000 / SV_CYCLESAVE_MS; // 2 часа
    const size_t packSz = SV_PACKETSZ * sizeof(Value);

    for (auto& s : sLast){
       for (auto& slast : s.second){
           if (slast.isActive) {
               auto sdata = sref[s.first];

               sdata->lastData.beginTime = slast.beginTime;
               memcpy(sdata->lastData.vals, slast.vals.data(), packSz);
               if (sdata->isBuffEnable) {
                   size_t vp = sdata->buffValuePos;
                   sdata->buffData[vp].beginTime = slast.beginTime;
                   memcpy(sdata->buffData[vp].vals, slast.vals.data(), packSz);

                   updateSignalsBuff(sdata, sdata->buffBeginPos, vp);

                   ++vp;
                   if (vp == buffSz) vp = 0;
                   sdata->buffValuePos = vp;

                   if (vp == sdata->buffBeginPos) {
                       ++sdata->buffBeginPos;
                       if (sdata->buffBeginPos >= buffSz) sdata->buffBeginPos = 0;
                   }
               }
               slast.isActive = false;
           }
       }
       s.second.resize(1);
    }
}

void ThreadUpdate::updateSignalsBuff(SignalData* sign, size_t beginPos, size_t valuePos){

  double minValue = sign->buffMinValue,
         maxValue = sign->buffMaxValue;

  if (sign->type == ValueType::INT){
    Value* vl = sign->buffData[valuePos].vals;
    for (int i = 0; i < SV_PACKETSZ; ++i){
      if (vl[i].vInt > maxValue) maxValue = vl[i].vInt;
      if (vl[i].vInt < minValue) minValue = vl[i].vInt;
    }

  }
  else if (sign->type == ValueType::FLOAT){
    Value* vl = sign->buffData[valuePos].vals;
    for (int i = 0; i < SV_PACKETSZ; ++i){
      if (vl[i].vFloat > maxValue) maxValue = vl[i].vFloat;
      if (vl[i].vFloat < minValue) minValue = vl[i].vFloat;
    }
  }

  sign->buffMinValue = minValue;
  sign->buffMaxValue = maxValue;

  sign->buffMinTime = sign->buffData[beginPos].beginTime;
  sign->buffMaxTime = sign->buffData[valuePos].beginTime + SV_CYCLESAVE_MS;
}

void ThreadUpdate::moduleConnect(const string& module){

  if (pfModuleConnectCBack)
    pfModuleConnectCBack(module);
}

void ThreadUpdate::moduleDisconnect(const string& module){

  if (pfModuleDisconnectCBack)
    pfModuleDisconnectCBack(module);
}

void ThreadUpdate::updateCycle(){

  auto sref = SV_Srv::getCopySignalRef();
  map<string, vector<LastSData>> sLast;
  map<string, bool> signActive;
  for (auto& s : sref) {
      sLast[s.first] = vector<LastSData>{ LastSData(SV_PACKETSZ)};
      signActive[s.first] = true;
  }
  auto mref = SV_Srv::getCopyModuleRef();
  map<string, bool> moduleActive;
  for (auto& m : mref) {
    moduleActive[m.first] = true;
  }

  SV_Misc::TimerDelay tmDelay;
  tmDelay.update();

  size_t packSz = SV_PACKETSZ * sizeof(Value);                    // размер пакета
  int checkConnectTout = 5 * SV_CYCLESAVE_MS / 1000;           // проверка связи, тоже жестко

  while (!m_thrStop){

    tmDelay.update();

    bool isNewSign = false, isBuffActive = false;
    
    BufferData::InputData bufPos = m_buffData.getDataByReadPos();
    while (bufPos.isActive){
      isBuffActive = true;

      string sign = bufPos.name + bufPos.module;
      if (sLast.find(sign) == sLast.end()){
        addSignal(bufPos);
        sref[sign] = SV_Srv::getSignalData(sign);
        mref[bufPos.module] = SV_Srv::getModuleData(bufPos.module);
        sLast[sign] = vector<LastSData>{ LastSData(SV_PACKETSZ) };
        isNewSign = true;
      }
      if (sLast[sign].back().isActive) {
          sLast[sign].push_back(LastSData(SV_PACKETSZ));
      }
      auto& sdata = sLast[sign].back();

      sdata.isActive = true;
      signActive[sign] = true;
      moduleActive[bufPos.module] = true;

      sdata.beginTime = bufPos.data.beginTime;
      memcpy(sdata.vals.data(), bufPos.data.vals, packSz);

      if (cng.outArchiveEna || cng.outDataBaseEna){
          m_archive.addValue(sign, bufPos.data);
      }
      m_buffData.incReadPos();
      bufPos = m_buffData.getDataByReadPos();
    }

    updateSignals(sLast, sref);

    if (isBuffActive && pfUpdateSignalsCBack){
      pfUpdateSignalsCBack();
    }

    if (isNewSign && pfAddSignalsCBack){
      pfAddSignalsCBack();
    }

    // архив
    if (tmDelay.hourOnc() && (cng.outArchiveEna || cng.outDataBaseEna)){
      m_archive.copyToDisk(false);
    }

    // проверка связи
    if (tmDelay.onDelaySec(true, checkConnectTout, 0)){
        tmDelay.onDelaySec(false, 0, 0);

        std::lock_guard lock(SV_Srv::m_mtxRW);

        for (auto& s : signActive){
            sref[s.first]->isActive = s.second;
            s.second = false;
        }
        for (auto& m : moduleActive){
            if (m.first == "Virtual") continue;

            if (!mref[m.first]->isActive && m.second){
                moduleConnect(m.first);
            }
            else if (mref[m.first]->isActive && !m.second){
                moduleDisconnect(m.first);
            }
            mref[m.first]->isActive = m.second;
            m.second = false;
        }
    }

    int ms = SV_CYCLESAVE_MS - (int)tmDelay.getCTime();
    if (ms > 0){
      sleepMs(std::min(ms, 10000));
    }
  }

  if (cng.outArchiveEna || cng.outDataBaseEna){
    m_archive.copyToDisk(true);
  }
}

