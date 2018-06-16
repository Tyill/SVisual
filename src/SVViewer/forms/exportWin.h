#pragma once

#include "src/stdafx.h"
#include "forms/ui_exportWin.h"

class exportWin : public QDialog
{
	Q_OBJECT

public:
	exportWin(QWidget *parent = 0);
	~exportWin();

	Ui::exportWin ui;

private:
	void showEvent(QShowEvent * event);
	
};
