#pragma once

#include "src/stdafx.h"
#include "SVConfig/SVConfigData.h"
#include "SVGraphPanel/SVGraphPanel.h"
#include "src/wdgAxisTime.h"
#include "src/wdgAxisValue.h"
#include "src/wdgMarker.h"
#include "SVGraphPanel/forms/ui_wdgGraph.h"
#include "SVGraphPanel/forms/graphPanel.h"

class wdgGraph : public QWidget
{
	Q_OBJECT

private:
	
	struct graphSignData{
		QString sign;
		QString name;
		SV_Cng::valueType type;
		int num;
		QColor color;
		QLabel* lb;
		QLabel* lbLeftMarkVal;
		QLabel* lbRightMarkVal;
		QVector<QVector<QPair<int, int>>> pnts;
	};

	struct histPos{
		QPair<double, double> valIntl;
		QPair<qint64, qint64> tmIntl;
	};

	QImage imSign_;

	bool repaintEna_ = false, selLeftMark_ = false, selRigthMark_ = false;

	int colorCnt_ = 0;
		
	QMap <QString, graphSignData> signals_, signalsAlter_;
	QStringList signalList_, signalListAlter_;
		
	QVector<QVector<QPair<int, int>>> getSignalPnt(SV_Cng::signalData* sign, bool isAlter = false);
	QPair<double, double > getSignMaxMinValue(graphSignData* sign);
	QPair<double, double> getSignMaxMinValue(SV_Cng::signalData* sign, QPair<qint64, qint64>& tmInterval);
	void addPosToHistory();

	wdgAxisTime* axisTime_ = NULL;
	
	wdgMarker* leftMarker_ = NULL;
	wdgMarker* rightMarker_ = NULL;
	
	bool eventFilter(QObject *target, QEvent *event);

	QVector<histPos> historyPos_;
	graphPanel* grPanel_ = nullptr;
    SV_Graph::config cng;

    void paintSignals();
	void paintSignalsAlter();
	void paintObjects();
	void paintObjectsAlter();
	
public:
	Ui::wdgGraphClass ui;

	struct graphSignPoint{
		int xPix, yPix;
		double val;
		QString sign;
		QString name;
		SV_Cng::valueType type;
		QColor color;
	};


	wdgGraph(QWidget *parent, SV_Graph::config cng_);
	~wdgGraph();
	
	QPainter plotPainterMem_;

	void setAxisTime(wdgAxisTime* axisTime);
	void plotUpdate();
	
	void setMarkersPos(QPoint left, QPoint right);
	void getMarkersPos(QPoint& left, QPoint& right);
	QVector<graphSignPoint> getSignalValueByMarkerPos(int pos);
	QVector<wdgGraph::graphSignPoint> getSignalAlterValueByMarkerPos(int pos);
	void addSignal(QString sign);
	void addAlterSignal(QString sign);
	QStringList getAllSignals();
	QStringList getAllAlterSignals();
	QSize sizeHint();
	void scale(bool posNeg);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void resizeEvent(QResizeEvent * event);

public slots:
	void axisValueChange();
	void axisTimeChange();
	void delSignal(QString sign, bool isLabelSender = true);
	void delSignalAlter(QString sign, bool isLabelSender = true);
	void resizeByTime();
	void resizeByValue();
	void resizeByRect();
	void showMarkPos();
	void updateByMarker();
	void undoCmd();
	void colorUpdate();

signals:
	void req_axisTimeUpdate(QString name);
	void req_markerChange(QString name);
	void req_selectGraph(QString name);
	void req_graphUp(QString name);
	void req_graphDn(QString name);
	void req_close();
};