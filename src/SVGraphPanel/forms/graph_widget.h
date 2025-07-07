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

#include "SVGraphPanel/graph_panel.h"
#include "GeneratedFiles/ui_graph_widget.h"

class QLabel;

class AxisTimeWidget;
class AxisValueWidget;

namespace SV_Graph {

class GraphPanelWidget;
class AxisSettingDialog;
class MarkerWidget;

class GraphWidget : public QWidget
{
  Q_OBJECT

public:
  Ui::GraphWidget ui;

  struct GraphSignPoint {
    int xPix, yPix;
    double val;
    QString sign;
    QString name;
    SV_Base::ValueType type;
    QColor color;
  };

  struct GraphSignStat {

    double vmin = INT32_MAX,
      vmax = INT32_MIN,
      vmean = 0.0;
  };

  SV_Graph::getSignalDataCBack pfGetSignalData = nullptr;
  SV_Graph::isLoadSignalDataCBack pfLoadSignalData = nullptr;
  SV_Graph::getSignalAttrCBack pfGetSignalAttr = nullptr;

  GraphWidget(QWidget *parent, SV_Graph::Config cng_);
  ~GraphWidget();

  void setAxisTime(AxisTimeWidget* axisTime);

  void setGraphSetting(const SV_Graph::GraphSetting&);
  void setSignalAttr(const QString& sign, const SV_Graph::SignalAttributes& att);

  void setAxisAttr(const SV_Graph::AxisAttributes& attr);
  SV_Graph::AxisAttributes getAxisAttr();

  void setMarkersPos(QPoint left, QPoint right);
  void getMarkersPos(QPoint& left, QPoint& right);

  QVector<GraphSignPoint> getSignalValueByMarkerPos(int pos);
  QVector<GraphWidget::GraphSignPoint> getSignalAlterValueByMarkerPos(int pos);

  QVector<GraphSignStat> getStatParams(int markPosBegin, int markPosEnd);

  QVector<GraphSignStat> getStatAlterParams(int markPosBegin, int markPosEnd);

  void addSignal(SV_Base::SignalData* sign);
  void addAlterSignal(SV_Base::SignalData* sign);

  QStringList getAllSignals()const;
  QStringList getAllAlterSignals()const;

  QSize sizeHint();
  void scale(bool posNeg, const QPoint& mpos);
  void plotUpdate();

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

protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void resizeEvent(QResizeEvent * event) override;
  void keyPressEvent(QKeyEvent * event) override;
  bool eventFilter(QObject *target, QEvent *event) override;

private:
  void paintSignals();
  void paintSignalsAlter();
  void paintObjects();
  void paintObjectsAlter();
  void lbSignBoolMove(bool isTop);
  bool getSignalAttributes(const QString& sign, SV_Graph::SignalAttributes&)const;

private:
  struct GraphSignData {
    QString sign;
    QString name;
    SV_Base::ValueType type;
    int num;
    QColor color;
    QLabel* lb;
    QLabel* lbLeftMarkVal;
    QLabel* lbRightMarkVal;
    SV_Base::SignalData* sdata;
    QVector<QVector<QPair<int, int>>> pnts;
  };

  QVector<QVector<QPair<int, int>>> getSignalPnts(SV_Base::SignalData* sign, bool isAlter = false);
  QPair<double, double > getSignPntsMaxMinValue(const GraphSignData& sign);
  QPair<double, double> getSignMaxMinValue(SV_Base::SignalData* sign, QPair<qint64, qint64>& tmInterval);
  void addPosToHistory();

  QImage imSign_;

  bool repaintEna_ = false,
    selLeftMark_ = false, selRigthMark_ = false;

  int colorCnt_ = 30;

  SV_Graph::GraphSetting graphSetting_;

  QMap <QString, GraphSignData> signals_, signalsAlter_;
  QStringList signalList_, signalListAlter_;

  AxisTimeWidget* axisTime_ = nullptr;

  MarkerWidget* leftMarker_ = nullptr;
  MarkerWidget* rightMarker_ = nullptr;

  struct HistPos {
    QPair<double, double> valIntl;
    QPair<qint64, qint64> tmIntl;
  };
  QVector<HistPos> historyPos_;
  SV_Graph::Config cng;

  AxisSettingDialog* axisSettPanel_ = nullptr;

};
}
