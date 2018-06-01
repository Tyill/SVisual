#pragma once

#include "src/stdafx.h"
#include "forms/ui_eventOrderWin.h"
#include "mainwin.h"

class eventOrderWin : public QDialog
{
	Q_OBJECT

public:
	eventOrderWin(QWidget *parent = 0);
	~eventOrderWin();

	Ui::eventOrderClass ui;

private:
	void showEvent(QShowEvent * event);

	MainWin* mainWin_ = nullptr;

public slots:
	void showOrder();
};
