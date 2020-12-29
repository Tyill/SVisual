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
#include "SVTriggerDialog/forms/trigger_dialog_impl.h"
#include "SVTriggerDialog/trigger_dialog.h"

namespace SV_Trigger {

  QDialog *createTriggerPanel(QWidget *parent, Config cng) {

    return new TriggerDialog(parent, cng);
  }

  void startUpdateThread(QDialog* stp){

    if (stp)
      ((TriggerDialog*)stp)->startUpdateThread();
  }

  void setGetCopySignalRef(QDialog *stp, pf_getCopySignalRef f) {

    if (stp)
      ((TriggerDialog*)stp)->pfGetCopySignalRef = f;
  }

  void setGetCopyModuleRef(QDialog* stp, pf_getCopyModuleRef f){

    if (stp)
      ((TriggerDialog*)stp)->pfGetCopyModuleRef = f;
  }

  void setGetModuleData(QDialog* stp, pf_getModuleData f){

    if (stp)
      ((TriggerDialog*)stp)->pfGetModuleData = f;
  }

  void setGetSignalData(QDialog *stp, pf_getSignalData f) {

    if (stp)
      ((TriggerDialog*)stp)->pfGetSignalData = f;
  }

  // вернуть все триггеры
  QMap<QString, TriggerData*> getCopyTriggerRef(QDialog *stp){

    if (stp)
      return ((TriggerDialog*)stp)->getCopyTriggerRef();
    else
      return QMap<QString, TriggerData*>();
  }

  // вернуть данные триггера
  TriggerData* getTriggerData(QDialog *stp, const QString& name){

    if (stp)
      return ((TriggerDialog*)stp)->getTriggerData(name);
    else
      return nullptr;
  }

  // добавить триггер
  bool addTrigger(QDialog *stp, const QString& name, TriggerData* td){

    if (stp)
      return ((TriggerDialog*)stp)->addTrigger(name, td);
    else
      return false;
  }

  void setOnTriggerCBack(QDialog* stp, pf_onTriggerCBack f){

    if (stp)
      ((TriggerDialog*)stp)->pfOnTriggerCBack = f;
  }

  /// вернуть тип события как строку
  /// \param type
  /// \return
  QString getEventTypeStr(EventType type) {

    QString res = "NONE";

    switch (type) {
    case EventType::CONNECT_MODULE:
      res = "CONNECT_MODULE";
      break;
    case EventType::DISCONNECT_MODULE:
      res = "DISCONNECT_MODULE";
      break;
    case EventType::LESS:
      res = "LESS";
      break;
    case EventType::EQUALS:
      res = "EQUALS";
      break;
    case EventType::MORE:
      res = "MORE";
      break;
    case EventType::POS_FRONT:
      res = "POS_FRONT";
      break;
    case EventType::NEG_FRONT:
      res = "NEG_FRONT";
      break;
    default:
      res = "NONE";
      break;
    }

    return res;
  }
}