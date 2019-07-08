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
#include "thrLoadData.h"
#include "forms/mainWin.h"
#include "SVConfig/SVConfigLimits.h"
#include "Lib/zlib/zlib.h"

extern MainWin* mainWin;

using namespace SV_Cng;

thrLoadData::thrLoadData(QStringList files){

	QThread* thr = new QThread(this);
		
	this->moveToThread(thr);
	connect(thr, &QThread::started, [this, files](){
        bool ok = mainWin->loadData(files);
        emit finished(ok);
	});
	connect(this, &thrLoadData::finished, [this, thr](){
		thr->quit();
	});

	thr->start();
}

bool uncompressData(char* inArr, size_t& cPos, size_t& outDataSz, char** outArr){

    int sInt = sizeof(int);

    int comprSz = *(int*)(inArr + cPos);
    uLongf dataSz = *(int*)(inArr + cPos + sInt);

    *outArr = new char[dataSz];

    int ret = uncompress((Bytef*)*outArr, &dataSz, (Bytef*)(inArr + cPos + sInt + sInt), comprSz);

    outDataSz = dataSz;
    cPos += comprSz + sInt + sInt;

    return ret == 0;
}

bool MainWin::loadModuleVals(QString path){

    if (mainWin->fileRef_.contains(path)) return true;

    int utcOffs = path.lastIndexOf("UTC");

    if (utcOffs > 0) {
        int ssz = path.size();
        QString ss = path.left(ssz - 4);

        bool isNum = false;
        utcOffs = ss.mid(utcOffs + 3).toInt(&isNum) * 3600000 - QDateTime::currentDateTime().offsetFromUtc() * 1000;

        if (!isNum) utcOffs = QDateTime::currentDateTime().offsetFromUtc() * 1000;
    }

    QFile file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return false;

    mainWin->fileRef_.insert(path, new fileData(path, utcOffs));

    QDataStream dataStream(&file);

    size_t fsz = file.size();
    char* inArr = new char[fsz];
    dataStream.readRawData(inArr, fsz);

    file.close();

    char* outArr; size_t cFilePos = 0, dataSz = 0, patchNum = 0;
    int valSz = sizeof(uint64_t) + sizeof(value) * SV_PACKETSZ, vdataSz = sizeof(valueData);
    while ((cFilePos < fsz) && uncompressData(inArr, cFilePos, dataSz, &outArr)){

        ////

        int cPos = 0, szVl = 0;

        size_t itData = 0;
        while (itData < dataSz){

            valueData* vr = (valueData*)(outArr + itData);

            QString sign = QString(vr->name) + vr->module;

            if (!fileRef_[path]->signls.contains(sign))
                fileRef_[path]->signls.insert(sign, fileData::fsd{ false, 0 });

            fileRef_[path]->signls[sign].patchApos.append(QPair<int, int>(patchNum, cPos));
            fileRef_[path]->signls[sign].vlsCnt += vr->vlCnt;

            if (signalRef_.contains(sign)){

                if (!signalRef_[sign]->isActive){
                    signalRef_[sign]->isActive = true;
                    moduleRef_[vr->module]->isActive = true;
                    groupRef_[signalRef_[sign]->group.c_str()]->isActive = true;

                    signalRef_[sign]->type = vr->type;
                }

                if (signalRef_[sign]->group.empty())
                    signalRef_[sign]->group = vr->group;

                if (signalRef_[sign]->comment.empty())
                    signalRef_[sign]->comment = vr->comment;

                szVl = vdataSz + vr->vlCnt * valSz;
                itData += szVl;
                cPos += szVl;
                continue;
            }

            if (!moduleRef_.contains(vr->module)) {
                moduleRef_.insert(vr->module, new moduleData(vr->module));
                moduleRef_[vr->module]->isActive = true;
            }

            moduleRef_[vr->module]->signls.push_back(sign.toStdString());

            if (!groupRef_.contains(vr->group)){
                groupRef_.insert(vr->group, new groupData(vr->group));
                groupRef_[vr->group]->isActive = true;
            }
			groupRef_[vr->group]->signls.push_back(sign.toStdString());

            auto sd = new signalData();
            sd->name = vr->name;
            sd->module = vr->module;
            sd->group = vr->group;
            sd->comment = vr->comment;
            sd->type = vr->type;
            sd->isActive = true;

			signalRef_[sign] = sd;

            szVl = vdataSz + vr->vlCnt * valSz;
            itData += szVl;
            cPos += szVl;
        }

        delete[] outArr;
        ++patchNum;
    }

    delete[] inArr;

    return true;
}

bool loadSignalData(const QString& sign){
   
    if (!mainWin->signalRef_.contains(sign))
        return false;

    MainWin::config& cng = mainWin->cng;

    auto sdata = mainWin->signalRef_[sign];
    
    bool  ok = true, isNewFile = false;
    for (auto path : mainWin->fileRef_){

        if (!path->signls.contains(sign) || path->signls[sign].isLoad) continue;

        QFile file(path->file);

        if (!file.exists() || !file.open(QIODevice::ReadOnly)){
            ok = false;
            continue;
        }

        QDataStream dataStream(&file);

        size_t fsz = file.size();
        char* inArr = new char[fsz];
        dataStream.readRawData(inArr, fsz);

        file.close();

        int csz = mainWin->signalRef_[sign]->buffData.size(),
            newsz = csz +  path->signls[sign].vlsCnt,
            vlSz = sizeof(value) * SV_PACKETSZ;

        sdata->buffData.resize(newsz);

        int buffSz = vlSz * path->signls[sign].vlsCnt;
        value* buff = new value[buffSz];
        memset(buff, 0, buffSz);

        for (int i = csz, j = 0; i < newsz; ++i, ++j)
            sdata->buffData[i].vals = &buff[j * SV_PACKETSZ];

        char* outArr; size_t cFilePos = 0, dataSz = 0;
        int psz = path->signls[sign].patchApos.size(), posMem = 0, patchNum = 0, vdSz =  sizeof(valueData);
        while ((cFilePos < fsz) && uncompressData(inArr, cFilePos, dataSz, &outArr)){

            for (int i = posMem; i < psz; ++i){

                if (path->signls[sign].patchApos[i].first != patchNum){ posMem = i; break; }

                int offs = path->signls[sign].patchApos[i].second;

                if (offs > dataSz) break;

                int vlCnt = ((valueData*)(outArr + offs))->vlCnt,
                        tmSz = sizeof(uint64_t), rdSz = tmSz + vlSz;

                offs += vdSz;
                for (int j = 0; j < vlCnt; ++j){
                    sdata->buffData[csz + j].beginTime = *(uint64_t*)(outArr + offs + j * rdSz) + path->utcOffsMs;
                    memcpy(sdata->buffData[csz + j].vals, outArr + offs + j * rdSz + tmSz, vlSz);
                }

                csz += vlCnt;
            }
            delete[] outArr;
            ++patchNum;
        }
        isNewFile = true;
        path->signls[sign].isLoad = true;
        delete[] inArr;
    }

    if (!isNewFile) 
        return ok && !sdata->buffData.empty();

    std::sort(sdata->buffData.begin(), sdata->buffData.end(), [](recData& left, recData& right) {
        return left.beginTime < right.beginTime;
    });

    int sz = sdata->buffData.size();
    sdata->buffMinTime = sdata->buffData[0].beginTime;
    sdata->buffMaxTime = sdata->buffData[sz - 1].beginTime + SV_CYCLESAVE_MS;

    double minValue = INT32_MAX, maxValue = -INT32_MAX;

    if (sdata->type == valueType::tInt){
        for (auto& val : sdata->buffData){

            for (int i = 0; i < SV_PACKETSZ; ++i){

                if (val.vals[i].tInt > maxValue) maxValue = val.vals[i].tInt;
                if (val.vals[i].tInt < minValue) minValue = val.vals[i].tInt;
            }
        }
    }
    else if (sdata->type == valueType::tFloat){
        for (auto& val : sdata->buffData){

            for (int i = 0; i < SV_PACKETSZ; ++i){

                if (val.vals[i].tFloat > maxValue) maxValue = val.vals[i].tFloat;
                if (val.vals[i].tFloat < minValue) minValue = val.vals[i].tFloat;
            }
        }
    }

    sdata->buffMinValue = minValue;
    sdata->buffMaxValue = maxValue;

    sdata->buffValuePos = sz - 1;

    return ok && !sdata->buffData.empty();
}

bool MainWin::loadData(QStringList files){

    cng.selOpenDir = files[0].left(files[0].lastIndexOf('/'));

    bool ok = false;
    for(QString& path : files)
        ok = loadModuleVals(path);

    return ok;
}