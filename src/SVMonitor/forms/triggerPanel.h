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

#include "src/stdafx.h"
#include "SVConfig/SVConfigData.h"
#include "ui_triggerPanel.h"
#include "mainwin.h"

class triggerPanel : public QDialog
{
	Q_OBJECT


private:

	MainWin* mainWin_ = nullptr;

	SV_Cng::eventType currCondition_ = SV_Cng::eventType::less;

	QString selModule_, selSignal_, selTrigg_, selDirProc_;

	bool isSelModule_ = true;

	int ctriggerCnt_ = 0, cTriggRow_ = 0;

	void load();

	void showEvent(QShowEvent * event);
	
	void enaBtnCondition(bool ena);

	void updateTableTrigger();
	void updateTableSignal();
	void updateStateSignal();

public:
	Ui::triggerPanelClass ui;

	triggerPanel(QWidget *parent = 0);
	~triggerPanel();

	void updateWin();

private slots:
	void selModule(QListWidgetItem * item);
	void selSignal(QTableWidgetItem * item);
	void selTrigger(QTableWidgetItem * item);
	void addTrigger();
	void delTrigger();
	void changeTrigger();
	void paramChange();
	void selCondition(SV_Cng::eventType);
	void selDirProc();
};


