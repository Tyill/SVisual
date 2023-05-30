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
#include "clickhouse/client.h"

#include <QDateTime>
#include <QDebug>

using namespace SV_Base;
namespace ch = clickhouse;

DbClickHouseLoader::DbClickHouseLoader(QMainWindow* mainWin, QObject* parent):
    QObject(parent),
    m_mainWin(qobject_cast<MainWin*>(mainWin)){

}

bool DbClickHouseLoader::loadSignalNames(){

    try{        
        auto& sref = m_mainWin->signalRef_;
        auto& mref = m_mainWin->moduleRef_;
        sref.clear();
        mref.clear();
        QString q = "SELECT id, sname, module, stype FROM tblSignal;";
        auto chClient = newClient();
        chClient->Select(q.toStdString(), [&sref, &mref](const ch::Block& block)mutable{
            int packetPos = 0;
            for (size_t i = 0; i < block.GetRowCount(); ++i){
                int sId = block[0]->As<ch::ColumnInt32>()->At(i);
                auto sname = block[1]->As<ch::ColumnString>()->At(i);
                auto module = block[2]->As<ch::ColumnString>()->At(i);
                auto stype = block[3]->As<ch::ColumnInt32>()->At(i);
                SV_Base::SignalData* sdata = new SV_Base::SignalData();
                sdata->id = sId;
                sdata->type = SV_Base::ValueType(stype);
                sdata->isActive = true;
                sdata->name = std::string(sname);
                sdata->module = std::string(module);
                sref.insert(QString::fromStdString(sdata->name + sdata->module), sdata);

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

bool DbClickHouseLoader::loadSignalData(const QString& sname, const QDateTime& from, const QDateTime& to){

    const auto& sref = m_mainWin->signalRef_;

    if (!sref.contains(sname)) return false;

    const auto& sdata = sref[sname];
    auto cng = m_mainWin->cng;
    auto timeBegin = from.toMSecsSinceEpoch();
    auto timeEnd = to.toMSecsSinceEpoch();

    try{        
        QString q = QString("SELECT count() FROM tblSData "
                            "WHERE id = %1 AND ts BETWEEN %2 AND %3;")
                            .arg(sdata->id).arg(timeBegin).arg(timeEnd);
        if (timeEnd <= 0){
            q = QString("SELECT count() FROM tblSData "
                        "WHERE id = %1 AND ts >= %2;")
                        .arg(sdata->id).arg(timeBegin);
        }
        int sValueCount = 0; 
        auto chClient = newClient();
        chClient->Select(q.toStdString(), [&sValueCount](const ch::Block& block){
            if (block.GetRowCount() > 0 && block.GetColumnCount() > 0){
                sValueCount = block[0]->As<ch::ColumnUInt64>()->At(0);
            }
        });
        
        sdata->buffData.resize(sValueCount / SV_PACKETSZ);

        if (m_signalValueBuff.contains(sdata->id)){
            delete[] m_signalValueBuff[sdata->id];
            m_signalValueBuff.remove(sdata->id);
        }
        if (sValueCount == 0) return true;

        size_t buffSz = sizeof(Value) * sValueCount;
        Value* buff = new Value[buffSz];
        memset(buff, 0, buffSz);

        m_signalValueBuff[sdata->id] = buff;

        for (size_t i = 0, j = 0; i < sValueCount / SV_PACKETSZ; ++i, ++j){
            sdata->buffData[i].vals = &buff[j * SV_PACKETSZ];
        }

        q = QString("SELECT ts, value  FROM tblSData "
                    "WHERE id = %1 AND ts BETWEEN %2 AND %3 ORDER BY id, ts;")
                            .arg(sref.value(sname)->id).arg(timeBegin).arg(timeEnd);
        if (timeEnd <= 0){
            q = QString("SELECT ts, value FROM tblSData "
                        "WHERE id = %1 AND ts >= %2 ORDER BY id, ts;")
                        .arg(sref.value(sname)->id).arg(timeBegin);
        }
        int buffPos = 0;
        chClient->Select(q.toStdString(), [&sdata, cng, &buffPos, sValueCount](const ch::Block& block)mutable{
            int packetPos = 0;
            for (size_t i = 0; i < block.GetRowCount() && buffPos < sValueCount / SV_PACKETSZ; ++i){                
                if (packetPos == SV_PACKETSZ){
                    ++buffPos;
                    packetPos = 0;
                }
                sdata->buffData[buffPos].beginTime = block[0]->As<ch::ColumnUInt64>()->At(i);
                sdata->buffData[buffPos].vals[packetPos].vFloat = block[1]->As<ch::ColumnFloat32>()->At(i);
                ++packetPos;
            }
        });
    }catch(std::exception& e){
        qWarning() << Q_FUNC_INFO << e.what();
        return false;
    }
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
