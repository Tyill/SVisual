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

#include "src/stdafx.h"
#include "SVAuxFunc/TimerDelay.h"
#include "SVAuxFunc/Front.h"
#include "SVConfig/SVConfigData.h"
#include "SVConfig/SVConfigLimits.h"
#include "SVTriggerPanel/SVTriggerPanel.h"
#include "ui_triggerPanel.h"

class triggerPanel : public QDialog
{
	Q_OBJECT

public:
   
    SV_Trigger::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
    SV_Trigger::pf_getCopyModuleRef pfGetCopyModuleRef = nullptr;
    SV_Trigger::pf_getSignalData pfGetSignalData = nullptr;
    SV_Trigger::pf_getModuleData pfGetModuleData = nullptr;
    SV_Trigger::pf_onTriggerCBack pfOnTriggerCBack = nullptr;

	Ui::triggerPanelClass ui;

	triggerPanel(QWidget *parent, SV_Trigger::config);
	~triggerPanel();

    void startUpdateThread();

	void updateWin();

    // вернуть все триггеры
    QMap<QString, SV_Trigger::triggerData*> triggerPanel::getCopyTriggerRef();

    // вернуть данные триггера
    SV_Trigger::triggerData* triggerPanel::getTriggerData(const QString& trg);

    // добавить триггер
    bool triggerPanel::addTrigger(const QString& trg, SV_Trigger::triggerData* td);

    // удалить триггер
    bool triggerPanel::delTrigger(const QString& trg);


private:

    SV_Trigger::config cng;

    SV_Trigger::eventType currCondition_ = SV_Trigger::eventType::less;
    QMap<QString, SV_Trigger::triggerData*> triggerData_;

    QString selModule_, selSignal_, selTrigg_, selDirProc_;

    bool isSelModule_ = true;

    int cTriggRow_ = 0;

    SV_Aux::TimerDelay tmDelay_;
    SV_Aux::Front front_;

    std::thread workThr_;
    std::mutex mtx_;
    bool thrStop_ = false;


    void load();

    void showEvent(QShowEvent * event);

    void enaBtnCondition(bool ena);

    void updateTableTrigger();
    void updateTableSignal();
    void updateStateSignal();

    bool checkCondition(SV_Trigger::triggerData* tr, SV_Cng::signalData* sd);
    void workCycle();


private slots:
	void selModule(QListWidgetItem * item);
	void selSignal(QTableWidgetItem * item);
	void selTrigger(QTableWidgetItem * item);
	void addTrigger();
	void delTrigger();
	void changeTrigger();
	void paramChange();
    void selCondition(SV_Trigger::eventType);
	void selDirProc();
};


