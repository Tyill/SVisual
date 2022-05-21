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

#include "structurs.h"
#include <QMutex>
#include <QString>

class sqlite3;
class QDateTime;

class DbProvider
{

public:

  DbProvider(const QString& path);

  ~DbProvider();

  bool isConnect();

  bool saveTriggers(const QMap<QString, SV_Trigger::TriggerData*>& trg);

  QVector<SV_Trigger::TriggerData*> getTrigger(const QString& signal, const QString& module);
  SV_Trigger::TriggerData* getTrigger(const QString& trname);

  bool saveSignals(const std::map<std::string, SV_Base::SignalData*>& signs);
  SV_Base::SignalData getSignal(const QString& signal, const QString& module);

  bool saveAttrSignals(const QMap<QString, SignalAttr>& attr);
  SignalAttr getAttrSignal(const QString& signal, const QString& module);
  void delAttrSignal(const QString& signal, const QString& module);

  void saveEvent(const QString& trigger, const QDateTime& dt);
  QVector<UserEvent> getEvents(const QDateTime& beginTime, const QDateTime& endTime);

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