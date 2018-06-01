
#pragma once

#include "src/stdafx.h"
#include "SVGraphPanel/SVGraphPanel.h"
#include "SVGraphPanel/forms/ui_graphPanel.h"

class wdgGraph;

class graphPanel : public QWidget
{
	Q_OBJECT


private:

	SV_Graph::config cng;

	const int MIN_HEIGHT_GRAPH = 300;
	int graphCnt_ = 0;
	QVector<wdgGraph*> graphObj_;

	QSplitter* splitterGraph_ = nullptr;
	wdgGraph* selGraph_ = nullptr;

	bool isPlay_ = true;
		
	void load();
	void tableUpdate(wdgGraph* graph);
	void tableUpdateAlter(wdgGraph* graph);

public:
	Ui::graphPanelClass ui;

	graphPanel(QWidget *parent, SV_Graph::config cng);
	~graphPanel();

	SV_Graph::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
	SV_Graph::pf_getSignalData pfGetSignalData = nullptr;
	SV_Graph::pf_loadSignalData pfLoadSignalData= nullptr;

	void addSignalOnGraph(QString name);
	QPair<qint64, qint64> getTimeInterval();
	void setTimeInterval(qint64 stTime, qint64 enTime);
	
public slots:
	void updateSignals();
	void resizeByValue();
	void delSignal(QString sign);
	void scaleGraph();

private slots:
	void addGraph(QString sign);
	void axisTimeChange(QString obj);
	void diapTimeUpdate();
	void markerChange(QString obj);
	void selectGraph(QString obj);	
	void graphToUp(QString obj);
	void graphToDn(QString obj);
	void closeGraph();
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void resizeByTime();		
	void undoCmd();
	void colorUpdate();
	
};


