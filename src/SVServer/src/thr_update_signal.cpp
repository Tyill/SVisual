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

#include "SVServer/server.h"
#include "SVAuxFunc/timer_delay.h"
#include "SVAuxFunc/aux_func.h"
#include "thr_update_signal.h"
#include "buffer_data.h"
#include "archive.h"

#include <algorithm>

using namespace SV_Base;
using namespace SV_Aux;
using namespace std;

extern SV_Srv::onModuleConnectCBack pfModuleConnectCBack;
extern SV_Srv::onModuleDisconnectCBack pfModuleDisconnectCBack;
extern SV_Srv::onUpdateSignalsCBack pfUpdateSignalsCBack;
extern SV_Srv::onAddSignalsCBack pfAddSignalsCBack;
extern const int BUFF_SIGN_HOUR_CNT;

ThrUpdateSignal::ThrUpdateSignal(const SV_Srv::Config& _cng, BufferData* pBuff):
  cng(_cng), _pBuffData(pBuff){
    
  _pArchive = new Archive(cng);
  _thr = std::thread(&ThrUpdateSignal::updCycle, this);
}

ThrUpdateSignal::~ThrUpdateSignal(){

  _thrStop = true;
  if (_thr.joinable()) _thr.join();
}

void ThrUpdateSignal::setArchiveConfig(SV_Srv::Config cng_){

  cng.outArchiveEna = cng_.outArchiveEna;

  _pArchive->setConfig(cng_);
}

void ThrUpdateSignal::addSignal(const string& sign, const BufferData::InputData& bp){

  SignalData* sd = new SignalData();

  sd->isActive = true;
  sd->isBuffEnable = false;
  sd->isDelete = false;

  sd->name = bp.name;
  sd->module = bp.module;
  sd->type = bp.type;

  sd->lastData.vals = new SV_Base::Value[cng.packetSz];
  sd->lastData.beginTime = SV_Aux::currDateTimeSinceEpochMs();
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

  _pArchive->addSignal(sign);
}

void ThrUpdateSignal::updateSign(SignalData* sign, int beginPos, int valuePos){

  sign->buffMinTime = sign->buffData[beginPos].beginTime;
  sign->buffMaxTime = sign->buffData[valuePos].beginTime + SV_CYCLESAVE_MS;

  double minValue = sign->buffMinValue, maxValue = sign->buffMaxValue;

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

}

void ThrUpdateSignal::modConnect(const string& module){

  if (pfModuleConnectCBack)
    pfModuleConnectCBack(module);
}

void ThrUpdateSignal::modDisconnect(const string& module){

  if (pfModuleDisconnectCBack)
    pfModuleDisconnectCBack(module);
}

void ThrUpdateSignal::updCycle(){

  auto sref = SV_Srv::getCopySignalRef();
  map<string, bool> signActive;
  for (auto& s : sref) {
    signActive[s.second->name + s.second->module] = true;
  }

  auto mref = SV_Srv::getCopyModuleRef();
  map<string, bool> moduleActive;
  for (auto& m : mref) {
    moduleActive[m.first] = true;
  }

  SV_Aux::TimerDelay tmDelay;
  tmDelay.update();

  int buffSz = BUFF_SIGN_HOUR_CNT * 3600000 / SV_CYCLESAVE_MS; // 2 часа жестко
  int packSz = SV_PACKETSZ * sizeof(Value);                    // размер пакета
  int checkConnectTout = 5 * SV_CYCLESAVE_MS / 1000;           // проверка связи, тоже жестко:(

  while (!_thrStop){

    tmDelay.update();

    bool isNewSign = false, isBuffActive = false; string sign;
    BufferData::InputData bp = _pBuffData->getDataByReadPos();
    while (bp.isActive){

      isBuffActive = true;

      sign = bp.name + bp.module;

      if (sref.find(sign) == sref.end()){
        addSignal(sign, bp);
        sref[sign] = SV_Srv::getSignalData(sign);
        mref[bp.module] = SV_Srv::getModuleData(bp.module);;
        isNewSign = true;
      }

      auto sd = sref[sign];

      signActive[sign] = true;
      moduleActive[sd->module] = true;

      sd->lastData.beginTime = bp.data.beginTime;
      memcpy(sd->lastData.vals, bp.data.vals, packSz);

      // заполняем буфер, если разрешено
      if (sd->isBuffEnable) {
        int vp = sd->buffValuePos;
        sd->buffData[vp].beginTime = bp.data.beginTime;
        memcpy(sd->buffData[vp].vals, bp.data.vals, packSz);

        updateSign(sd, sd->buffBeginPos, vp);

        ++vp;
        if (vp == buffSz) vp = 0;
        sd->buffValuePos = vp;

        if (vp == sd->buffBeginPos) {
          ++sd->buffBeginPos;
          if (sd->buffBeginPos >= buffSz) sd->buffBeginPos = 0;
        }
      }

      if (cng.outArchiveEna)
        _pArchive->addValue(sign, bp.data);

      _pBuffData->incReadPos();
      bp = _pBuffData->getDataByReadPos();
    }

    if (isBuffActive && pfUpdateSignalsCBack)
      pfUpdateSignalsCBack();

    if (isNewSign && pfAddSignalsCBack)
      pfAddSignalsCBack();

    // архив
    if (cng.outArchiveEna && tmDelay.hourOnc())
      _pArchive->copyToDisk(false);

    // проверка связи
    if (tmDelay.onDelaySec(true, checkConnectTout, 0)){
      tmDelay.onDelaySec(false, 0, 0);

      for (auto& s : signActive){
        sref[s.first]->isActive = s.second;
        s.second = false;
      }

      for (auto& m : moduleActive){

        if (m.first == "Virtual") continue;

        if (!mref[m.first]->isActive && m.second)
          modConnect(m.first);

        else if (mref[m.first]->isActive && !m.second)
          modDisconnect(m.first);

        mref[m.first]->isActive = m.second;
        m.second = false;
      }
    }

    int ms = SV_CYCLESAVE_MS - (int)tmDelay.getCTime();
    if (ms > 0)
      sleepMs(std::min(ms, 10000));
  }

  if (cng.outArchiveEna)
    _pArchive->copyToDisk(true);
}

