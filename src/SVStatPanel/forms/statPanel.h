
#pragma once

#include "src/stdafx.h"
#include "ui_statPanel.h"
#include "SVConfig/SVConfigData.h"
#include "SVStatPanel.h"

class statPanel : public QDialog
{
	Q_OBJECT

public:

	SV_Stat::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
	SV_Stat::pf_getSignalData pfGetSignalData = nullptr;
	SV_Stat::pf_loadSignalData pfLoadSignalData= nullptr;
	SV_Stat::pf_setTimeInterval pfSetTimeInterval = nullptr;
	SV_Stat::pf_getTimeInterval pfGetTimeInterval = nullptr;

	statPanel(QWidget *parent, SV_Stat::config);
	~statPanel();
	
private:
	Ui::StatPanelClass ui;
	
	SV_Stat::config cng;

	struct valSData{
		int changeCnt;
		int duration;
		int durationMean;
	};

	struct graphSignData{
		QString sign;
		QString name;	
		QMap<int, valSData> valData;
		QVector<QPair<int, int>> hist;
	};

			
	bool diapEna_ = false;

	QMap<QString, graphSignData> sign_;
	QVector<SV_Cng::recData> vars_;

	QVector<SV_Cng::recData> getSignData(QString sign);
	void listTimeUpdate(QString sname, int val);
	QVector<QPair<int, int>> calcHist(QString sname);

	// Mx Hist
	int Mx(QVector<QPair<int, int>>& hist);
	
private slots:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void graphValueChange();
	void selectSignalChange();
	void selectSignalTime(int row);
	
};


