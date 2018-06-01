
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


