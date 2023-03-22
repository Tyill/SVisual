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

using namespace SV_Base;
namespace ch = clickhouse;

DbClickHouseLoader::DbClickHouseLoader(QMainWindow* mainWin, QObject* parent):
    QObject(parent),
    m_mainWin(qobject_cast<MainWin*>(mainWin)){

}

bool DbClickHouseLoader::loadSignalData(const QDateTime& from, const QDateTime& to){

    QMap<QString, SignalData*> sref = m_mainWin->signalRef_;

    auto timeBegin = from.toMSecsSinceEpoch();
    auto timeEnd = to.toMSecsSinceEpoch();
    auto cng = m_mainWin->cng;
    try{
        QString strIds;
        for (auto s : sref){
         strIds += QString("%1,").arg(s->id);
        }
        strIds.chop(1);
        QString q = QString("SELECT id, ts, value FROM tblSData "
                         "WHERE id IN (%1) AND ts BETWEEN %2 AND %3 ORDER BY id, ts;")
                         .arg(strIds).arg(timeBegin).arg(timeEnd);
        if (timeEnd <= 0){
         q = QString("SELECT id, ts, value FROM tblSData "
                     "WHERE id IN (%1) AND ts >= %2 ORDER BY id, ts;")
                     .arg(strIds).arg(timeBegin);
        }

        auto chClient = newClient();
        chClient->Select(q.toStdString(), [&sref](const ch::Block& block){
            int csId = -1;
            SignalData* sd = nullptr;
            for (size_t i = 0; i < block.GetRowCount(); ++i){
                int sId = block[0]->As<ch::ColumnInt32>()->At(i);
                if (csId != sId){
                    csId = sId;
                    if (auto it = std::find_if(sref.begin(), sref.end(),[sId](const auto& v){
                        return v->id == sId;
                    }); it != sref.end()){
                        sd = *it;
                    }
                }
                sd->buffData.push_back(push_back(block[1]->As<ch::ColumnInt64>()->At(i));
                sd->values.push_back(block[2]->As<ch::ColumnFloat64>()->At(i));
                sd->quality.push_back(block[3]->As<ch::ColumnInt8>()->At(i));
            }
        });
     }catch(std::exception& e){
//         logWarning({Q_FUNC_INFO, e.what()});
//         m_chClient->ResetConnection();
     }
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
