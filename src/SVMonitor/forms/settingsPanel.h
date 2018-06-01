#pragma once

#include "forms/ui_settingsPanel.h"
#include "forms/mainwin.h"

class settingsPanel : public QDialog
{
	Q_OBJECT

public:
	settingsPanel(QWidget *parent = 0);
	~settingsPanel();

	QString selDirCopy_;

	Ui::settingsPanelClass ui;

private:
	
	bool selParamLoad_ = false;
	
	void showEvent(QShowEvent * event);

    MainWin* mainWin_ = nullptr;

private slots:
	void saveChange();
	void selDirCopy();
	void paramChange();

};
