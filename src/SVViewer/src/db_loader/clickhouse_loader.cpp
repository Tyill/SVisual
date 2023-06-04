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

#include "clickhouse_loader.h"
#include "SVViewer/forms/main_win.h"
#ifdef WIN32
#include "3rdparty/clickhouse/client.h"
#else
#include <clickhouse/client.h>
#endif

#include <QDebug>

using namespace SV_Base;
namespace ch = clickhouse;

DbClickHouseLoader::DbClickHouseLoader(QMainWindow* mainWin, QObject* parent):
    QObject(parent),
    m_mainWin(qobject_cast<MainWin*>(mainWin)){

}

QPair<QDateTime, QDateTime> DbClickHouseLoader::getSignalInterval(const QString& sname)
{
    if (m_timeDiapMem.contains(sname)){
        m_lastValidTimeDiap = m_timeDiapMem[sname];
    }
    return m_lastValidTimeDiap;
}

bool DbClickHouseLoader::loadSignalNames(){

    try{        
        auto& sref = m_mainWin->signalRef_;
        auto& mref = m_mainWin->moduleRef_;
        mref.clear();

        QString q = "SELECT id, sname, module, stype FROM tblSignal;";
        auto chClient = newClient();
        chClient->Select(q.toStdString(), [&sref, &mref](const ch::Block& block)mutable{
            int packetPos = 0;
            for (size_t i = 0; i < block.GetRowCount(); ++i){
                int sId = block[0]->AsStrict<ch::ColumnInt32>()->At(i);
                auto sname = block[1]->AsStrict<ch::ColumnString>()->At(i);
                auto module = block[2]->AsStrict<ch::ColumnString>()->At(i);
                auto stype = block[3]->AsStrict<ch::ColumnInt32>()->At(i);
                SV_Base::SignalData* sdata = new SV_Base::SignalData();
                sdata->id = sId;
                sdata->type = SV_Base::ValueType(stype);
                sdata->isActive = true;
                sdata->name = std::string(sname);
                sdata->module = std::string(module);
                auto stdname = QString::fromStdString(sdata->name + sdata->module);
                if (sref.contains(stdname)){
                    sref[stdname]->isActive = true;
                    sref[stdname]->id = sId;
                }else{
                    sref.insert(stdname, sdata);
                }
                const auto mname = QString::fromStdString(sdata->module);
                if (!mref.contains(mname)){
                    SV_Base::ModuleData* mdata = new SV_Base::ModuleData(sdata->module);
                    mdata->isActive = true;
                    mdata->isEnable = true; 
                    mref.insert(mname, mdata);
                }
                mref[mname]->signls.push_back(sdata->name + sdata->module);
            }
        });
    }catch(std::exception& e){
        qWarning() << Q_FUNC_INFO << e.what();
        return false;
    }
    return true;
}

bool DbClickHouseLoader::loadSignalData(const QString& sname, const QDateTime& fromDTime, const QDateTime& toDTime){

    const auto& sref = m_mainWin->signalRef_;

    if (!sref.contains(sname)) return false;

    const auto& sdata = sref[sname];
    
    auto cng = m_mainWin->cng;
    auto timeBegin = fromDTime.isValid() ? fromDTime.toMSecsSinceEpoch() : 0;
    auto timeEnd = toDTime.isValid() ? toDTime.toMSecsSinceEpoch() : 0;

    if (m_timeDiapMem.contains(sname) &&
        m_timeDiapMem[sname].first == fromDTime &&
        m_timeDiapMem[sname].second == toDTime){
        return true;
    }
    try{        
        QString queryCount = QString("SELECT count() FROM tblSData "
                                     "WHERE id = %1;")
                                     .arg(sdata->id);
        QString queryData = QString("SELECT ts, value  FROM tblSData "
                                    "WHERE id = %1 ORDER BY ts;")
                                    .arg(sref.value(sname)->id);
        if (timeBegin > 0 && timeEnd > 0){
            queryCount = QString("SELECT count() FROM tblSData "
                                 "WHERE id = %1 AND ts BETWEEN %2 AND %3;")
                                 .arg(sdata->id).arg(timeBegin).arg(timeEnd);
            queryData = QString("SELECT ts, value  FROM tblSData "
                                "WHERE id = %1 AND ts BETWEEN %2 AND %3 ORDER BY ts;")
                                .arg(sref.value(sname)->id).arg(timeBegin).arg(timeEnd);
        }
        else if (timeBegin > 0){
            queryCount = QString("SELECT count() FROM tblSData "
                                 "WHERE id = %1 AND ts >= %2;")
                                 .arg(sdata->id).arg(timeBegin);
            queryData = QString("SELECT ts, value FROM tblSData "
                                "WHERE id = %1 AND ts >= %2 ORDER BY ts;")
                                .arg(sref.value(sname)->id).arg(timeBegin);
        }
        else if (timeEnd > 0){
            queryCount = QString("SELECT count() FROM tblSData "
                                 "WHERE id = %1 AND ts < %2;")
                                 .arg(sdata->id).arg(timeEnd);
            queryData = QString("SELECT ts, value FROM tblSData "
                                "WHERE id = %1 AND ts < %2 ORDER BY ts;")
                                .arg(sref.value(sname)->id).arg(timeEnd);
        }
        int sValueCount = 0; 
        auto chClient = newClient();
        chClient->Select(queryCount.toStdString(), [&sValueCount](const ch::Block& block){
            if (block.GetRowCount() > 0 && block.GetColumnCount() > 0){
                sValueCount = block[0]->AsStrict<ch::ColumnUInt64>()->At(0);
            }
        });
        
        sdata->buffData.resize(sValueCount / SV_PACKETSZ);

        if (m_signalValueBuff.contains(sdata->id)){
            delete[] m_signalValueBuff[sdata->id];
            m_signalValueBuff.remove(sdata->id);
        }
        if (sValueCount > 0){
            size_t buffSz = sizeof(Value) * sValueCount;
            Value* buff = new Value[buffSz];
            memset(buff, 0, buffSz);

            m_signalValueBuff[sdata->id] = buff;

            for (size_t i = 0, j = 0; i < sValueCount / SV_PACKETSZ; ++i, ++j){
                sdata->buffData[i].vals = &buff[j * SV_PACKETSZ];
            }

            int buffPos = 0;
            chClient->Select(queryData.toStdString(), [&sdata, cng, &buffPos, sValueCount](const ch::Block& block)mutable{
                int packetPos = 0;
                for (size_t i = 0; i < block.GetRowCount() && buffPos < sValueCount / SV_PACKETSZ; ++i){
                    sdata->buffData[buffPos].beginTime = block[0]->AsStrict<ch::ColumnUInt64>()->At(i);
                    if (sdata->type == SV_Base::ValueType::FLOAT)
                        sdata->buffData[buffPos].vals[packetPos].vFloat = block[1]->AsStrict<ch::ColumnFloat32>()->At(i);
                    else{
                        sdata->buffData[buffPos].vals[packetPos].vInt = block[1]->AsStrict<ch::ColumnFloat32>()->At(i);
                    }
                    ++packetPos;
                    if (packetPos == SV_PACKETSZ){
                        ++buffPos;
                        packetPos = 0;
                    }
                }
            });
        }
    }catch(std::exception& e){
        qWarning() << Q_FUNC_INFO << e.what();
        return false;
    }
   
    const size_t bsz = sdata->buffData.size();
    if (bsz > 0){
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
    }

    m_timeDiapMem[sname] = {fromDTime, toDTime};

    return true;
}

std::unique_ptr<clickhouse::Client> DbClickHouseLoader::newClient()const
{
    ch::ClientOptions opts;{
        opts.SetDefaultDatabase(m_mainWin->cng.inputDataBaseName.toStdString());
        auto sp = m_mainWin->cng.inputDataBaseAddr.split(':');
        if (sp.size() > 0){
            opts.SetHost(sp[0].toStdString());
            opts.SetPort(sp[1].toInt());
        }else{
            opts.SetHost(m_mainWin->cng.inputDataBaseAddr.toStdString());
        }
    }
    return std::make_unique<ch::Client>(opts);
}
