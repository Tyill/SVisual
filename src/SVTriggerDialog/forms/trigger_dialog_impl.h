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

#include <thread>
#include <mutex>

#include "SVAuxFunc/timer_delay.h"
#include "SVAuxFunc/front.h"
#include "SVConfig/config_data.h"
#include "SVConfig/config_limits.h"
#include "SVTriggerDialog/trigger_dialog.h"
#include "GeneratedFiles/ui_trigger_dialog.h"

class TriggerDialog : public QDialog
{
  Q_OBJECT

public:

  SV_Trigger::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
  SV_Trigger::pf_getCopyModuleRef pfGetCopyModuleRef = nullptr;
  SV_Trigger::pf_getSignalData pfGetSignalData = nullptr;
  SV_Trigger::pf_getModuleData pfGetModuleData = nullptr;
  SV_Trigger::pf_onTriggerCBack pfOnTriggerCBack = nullptr;

  Ui::TriggerDialog ui;

  TriggerDialog(QWidget *parent, SV_Trigger::Config);
  ~TriggerDialog();

  void startUpdateThread();

  void updateWin();

  QMap<QString, SV_Trigger::TriggerData*> getCopyTriggerRef();

  SV_Trigger::TriggerData* getTriggerData(const QString& trg);

  bool addTrigger(const QString& trg, SV_Trigger::TriggerData* td);

  bool delTrigger(const QString& trg);


private:

  SV_Trigger::Config cng;

  SV_Trigger::EventType currCondition_ = SV_Trigger::EventType::LESS;
  QMap<QString, SV_Trigger::TriggerData*> triggerData_;

  QString selModule_, selSignal_, selTrigg_, selDirProc_;

  bool isSelModule_ = true;

  int cTriggRow_ = 0;

  std::thread workThr_;
  std::mutex mtx_;
  volatile bool thrStop_ = false;


  void load();

  void showEvent(QShowEvent * event);

  void enaBtnCondition(bool ena);

  void updateTableTrigger();
  void updateTableSignal();
  void updateStateSignal();

  bool checkCondition(SV_Trigger::TriggerData* tr, SV_Base::SignalData* sd);
  void workCycle();

  private slots:
  void selModule(QListWidgetItem * item);
  void selSignal(QTableWidgetItem * item);
  void selTrigger(QTableWidgetItem * item);
  void addTrigger();
  void delTrigger();
  void changeTrigger();
  void paramChange();
  void selCondition(SV_Trigger::EventType);
  void selDirProc();
};


