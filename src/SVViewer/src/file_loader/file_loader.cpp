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

#include "file_loader.h"
#include "SVViewer/forms/main_win.h"
#include "zlib/zlib.h"

#include <QDateTime>
#include <QFile>

using namespace SV_Base;

FileLoader::FileLoader(QMainWindow* mainWin, QObject* parent):
    QObject(parent),
    m_mainWin(qobject_cast<MainWin*>(mainWin)){

}

bool FileLoader::preloadFiles(const QStringList& files) {

  m_mainWin->cng.selOpenDir = files[0].left(files[0].lastIndexOf('/'));

  bool ok = true;
  for (const auto& path : files){
    ok &= loadFile(path);
  }
  return ok;
}

bool FileLoader::loadFile(const QString& path) {

  if (m_mainWin->fileRef_.contains(path)) return true;

  auto cng = m_mainWin->cng;
  auto& moduleRef = m_mainWin->moduleRef_;
  auto& groupRef = m_mainWin->groupRef_;
  auto& signalRef = m_mainWin->signalRef_;
  auto& fileRef = m_mainWin->fileRef_;

  int utcOffs = path.lastIndexOf("UTC");

  if (utcOffs > 0) {
    int ssz = path.size();
    QString ss = path.left(ssz - 4);

    bool isNum = false;
    utcOffs = ss.midRef(utcOffs + 3).toInt(&isNum) * 3600000 - QDateTime::currentDateTime().offsetFromUtc() * 1000;

    if (!isNum) utcOffs = QDateTime::currentDateTime().offsetFromUtc() * 1000;
  }

  QFile file(path);
  if (!file.exists() || !file.open(QIODevice::ReadOnly)){
      return false;
  }
  fileRef.insert(path, new FileData(path, utcOffs));

  QDataStream dataStream(&file);

  int valSz = sizeof(uint64_t) + sizeof(Value) * SV_PACKETSZ,
    vheadSz = sizeof(ValueData),
    patchNum = 0;

  uLong comprSz = 0,
    dataSz = 0;

  QVector<Bytef> inArr, outArr;

  while (!dataStream.atEnd()) {

    dataStream.readRawData((char*)&comprSz, 4);
    dataStream.readRawData((char*)&dataSz, 4);

    inArr.resize(comprSz);
    outArr.resize(dataSz);

    dataStream.readRawData((char*)inArr.data(), comprSz);

    if (uncompress(outArr.data(), &dataSz, inArr.data(), comprSz) != 0) {
      file.close();
      return false;
    }

    int cPos = 0, itData = 0;
    while (itData < (int)dataSz) {

      ValueData* vr = (ValueData*)(outArr.data() + itData);

      QString sign = QString(vr->name) + vr->module;

      if (!fileRef[path]->signls.contains(sign)){
        fileRef[path]->signls.insert(sign, FileData::fsd{ false, 0 });
      }
      fileRef[path]->signls[sign].patchApos.append(QPair<int, int>(patchNum, cPos));
      fileRef[path]->signls[sign].vlsCnt += vr->vlCnt;

      if (signalRef.contains(sign)) {
        if (!signalRef[sign]->isActive) {
          signalRef[sign]->isActive = true;
          moduleRef[vr->module]->isActive = true;
          groupRef[signalRef[sign]->group.c_str()]->isActive = true;
          signalRef[sign]->type = vr->type;
        }
        if (signalRef[sign]->group.empty()){
          signalRef[sign]->group = vr->group;
        }
        if (signalRef[sign]->comment.empty()){
          signalRef[sign]->comment = vr->comment;
        }
        int szVl = vheadSz + vr->vlCnt * valSz;
        itData += szVl;
        cPos += szVl;
        continue;
      }

      if (!moduleRef.contains(vr->module)) {
        moduleRef.insert(vr->module, new ModuleData(vr->module));
        moduleRef[vr->module]->isActive = true;
      }
      moduleRef[vr->module]->signls.push_back(sign.toStdString());

      if (!groupRef.contains(vr->group)) {
        groupRef.insert(vr->group, new GroupData(vr->group));
        groupRef[vr->group]->isActive = true;
      }
      groupRef[vr->group]->signls.push_back(sign.toStdString());

      auto sd = new SignalData();
      sd->name = vr->name;
      sd->module = vr->module;
      sd->group = vr->group;
      sd->comment = vr->comment;
      sd->type = vr->type;
      sd->isActive = true;

      signalRef[sign] = sd;

      int szVl = vheadSz + vr->vlCnt * valSz;
      itData += szVl;
      cPos += szVl;
    }
    ++patchNum;
  }
  file.close();
  return true;
}

bool FileLoader::loadSignalData(const QString& sign) {

  QMap<QString, SignalData*> sref = m_mainWin->signalRef_;
  if (!sref.contains(sign)){
    return false;
  }
  auto cng = m_mainWin->cng;
  auto sdata = sref[sign];

  bool  ok = true, isNewFile = false;
  for (auto path : qAsConst(m_mainWin->fileRef_)) {

    if (!path->signls.contains(sign) || path->signls[sign].isLoad) continue;

    QFile file(path->file);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
      ok = false;
      continue;
    }

    QDataStream dataStream(&file);

    size_t csz = sref[sign]->buffData.size(),
      newsz = csz + path->signls[sign].vlsCnt,
      vlSz = sizeof(Value) * SV_PACKETSZ;

    sdata->buffData.resize(newsz);

    size_t buffSz = vlSz * path->signls[sign].vlsCnt;
    Value* buff = new Value[buffSz];
    memset(buff, 0, buffSz);

    for (size_t i = csz, j = 0; i < newsz; ++i, ++j){
      sdata->buffData[i].vals = &buff[j * SV_PACKETSZ];
    }
    int psz = path->signls[sign].patchApos.size(),
      posMem = 0,
      patchNum = 0,
      tmSz = sizeof(uint64_t),
      valSz = tmSz + (int)vlSz,
      vheadSz = sizeof(ValueData);

    uLong comprSz = 0,
      dataSz = 0;

    QVector<Bytef> inArr, outArr;

    while (!dataStream.atEnd()) {

      dataStream.readRawData((char*)&comprSz, 4);
      dataStream.readRawData((char*)&dataSz, 4);

      inArr.resize(comprSz);
      outArr.resize(dataSz);

      dataStream.readRawData((char*)inArr.data(), comprSz);

      if (uncompress(outArr.data(), &dataSz, inArr.data(), comprSz) != 0) {
        ok = false;
        continue;
      }

      for (int i = posMem; i < psz; ++i) {

        if (path->signls[sign].patchApos[i].first != patchNum) {
          posMem = i;
          break;
        }

        int offs = path->signls[sign].patchApos[i].second;

        if (offs > (int)dataSz) break;

        size_t vlCnt = (size_t)((ValueData*)(outArr.data() + offs))->vlCnt;

        offs += vheadSz;

        Bytef* pData = outArr.data() + offs;

        for (size_t j = 0; j < vlCnt; ++j) {
          sdata->buffData[csz + j].beginTime = *(uint64_t*)(pData + j * valSz) + path->utcOffsMs;
          memcpy(sdata->buffData[csz + j].vals, pData + j * valSz + tmSz, vlSz);
        }

        csz += vlCnt;
      }

      ++patchNum;
    }

    file.close();

    isNewFile = true;
    path->signls[sign].isLoad = true;

  }

  if (!isNewFile){
    return ok && !sdata->buffData.empty();
  }
  std::sort(sdata->buffData.begin(), sdata->buffData.end(), [](RecData& left, RecData& right) {
    return left.beginTime < right.beginTime;
  });

  size_t bsz = sdata->buffData.size();
  sdata->buffMinTime = sdata->buffData[0].beginTime;
  sdata->buffMaxTime = sdata->buffData[bsz - 1].beginTime + SV_CYCLESAVE_MS;

  double minValue = INT32_MAX, maxValue = -INT32_MAX;

  if (sdata->type == ValueType::INT) {
    for (auto& val : sdata->buffData) {

      for (int i = 0; i < SV_PACKETSZ; ++i) {

        if (val.vals[i].vInt > maxValue) maxValue = val.vals[i].vInt;
        if (val.vals[i].vInt < minValue) minValue = val.vals[i].vInt;
      }
    }
  }
  else if (sdata->type == ValueType::FLOAT) {
    for (auto& val : sdata->buffData) {

      for (int i = 0; i < SV_PACKETSZ; ++i) {

        if (val.vals[i].vFloat > maxValue) maxValue = val.vals[i].vFloat;
        if (val.vals[i].vFloat < minValue) minValue = val.vals[i].vFloat;
      }
    }
  }

  sdata->buffMinValue = minValue;
  sdata->buffMaxValue = maxValue;

  sdata->buffValuePos = bsz - 1;

  return ok && !sdata->buffData.empty();
}

