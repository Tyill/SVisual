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
#include "SVGraphPanel/SVGraphPanel.h"
#include "src/wdgAxisTime.h"
#include "src/wdgAxisValue.h"
#include "src/wdgMarker.h"
#include "SVGraphPanel/forms/ui_wdgGraph.h"
#include "SVGraphPanel/forms/graphPanel.h"
#include "SVGraphPanel/forms/axisSettingPanel.h"

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
        SV_Cng::signalData* sdata;
		QVector<QVector<QPair<int, int>>> pnts;
	};

	struct histPos{
		QPair<double, double> valIntl;
		QPair<qint64, qint64> tmIntl;
	};

	QImage imSign_;

	bool repaintEna_ = false,
         selLeftMark_ = false, selRigthMark_ = false;

	int colorCnt_ = 30;

    SV_Graph::graphSetting graphSetting_;
		
	QMap <QString, graphSignData> signals_, signalsAlter_;
	QStringList signalList_, signalListAlter_;
		
	QVector<QVector<QPair<int, int>>> getSignalPnts(SV_Cng::signalData* sign, bool isAlter = false);

	QPair<double, double > getSignPntsMaxMinValue(const graphSignData& sign);
	QPair<double, double> getSignMaxMinValue(SV_Cng::signalData* sign, QPair<qint64, qint64>& tmInterval);
	void addPosToHistory();

	wdgAxisTime* axisTime_ = nullptr;
	
	wdgMarker* leftMarker_ = nullptr;
	wdgMarker* rightMarker_ = nullptr;
	
	bool eventFilter(QObject *target, QEvent *event);

	QVector<histPos> historyPos_;
	graphPanel* grPanel_ = nullptr;
    SV_Graph::config cng;

    axisSettingPanel* axisSettPanel_ = nullptr;
    
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

    struct graphSignStat{
        
        double vmin = INT32_MAX,
               vmax = INT32_MIN,
               vmean = 0.0;
    };


	wdgGraph(QWidget *parent, SV_Graph::config cng_);
	~wdgGraph();
	
	void setAxisTime(wdgAxisTime* axisTime);
	
    void setGraphSetting(const SV_Graph::graphSetting&);
    void setSignalAttr(const QString& sign, const SV_Graph::signalAttr& att);

    void setAxisAttr(const SV_Graph::axisAttr& attr);
    SV_Graph::axisAttr getAxisAttr();

	void setMarkersPos(QPoint left, QPoint right);
	void getMarkersPos(QPoint& left, QPoint& right);
	
    QVector<graphSignPoint> getSignalValueByMarkerPos(int pos);
	QVector<wdgGraph::graphSignPoint> getSignalAlterValueByMarkerPos(int pos);
	
    QVector<graphSignStat> getStatParams(int markPosBegin, int markPosEnd);

    QVector<graphSignStat> getStatAlterParams(int markPosBegin, int markPosEnd);

    void addSignal(QString sign);
	void addAlterSignal(QString sign);
	
    QStringList getAllSignals();
	QStringList getAllAlterSignals();
	
    QSize sizeHint();
	void scale(bool posNeg);   
    void plotUpdate();

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void resizeEvent(QResizeEvent * event);
    void keyPressEvent(QKeyEvent * event);

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