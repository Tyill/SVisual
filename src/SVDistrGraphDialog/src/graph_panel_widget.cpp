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

#include "src/graph_panel_widget.h"
#include "SVBase/base.h"
#include "SVBase/sv_limits.h"
#include "forms/graph_widget.h"

#include <QtGui>
#include <QScrollBar>
#include <QTreeWidget>
#include <QTranslator>

using namespace SV_Base;

namespace SV_Distr {

GraphPanelWidget::GraphPanelWidget(QWidget *parent, const SV_Distr::Config& cng_):
    QWidget(parent)
{
#ifdef SV_EN
  QTranslator translator;
  translator.load(":/SVGp/graph_panel_en.qm");
  QCoreApplication::installTranslator(&translator);
#endif

  cng = cng_;

//  ui.setupUi(this);

//  load();

//  ////// хак. не открывается на полную
//  ui.dTimeBegin->setVisible(false);
//  ui.dTimeEnd->setVisible(false);
//  ui.lbDTime->setVisible(false);

//  QTimer* tm = new QTimer(this);
//  connect(tm, &QTimer::timeout, this, [this, tm]() {

//    if (cng.mode == SV_Distr::ModeGr::Viewer) {
//      ui.btnPlay->setVisible(false);
//      ui.btnAScale->setVisible(false);
//      ui.btnAScale->setChecked(false);
//      ui.dTimeBegin->setVisible(true);
//      ui.dTimeEnd->setVisible(true);
//      ui.lbDTime->setVisible(true);
//    }
//    else {
//      ui.dTimeBegin->setVisible(false);
//      ui.dTimeEnd->setVisible(false);
//      ui.lbDTime->setVisible(false);
//    }
//    tm->stop();  tm->deleteLater();
//  });
//  tm->start(100);
  ////////////

}

GraphPanelWidget::~GraphPanelWidget() {}

void GraphPanelWidget::load() {

  setAcceptDrops(true);

  QList<int> ss; ss.append(90); ss.append(cng.isShowTable ? 1 : 0);
  ui.splitter_2->setSizes(ss);

  connect(ui.btnResizeByAuto, &QPushButton::clicked, this, [this]() {
    resizeByTime();
    resizeByValue();
  });
  connect(ui.btnResizeByTime, SIGNAL(clicked()), this, SLOT(resizeByTime()));
  connect(ui.btnResizeByValue, SIGNAL(clicked()), this, SLOT(resizeByValue()));
  connect(ui.btnScalePos, SIGNAL(clicked()), this, SLOT(scaleGraph()));
  connect(ui.btnScaleNeg, SIGNAL(clicked()), this, SLOT(scaleGraph()));
  connect(ui.btnUndo, SIGNAL(clicked()), this, SLOT(undoCmd()));
  connect(ui.btnColorUpdate, SIGNAL(clicked()), this, SLOT(colorUpdate()));

  splitterGraph_ = new QSplitter(this);
  splitterGraph_->setOrientation(Qt::Vertical);

  ui.scrollAreaWidgetContents->layout()->addWidget(splitterGraph_);
  connect(ui.btnPlay, &QPushButton::clicked, this, [this] {
    isPlay_ = !isPlay_;
    if (isPlay_)
      ui.btnPlay->setIcon(QIcon(":/SVGp/images/player_stop.png"));
    else
      ui.btnPlay->setIcon(QIcon(":/SVGp/images/player_play.png"));
  });

  ui.dTimeBegin->setDisplayFormat("dd.MM.yy hh:mm:ss");
  ui.dTimeEnd->setDisplayFormat("dd.MM.yy hh:mm:ss");
  ui.dTimeBegin->setDateTime(QDateTime::currentDateTime());
  ui.dTimeEnd->setDateTime(QDateTime::currentDateTime());

  auto header = ui.tblValues->horizontalHeader();
  header->setSortIndicatorShown(true);
  ui.tblValues->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

  connect(header, &QHeaderView::sortIndicatorChanged, this, [this](int, Qt::SortOrder) {
      if (selGraph_) {
          tableUpdate(selGraph_);
          tableUpdateAlter(selGraph_);
      }
  });
}

void GraphPanelWidget::setGraphSetting(const SV_Distr::GraphSetting& gs) {

  graphSett_ = gs;

  for (auto ob : qAsConst(graphObj_)) {

    ob->setGraphSetting(gs);
    ob->plotUpdate();
  }
}

void GraphPanelWidget::addSignalOnGraph(QString sign, int section) {

  SV_Base::SignalData* sd = pfGetSignalData(sign);

  if (!sd) return;

  if (sd && !sd->isBuffEnable && pfLoadSignalData)
    pfLoadSignalData(sign);

  if (graphObj_.size() <= section) {
    while (graphObj_.size() <= section) {
        addGraph("");
    }
    uint64_t buffMinTime, buffMaxTime;
    {LockerReadSDataGraph lock;
        buffMinTime = sd->buffMinTime;
        buffMaxTime = sd->buffMaxTime;
    }
    ui.axisTime->setTimeInterval(buffMinTime, buffMaxTime);
    if (buffMinTime == buffMaxTime) {
        ui.axisTime->setTimeInterval(buffMinTime, buffMinTime + 1000);
    }
    emit ui.axisTime->req_axisChange();
  }

  if (selGraph_) {
    selGraph_->addSignal(sign);

    tableUpdate(selGraph_);
    tableUpdateAlter(selGraph_);
  }
  else {
    graphObj_[section]->addSignal(sign);

    tableUpdate(graphObj_[section]);
    tableUpdateAlter(graphObj_[section]);

    selGraph_ = graphObj_[section];
  }

}

void GraphPanelWidget::addGraph(QString sign) {

  GraphWidget* graph = new GraphWidget(this, cng);

  graph->setObjectName("graph_" + QString::number(graphCnt_));
  ++graphCnt_;

  graph->setGraphSetting(graphSett_);

  graphObj_.append(graph);

  splitterGraph_->addWidget(graph);

  ui.scrollAreaWidgetContents->setMinimumHeight(graphObj_.size() * MIN_HEIGHT_GRAPH);

  QScrollBar* vscr = ui.scrollArea->verticalScrollBar();
  vscr->setValue(vscr->maximumHeight());

  graph->setAxisTime(ui.axisTime);

  if (!sign.isEmpty())
    graph->addSignal(sign);

  connect(ui.axisTime, SIGNAL(req_axisChange()), this, SLOT(diapTimeUpdate()));
  connect(ui.axisTime, SIGNAL(req_axisChange()), graph, SLOT(axisTimeChange()));
  connect(graph, SIGNAL(req_axisTimeUpdate(QString)), this, SLOT(axisTimeChange(QString)));
  connect(graph, SIGNAL(req_markerChange(QString)), this, SLOT(markerChange(QString)));
  connect(graph, SIGNAL(req_selectGraph(QString)), this, SLOT(selectGraph(QString)));
  connect(graph, SIGNAL(req_graphUp(QString)), this, SLOT(graphToUp(QString)));
  connect(graph, SIGNAL(req_graphDn(QString)), this, SLOT(graphToDn(QString)));
  connect(graph, SIGNAL(req_close()), this, SLOT(closeGraph()));

  if (graphObj_.size() > 1) {

    QPoint leftMarkPos, rightMarkPos;
    graphObj_[0]->getMarkersPos(leftMarkPos, rightMarkPos);
    graph->setMarkersPos(leftMarkPos, rightMarkPos);
  }

  tableUpdate(graph);
  tableUpdateAlter(graph);

  selGraph_ = graph;
}

void GraphPanelWidget::delSignal(QString sign) {

  for (auto gr : qAsConst(graphObj_)) {

    gr->delSignal(sign, false);
  }
  tableUpdate(selGraph_);

}

void GraphPanelWidget::dragEnterEvent(QDragEnterEvent *event)
{
  if (qobject_cast<QTreeWidget *>(event->source()) ||
    qobject_cast<DragLabel *>(event->source())) {

    event->accept();

  }
}

void GraphPanelWidget::dragMoveEvent(QDragMoveEvent *event) {

  if (qobject_cast<QTreeWidget *>(event->source()) ||
    qobject_cast<DragLabel *>(event->source())) {

    event->accept();

  }
}

void GraphPanelWidget::dropEvent(QDropEvent *event)
{
  DragLabel* lb = qobject_cast<DragLabel *>(event->source());

  if (qobject_cast<QTreeWidget *>(event->source()) || lb) {

    QString sign = event->mimeData()->text();

    if (!sign.isEmpty()) {

      auto sd = pfGetSignalData(sign);

      if (sd && !sd->isBuffEnable && pfLoadSignalData) {
          pfLoadSignalData(sign);
      }
      if (lb){
          emit lb->req_delSignal(sign);
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
      else{
        addGraph(sign);
      }
    }

    event->accept();
  }
}

void GraphPanelWidget::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier)){
        auto cells = ui.tblValues->selectedItems();

        QString text;
        int currentRow = 0;
        foreach (const QTableWidgetItem* cell, cells) {
            if (text.length() != 0) {
                if (cell->row() != currentRow) {
                    text += '\n';
                } else {
                    text += '\t';
                }
            }
            currentRow = cell->row();
            text += cell->text();
        }

        QApplication::clipboard()->setText(text);
    }
}

void GraphPanelWidget::tableUpdate(GraphWidget* graph) {

  QPoint leftMarkPos, rightMarkPos;
  graph->getMarkersPos(leftMarkPos, rightMarkPos);

  ui.tblValues->clearContents();
  
  int leftMarkP = leftMarkPos.x(),
    rightMarkP = rightMarkPos.x();

  QPair<qint64, qint64> tmInterv = ui.axisTime->getTimeInterval();
  double tmScale = ui.axisTime->getTimeScale();

  QVector<GraphWidget::GraphSignPoint> leftMarkVal = graph->getSignalValueByMarkerPos(leftMarkP);
  QVector<GraphWidget::GraphSignPoint> rightMarkVal = graph->getSignalValueByMarkerPos(rightMarkP);

  QVector<GraphWidget::GraphSignStat> statVal = graph->getStatParams(leftMarkP, rightMarkP);

  QString x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2_x1 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale - leftMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");
  if (leftMarkP > rightMarkP) x2_x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale - rightMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");

  int vCount = leftMarkVal.size();
  while (vCount > ui.tblValues->rowCount()) {
    ui.tblValues->insertRow(ui.tblValues->rowCount());
  }
  
  for (int i = 0; i < vCount; ++i) {

    ValueType vt = leftMarkVal[i].type;

    QString y1 = getSValue(vt, leftMarkVal[i].val).c_str();
    QString y2 = getSValue(vt, rightMarkVal[i].val).c_str();
    QString y2_y1 = getSValue(vt, rightMarkVal[i].val - leftMarkVal[i].val).c_str();
    QString vmin = getSValue(vt, statVal[i].vmin).c_str();
    QString vmax = getSValue(vt, statVal[i].vmax).c_str();
    QString vmean = getSValue(vt, statVal[i].vmean).c_str();

    if (vt == ValueType::BOOL) {
        vmean = QString::number(int(statVal[i].vmean + 0.5));
    }

    QTableWidgetItem* nameItem = new QTableWidgetItem(leftMarkVal[i].name);
    nameItem->setForeground(leftMarkVal[i].color);
    ui.tblValues->setItem(i, 0, nameItem);

    ui.tblValues->setItem(i, 1, new TableWidgetItem(getSVTypeStr(vt).c_str()));
    ui.tblValues->setItem(i, 2, new TableWidgetItem(x1));
    ui.tblValues->setItem(i, 3, new TableWidgetItem(x2));
    ui.tblValues->setItem(i, 4, new TableWidgetItem(x2_x1));
    ui.tblValues->setItem(i, 5, new TableWidgetItem(y1));
    ui.tblValues->setItem(i, 6, new TableWidgetItem(y2));
    ui.tblValues->setItem(i, 7, new TableWidgetItem(y2_y1));
    ui.tblValues->setItem(i, 8, new TableWidgetItem(vmin));
    ui.tblValues->setItem(i, 9, new TableWidgetItem(vmean));
    ui.tblValues->setItem(i, 10, new TableWidgetItem(vmax));
  }
  int sortSection = ui.tblValues->horizontalHeader()->sortIndicatorSection();
  auto sortOrder = ui.tblValues->horizontalHeader()->sortIndicatorOrder();
  ui.tblValues->sortByColumn(sortSection, sortOrder);
  ui.tblValues->resizeColumnsToContents();

}

void GraphPanelWidget::tableUpdateAlter(GraphWidget* graph) {

  if (graph->getAllAlterSignals().isEmpty()) return;

  QPoint leftMarkPos, rightMarkPos;
  graph->getMarkersPos(leftMarkPos, rightMarkPos);

  int leftMarkP = leftMarkPos.x(), rightMarkP = rightMarkPos.x();

  QPair<qint64, qint64> tmInterv = ui.axisTime->getTimeInterval();
  double tmScale = ui.axisTime->getTimeScale();

  QVector<GraphWidget::GraphSignPoint> leftMarkVal = graph->getSignalAlterValueByMarkerPos(leftMarkP);
  QVector<GraphWidget::GraphSignPoint> rightMarkVal = graph->getSignalAlterValueByMarkerPos(rightMarkP);

  QVector<GraphWidget::GraphSignStat> statVal = graph->getStatAlterParams(leftMarkP, rightMarkP);

  QString x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2_x1 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale - leftMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");
  if (rightMarkP < leftMarkP) {
      x2_x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale - rightMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");
  }
  int vCount = leftMarkVal.size();
  while (vCount > ui.tblValues->rowCount()) {
    ui.tblValues->insertRow(ui.tblValues->rowCount());
  }

  int offs = graph->getAllSignals().size();
  for (int i = 0; i < vCount; ++i) {

    ValueType vt = leftMarkVal[i].type;

    QString y1 = getSValue(vt, leftMarkVal[i].val).c_str();
    QString y2 = getSValue(vt, rightMarkVal[i].val).c_str();
    QString y2_y1 = getSValue(vt, rightMarkVal[i].val - leftMarkVal[i].val).c_str();
    QString vmin = getSValue(vt, statVal[i].vmin).c_str();
    QString vmax = getSValue(vt, statVal[i].vmax).c_str();
    QString vmean = getSValue(vt, statVal[i].vmean).c_str();

    if (vt == ValueType::BOOL) {
        vmean = QString::number(int(statVal[i].vmean + 0.5));
    }
    QTableWidgetItem* nameItem = new QTableWidgetItem(leftMarkVal[i].name);
    nameItem->setForeground(leftMarkVal[i].color);
    ui.tblValues->setItem(offs + i, 0, nameItem);

    ui.tblValues->setItem(offs + i, 1, new TableWidgetItem(getSVTypeStr(vt).c_str()));
    ui.tblValues->setItem(offs + i, 2, new TableWidgetItem(x1));
    ui.tblValues->setItem(offs + i, 3, new TableWidgetItem(x2));
    ui.tblValues->setItem(offs + i, 4, new TableWidgetItem(x2_x1));
    ui.tblValues->setItem(offs + i, 5, new TableWidgetItem(y1));
    ui.tblValues->setItem(offs + i, 6, new TableWidgetItem(y2));
    ui.tblValues->setItem(offs + i, 7, new TableWidgetItem(y2_y1));
    ui.tblValues->setItem(offs + i, 8, new TableWidgetItem(vmin));
    ui.tblValues->setItem(offs + i, 9, new TableWidgetItem(vmean));
    ui.tblValues->setItem(offs + i, 10, new TableWidgetItem(vmax));
  }
  int sortSection = ui.tblValues->horizontalHeader()->sortIndicatorSection();
  auto sortOrder = ui.tblValues->horizontalHeader()->sortIndicatorOrder();
  ui.tblValues->sortByColumn(sortSection, sortOrder);
  ui.tblValues->resizeColumnsToContents();
}

void GraphPanelWidget::diapTimeUpdate() {

  QPair<qint64, qint64> tIntl = ui.axisTime->getTimeInterval();

  ui.dTimeBegin->setDateTime(QDateTime::fromMSecsSinceEpoch(tIntl.first));
  ui.dTimeEnd->setDateTime(QDateTime::fromMSecsSinceEpoch(tIntl.second));

  if (selGraph_) {
    tableUpdate(selGraph_);
    tableUpdateAlter(selGraph_);
  }
}

void GraphPanelWidget::axisTimeChange(QString obj) {

  diapTimeUpdate();

  for (auto ob : qAsConst(graphObj_)) {

    if (ob->objectName() != obj)
      ob->plotUpdate();
  }
}

void GraphPanelWidget::markerChange(QString obj) {

  GraphWidget* graph = qobject_cast<GraphWidget*>(sender());

  if (!graph) return;

  tableUpdate(graph);
  tableUpdateAlter(graph);

  QPoint leftMarkPos, rightMarkPos;
  graph->getMarkersPos(leftMarkPos, rightMarkPos);

  for (auto ob : qAsConst(graphObj_)) {

    if (ob->objectName() != obj) {
      ob->setMarkersPos(leftMarkPos, rightMarkPos);
      ob->ui.plot->update();
    }
  }
}

void GraphPanelWidget::selectGraph(QString obj) {

  for (auto ob : qAsConst(graphObj_)) {

    if (ob->objectName() == obj) {
      selGraph_ = ob;
      break;
    }
  }

  markerChange(obj);
}

void GraphPanelWidget::closeGraph() {

  GraphWidget* obj = qobject_cast<GraphWidget*>(sender());

  if (obj) {
    int sz = graphObj_.size();
    for (int i = 0; i < sz; ++i) {
      if (graphObj_[i]->objectName() == obj->objectName()) {

        graphObj_.remove(i);
        sz--;
        break;
      }
    }

    if (obj == selGraph_) {
      if (sz > 1) selectGraph(graphObj_[0]->objectName());
      else selGraph_ = nullptr;
    }
    obj->deleteLater();
    ui.scrollAreaWidgetContents->setMinimumHeight(sz * MIN_HEIGHT_GRAPH);
  }

}

void GraphPanelWidget::resizeByTime() {

  if (selGraph_) selGraph_->resizeByTime();

}

void GraphPanelWidget::resizeByValue() {

  if (selGraph_) {
    selGraph_->resizeByValue();
  }
}

void GraphPanelWidget::scaleGraph() {

  if (selGraph_) {

    if (sender()->objectName() == "btnScalePos")
      selGraph_->scale(true, QPoint(selGraph_->width()/2, selGraph_->height()/2));
    else
      selGraph_->scale(false, QPoint(selGraph_->width()/2, selGraph_->height()/2));
  }
}

void GraphPanelWidget::undoCmd() {

  if (selGraph_) selGraph_->undoCmd();
}

void GraphPanelWidget::colorUpdate() {

  if (selGraph_) selGraph_->colorUpdate();
}

void GraphPanelWidget::updateSignals() {

  if (graphObj_.isEmpty() || !isPlay_) return;

  if (ui.btnAScale->isChecked()) {
    for (auto ob : qAsConst(graphObj_))
      ob->resizeByValue();
  }

  if (cng.mode == SV_Distr::ModeGr::Player) {

    qint64 bTm = QDateTime::currentDateTime().toMSecsSinceEpoch() - SV_CYCLESAVE_MS;

    QPair<qint64, qint64> tmIntl = ui.axisTime->getTimeInterval();

    ui.axisTime->setTimeInterval(bTm - (tmIntl.second - tmIntl.first), bTm);

    ui.axisTime->update();
  }

  for (auto ob : qAsConst(graphObj_)){
    ob->plotUpdate();
  }

  if (selGraph_) {
    tableUpdate(selGraph_);
    tableUpdateAlter(selGraph_);
  }
}

void GraphPanelWidget::graphToDn(QString obj) {

  GraphWidget* graph = qobject_cast<GraphWidget*>(sender());

  if (!graph) return;

  for (auto ob : qAsConst(graphObj_)) {

    if (ob->objectName() == obj) {

      int ind = splitterGraph_->indexOf(ob);

      splitterGraph_->insertWidget(ind - 1, graph);

      QScrollBar* vscr = ui.scrollArea->verticalScrollBar();
      vscr->setValue(graph->pos().y());
      break;
    }
  }


}

void GraphPanelWidget::graphToUp(QString obj) {

  GraphWidget* graph = qobject_cast<GraphWidget*>(sender());

  if (!graph) return;

  int sz = graphObj_.size();
  for (auto ob : qAsConst(graphObj_)) {

    if (ob->objectName() == obj) {

      int ind = splitterGraph_->indexOf(ob);

      splitterGraph_->insertWidget(ind + 1 >= sz ? 0 : ind + 1, graph);
      QScrollBar* vscr = ui.scrollArea->verticalScrollBar();
      vscr->setValue(graph->pos().y());
      break;
    }
  }
}

QPair<qint64, qint64> GraphPanelWidget::getTimeInterval() {

  return ui.axisTime->getTimeInterval();

}

void GraphPanelWidget::setTimeInterval(qint64 stTime, qint64 enTime) {

  ui.axisTime->setTimeInterval(stTime, enTime);

}

QVector<QVector<QString>> GraphPanelWidget::getLocateSignals() {

  QVector<QVector<QString>> res;
  for (auto ob : qAsConst(graphObj_)) {

    int ind = splitterGraph_->indexOf(ob);

    if (ind >= res.size())
      res.resize(ind + 1);

    QStringList signs = ob->getAllSignals();
    signs.append(ob->getAllAlterSignals());

    for (auto& s : signs)
      res[ind].append(s);
  }

  return res;
}
}