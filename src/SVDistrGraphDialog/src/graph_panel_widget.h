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

#include "SVDistrGraphDialog/distr_dialog.h"

#include <QWidget>

class GraphWidget;
class QSplitter;

namespace SV_Distr {

class GraphPanelWidget : public QWidget
{
  Q_OBJECT

public:

  GraphPanelWidget(QWidget *parent, const SV_Distr::Config& cng);
  ~GraphPanelWidget();

  SV_Distr::getCopySignalRefCBack pfGetCopySignalRef = nullptr;
  SV_Distr::getSignalDataCBack pfGetSignalData = nullptr;
  SV_Distr::isLoadSignalDataCBack pfLoadSignalData = nullptr;
  
  void addSignalOnGraph(QString name, int section);
  QPair<qint64, qint64> getTimeInterval();
  void setTimeInterval(qint64 stTime, qint64 enTime);
  QVector<QVector<QString>> getLocateSignals();

  void setGraphSetting(const SV_Distr::GraphSetting&);

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

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  void load();
  void tableUpdate(GraphWidget* graph);
  void tableUpdateAlter(GraphWidget* graph);

private:
  SV_Distr::Config cng;

  const int MIN_HEIGHT_GRAPH = 300;
  int graphCnt_ = 0;
  QVector<GraphWidget*> graphObj_;

  QSplitter* splitterGraph_ = nullptr;
  GraphWidget* selGraph_ = nullptr;
};

class LockerReadSDataGraph {
public:
    explicit LockerReadSDataGraph();
    ~LockerReadSDataGraph();
};
}
