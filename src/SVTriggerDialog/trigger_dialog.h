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

#include <QtCore>
#include "SVConfig/config_data.h"

class QDialog;

namespace SV_Trigger {

  struct Config {

    int cycleRecMs;
    int packetSz;

    Config(int cycleRecMs_ = 100, int packetSz_ = 10) :
      cycleRecMs(cycleRecMs_),
      packetSz(packetSz_) {}
  };

  /// тип события
  enum class EventType {
    NONE = -1,            ///< нет
    CONNECT_MODULE = 0,    ///< модуль подключен
    DISCONNECT_MODULE = 1, ///< модуль отключен
    LESS = 2,             ///< "<"
    EQUALS = 3,           ///< "=="
    MORE = 4,             ///< ">"
    POS_FRONT = 5,         ///< положительный фронт
    NEG_FRONT = 6,         ///< отрицательный фронт
  };

  /// триггер
  struct TriggerData {
    bool isActive;               ///< активен

    QString name;                ///< название триггера
    QString signal;              ///< сигнал
    QString module;              ///< модуль
    QString userProcPath;        ///< путь к польз процессу
    QString userProcArgs;        ///< аргументы к польз процессу, через /t

    EventType condType;          ///< тип
    int condValue;               ///< значение условия (порог срабатывания)
    int condTOut;                ///< таймаут срабатывания, с

    TriggerData() {
      isActive = false;
      condType = EventType::NONE;
      condTOut = 0;
    }
  };

  QDialog* createTriggerPanel(QWidget* parent, Config);

  void startUpdateThread(QDialog* panel);

  typedef QMap<QString, SV_Base::SignalData*>(*pf_getCopySignalRef)();
  void setGetCopySignalRef(QDialog* panel, pf_getCopySignalRef f);

  typedef SV_Base::SignalData *(*pf_getSignalData)(const QString &sign);
  void setGetSignalData(QDialog* panel, pf_getSignalData f);

  typedef QMap<QString, SV_Base::ModuleData*>(*pf_getCopyModuleRef)();
  void setGetCopyModuleRef(QDialog* panel, pf_getCopyModuleRef f);

  typedef SV_Base::ModuleData *(*pf_getModuleData)(const QString &module);
  void setGetModuleData(QDialog* panel, pf_getModuleData f);

  typedef void(*pf_onTriggerCBack)(const QString& name);
  void setOnTriggerCBack(QDialog* panel, pf_onTriggerCBack f);

  // вернуть все триггеры
  QMap<QString, TriggerData*> getCopyTriggerRef(QDialog* panel);

  // вернуть данные триггера
  TriggerData* getTriggerData(QDialog* panel, const QString& name);

  // добавить триггер
  bool addTrigger(QDialog* panel, const QString& name, TriggerData* td);

  // вернуть тип события как строку
  QString getEventTypeStr(EventType type);
}