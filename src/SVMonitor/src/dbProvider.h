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
#pragma once

#include "stdafx.h"
#include "structurs.h"
#include "Lib/sqlite/sqlite3.h"
#include "SVConfig/SVConfigData.h"
#include "SVTriggerPanel/SVTriggerPanel.h"

class dbProvider
{

public:

    dbProvider(const QString& path);

    ~dbProvider();

    bool isConnect();

    bool saveTriggers(const QMap<QString, SV_Trigger::triggerData*>& trg);

    QVector<SV_Trigger::triggerData*> getTrigger(const QString& signal, const QString& module);
    SV_Trigger::triggerData* getTrigger(const QString& trname);

    bool saveSignals(const std::map<std::string, SV_Cng::signalData*>& signs);
    SV_Cng::signalData getSignal(const QString& signal, const QString& module);

    bool saveAttrSignals(const QMap<QString, signalAttr>& attr);
    signalAttr getAttrSignal(const QString& signal, const QString& module);
    void delAttrSignal(const QString& signal, const QString& module);

    void saveEvent(QString trigger, QDateTime dt);
    QVector<uEvent> getEvents(QDateTime beginTime, QDateTime endTime);

private:

    sqlite3* db_ = nullptr;

    bool isConnect_ = false;

    QString pathDB_;

    QMutex mtx_;

    bool init();

    bool open();

    void close();

    bool query(const std::string& query,
        std::vector<std::vector<std::string>>& result);
};