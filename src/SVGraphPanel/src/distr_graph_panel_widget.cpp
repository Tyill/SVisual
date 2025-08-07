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

#include "forms/graph_panel_widget.h"
#include "SVBase/base.h"
#include "SVBase/sv_limits.h"
#include "drag_label.h"
#include "forms/graph_widget.h"

#include <QtGui>
#include <QScrollBar>
#include <QTreeWidget>
#include <QTranslator>

using namespace SV_Base;


DistrGraphPanelWidget::DistrGraphPanelWidget(QWidget *parent, const SV_Graph::Config& cng_):
    GraphPanelWidget(parent, cng_)
{

}

DistrGraphPanelWidget::~DistrGraphPanelWidget() {}

void DistrGraphPanelWidget::dropEvent(QDropEvent *event)
{
  if (qobject_cast<QTreeWidget *>(event->source())) {

    QString sign = event->mimeData()->text();

    addSignalOnGraph(sign, 0);

    event->accept();
  }
}

void DistrGraphPanelWidget::addSignalOnGraph(const QString& sign, int /*section*/)
{
    if (!sign.isEmpty()) {

      auto sd = pfGetSignalData(sign);

      if (sd && !sd->isBuffEnable && pfLoadSignalData) {
          pfLoadSignalData(sign);
      }
      if (graphObj_.isEmpty()) {
        uint64_t buffMinTime, buffMaxTime;
        {LockerReadSDataGraph lock;
            buffMinTime = sd->buffMinTime;
            buffMaxTime = sd->buffMaxTime;
        }
        ui.axisTime->setTimeInterval(buffMinTime, buffMaxTime);

        addGraph(sign);

        emit ui.axisTime->req_axisChange();
      }
      else if (!sign.isEmpty()){
        for (auto g : graphObj_){
          if (auto sd = pfGetSignalData(sign); sd){
            g->addSignal(sd);
          }
        }
      }
    }
}

void DistrGraphPanelWidget::addGraph(const QString& sign) {

  const auto tintlMax = ui.axisTime->getTimeInterval();
  const auto count = qBound<int>(1, (tintlMax.second - tintlMax.first) / (ui.spnPeriodMin->value() * 60 * 1000), 1000);
  for (int iGr = 0; iGr < count; ++iGr){
      GraphWidget* graph = new GraphWidget(this, cng);

      graph->pfGetSignalData = pfGetSignalData;
      graph->pfLoadSignalData = pfLoadSignalData;
      graph->pfGetSignalAttr = pfGetSignalAttr;

      graph->setObjectName("graph_" + QString::number(graphObj_.size()));

      graph->setGraphSetting(graphSett_);

      graphObj_.append(graph);

      splitterGraph_->addWidget(graph);

      ui.scrollAreaWidgetContents->setMinimumHeight(graphObj_.size() * MIN_HEIGHT_GRAPH);

      QScrollBar* vscr = ui.scrollArea->verticalScrollBar();
      vscr->setValue(vscr->maximumHeight());

      AxisTimeProxy* atp = new AxisTimeProxy(this);{
          atp->pfMouseMoveEvent = [this](QMouseEvent* e){ ui.axisTime->mouseMoveEvent(e);};
          atp->pfMousePressEvent = [this](QMouseEvent* e){ ui.axisTime->mousePressEvent(e);};
          atp->pfWheelEvent = [this](QWheelEvent* e){ ui.axisTime->wheelEvent(e);};
          atp->pfSetTimeIntervalCBack = [this, iGr](qint64 l, qint64 r){
              auto offset = (r - l) * iGr;
              ui.axisTime->setTimeInterval(l - offset, r - offset);
          };
          atp->pfGetTimeIntervalCBack = [this, iGr]()->QPair<qint64, qint64>{
              auto tintl = ui.axisTime->getTimeInterval();
              auto offset = (tintl.second - tintl.first) * iGr;
              return QPair(tintl.first - offset, tintl.second - offset);
          };
          atp->pfGetTimeScaleCBack = [this](){return ui.axisTime->getTimeScale();};
          atp->pfGetAxisMarkCBack = [this](){return ui.axisTime->getAxisMark();};
          atp->pfScaleCBack = [this](int delta, int mpos){ ui.axisTime->scale(delta, mpos);};
          atp->pfUpdate = [this](){ ui.axisTime->update();};
          atp->pfWidthCBack = [this](){return ui.axisTime->width();};
      }
      graph->setAxisTime(atp);

      if (!sign.isEmpty()){
        if (auto sd = pfGetSignalData(sign); sd){
          graph->addSignal(sd);
        }
      }
      connect(ui.axisTime, SIGNAL(req_axisChange()), this, SLOT(diapTimeUpdate()));
      connect(ui.axisTime, SIGNAL(req_axisChange()), graph, SLOT(axisTimeChange()));
      connect(graph, SIGNAL(req_axisTimeUpdate(QString)), this, SLOT(axisTimeChange(QString)));
      connect(graph, SIGNAL(req_markerChange(QString)), this, SLOT(markerChange(QString)));
      connect(graph, SIGNAL(req_selectGraph(QString)), this, SLOT(selectGraph(QString)));
      connect(graph, SIGNAL(req_graphUp(QString)), this, SLOT(graphToUp(QString)));
      connect(graph, SIGNAL(req_graphDn(QString)), this, SLOT(graphToDn(QString)));
      connect(graph, SIGNAL(req_close()), this, SLOT(closeGraph()));

      QPoint leftMarkPos, rightMarkPos;
      graphObj_.at(0)->getMarkersPos(leftMarkPos, rightMarkPos);
      graph->setMarkersPos(leftMarkPos, rightMarkPos);
  }
  tableUpdate();
  tableUpdateAlter();

  if (!graphObj_.isEmpty()){
    selGraph_ = graphObj_.at(0);
  }
}
