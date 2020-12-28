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
#include "stdafx.h"
#include "forms/graphPanel.h"
#include "SVConfig/SVConfigData.h"
#include "SVConfig/SVConfigLimits.h"
#include "dragLabel.h"
#include "forms/wdgGraph.h"

using namespace SV_Base;

void graphPanel::load(){

  setAcceptDrops(true);

  QList<int> ss; ss.append(90); ss.append(cng.isShowTable ? 1 : 0);
  ui.splitter_2->setSizes(ss);

  connect(ui.btnResizeByAuto, &QPushButton::clicked, [this](){
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
}

graphPanel::graphPanel(QWidget *parent, const SV_Graph::config& cng_){

#ifdef SV_EN
  QTranslator translator;
  translator.load(":/SVGp/svgraphpanel_en.qm");
  QCoreApplication::installTranslator(&translator);
#endif

  cng = cng_;

  setParent(parent);

  ui.setupUi(this);
    
  load();

  ////// хак. не открывается на полную
  ui.dTimeBegin->setVisible(false);
  ui.dTimeEnd->setVisible(false);
  ui.lbDTime->setVisible(false);

  QTimer* tm = new QTimer(this);
  connect(tm, &QTimer::timeout, [=]() {

    if (cng.mode == SV_Graph::modeGr::viewer){
      ui.btnPlay->setVisible(false);           
            ui.btnAScale->setVisible(false);
            ui.btnAScale->setChecked(false);
      ui.dTimeBegin->setVisible(true);
      ui.dTimeEnd->setVisible(true);
      ui.lbDTime->setVisible(true);
    }
    else{
      ui.dTimeBegin->setVisible(false);
      ui.dTimeEnd->setVisible(false);
      ui.lbDTime->setVisible(false);
    }
    tm->stop();  tm->deleteLater();
  });
  tm->start(100);  
  ////////////

}

graphPanel::~graphPanel(){}

void graphPanel::setGraphSetting(const SV_Graph::graphSetting& gs){

    graphSett_ = gs;

    for (auto ob : graphObj_){
     
        ob->setGraphSetting(gs);        
        ob->plotUpdate();
    }
}

void graphPanel::setSignalAttr(const QString& sign, const SV_Graph::signalAttr& att){

    for (auto ob : graphObj_){

        ob->setSignalAttr(sign, att);
    }
}

void graphPanel::setAxisAttr(const QVector<SV_Graph::axisAttr>& attr){

    for (auto ob : graphObj_){

        int ind = splitterGraph_->indexOf(ob);

        if (ind < attr.size())
            ob->setAxisAttr(attr[ind]);
    }

}

QVector<SV_Graph::axisAttr> graphPanel::getAxisAttr(){

    QVector<SV_Graph::axisAttr> res;
    for (auto ob : graphObj_){

        int ind = splitterGraph_->indexOf(ob);

        if (ind >= res.size())
            res.resize(ind + 1);

        res[ind] = ob->getAxisAttr();
    }

    return res;
}

void graphPanel::addSignalOnGraph(QString sign, int section){

  SV_Base::SignalData* sd = pfGetSignalData(sign);

    if (!sd) return;

  if (sd && !sd->isBuffEnable && pfLoadSignalData)
    pfLoadSignalData(sign);

    if (graphObj_.size() <= section){
        while (graphObj_.size() <= section)
            addGraph("");

        ui.axisTime->setTimeInterval(sd->buffMinTime, sd->buffMaxTime);
        if (sd->buffMinTime == sd->buffMaxTime)
            ui.axisTime->setTimeInterval(sd->buffMinTime, sd->buffMinTime + 1000);

        ui.axisTime->req_axisChange();
    }
        
  if (selGraph_){
    selGraph_->addSignal(sign);

    tableUpdate(selGraph_);
    tableUpdateAlter(selGraph_);
  }
    else{
        graphObj_[section]->addSignal(sign);
        
        tableUpdate(graphObj_[section]);
        tableUpdateAlter(graphObj_[section]);

        selGraph_ = graphObj_[section];
    }
  
}

void graphPanel::addGraph(QString sign){

  wdgGraph* graph = new wdgGraph(this, cng);
      
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
  
  if (graphObj_.size() > 1){

    QPoint leftMarkPos, rightMarkPos;
    graphObj_[0]->getMarkersPos(leftMarkPos, rightMarkPos);
    graph->setMarkersPos(leftMarkPos, rightMarkPos);
  }

  tableUpdate(graph);
  tableUpdateAlter(graph);

    selGraph_ = graph;
}

void graphPanel::delSignal(QString sign){
    
  for (auto gr : graphObj_){
    
    gr->delSignal(sign, false);
  }
  tableUpdate(selGraph_);

}

void graphPanel::dragEnterEvent(QDragEnterEvent *event)
{
  if (qobject_cast<QTreeWidget *>(event->source()) || 
    qobject_cast<dragLabel *>(event->source())) {
    
    event->accept();

  }    
}

void graphPanel::dragMoveEvent(QDragMoveEvent *event){

  if (qobject_cast<QTreeWidget *>(event->source()) ||
    qobject_cast<dragLabel *>(event->source())) {

    event->accept();

  }
}

void graphPanel::dropEvent(QDropEvent *event)
{
  dragLabel* lb = qobject_cast<dragLabel *>(event->source());

  if (qobject_cast<QTreeWidget *>(event->source()) || lb) {

    QString sign = event->mimeData()->text();
        
    if (!sign.isEmpty()){

      auto sd = pfGetSignalData(sign);

      if (sd && !sd->isBuffEnable && pfLoadSignalData)
        pfLoadSignalData(sign);

      if (lb) lb->req_delSignal(sign);
      if (graphObj_.isEmpty()){

        ui.axisTime->setTimeInterval(sd->buffMinTime, sd->buffMaxTime);
        
        addGraph(sign);

        ui.axisTime->req_axisChange();
      }
      else addGraph(sign);
      
    }

    event->accept();
  }    
}

void graphPanel::tableUpdate(wdgGraph* graph){
  
  QPoint leftMarkPos, rightMarkPos;
  graph->getMarkersPos(leftMarkPos, rightMarkPos);

  ui.tblValues->clearContents();
   
  int leftMarkP = leftMarkPos.x(), 
        rightMarkP = rightMarkPos.x();

  QPair<qint64, qint64> tmInterv = ui.axisTime->getTimeInterval();
  double tmScale = ui.axisTime->getTimeScale();

  QVector<wdgGraph::graphSignPoint> leftMarkVal = graph->getSignalValueByMarkerPos(leftMarkP);
  QVector<wdgGraph::graphSignPoint> rightMarkVal = graph->getSignalValueByMarkerPos(rightMarkP);

    QVector<wdgGraph::graphSignStat> statVal = graph->getStatParams(leftMarkP, rightMarkP);

  QString x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2_x1 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale - leftMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");
  if (leftMarkP > rightMarkP) x2_x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale - rightMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");

  int sz = leftMarkVal.size();
  while (sz > ui.tblValues->rowCount()){
    ui.tblValues->insertRow(ui.tblValues->rowCount());
  }
       
  for (int i = 0; i < sz; ++i){

    ValueType vt = leftMarkVal[i].type;

    QString y1 = getSValue(vt, leftMarkVal[i].val).c_str();
    QString y2 = getSValue(vt, rightMarkVal[i].val).c_str();
    QString y2_y1 = getSValue(vt, rightMarkVal[i].val - leftMarkVal[i].val).c_str();
        QString vmin = getSValue(vt, statVal[i].vmin).c_str();
        QString vmax = getSValue(vt, statVal[i].vmax).c_str();
        QString vmean = getSValue(vt, statVal[i].vmean).c_str();

        if (vt == ValueType::BOOL)
            vmean = QString::number(int(statVal[i].vmean + 0.5));

    QTableWidgetItem* nameItem = new QTableWidgetItem(leftMarkVal[i].name);
    nameItem->setForeground(leftMarkVal[i].color);
    ui.tblValues->setItem(i, 0, nameItem);

    ui.tblValues->setItem(i, 1, new QTableWidgetItem(getSVTypeStr(vt).c_str()));
    ui.tblValues->setItem(i, 2, new QTableWidgetItem(x1));
    ui.tblValues->setItem(i, 3, new QTableWidgetItem(x2));
    ui.tblValues->setItem(i, 4, new QTableWidgetItem(x2_x1));
    ui.tblValues->setItem(i, 5, new QTableWidgetItem(y1));
    ui.tblValues->setItem(i, 6, new QTableWidgetItem(y2));
    ui.tblValues->setItem(i, 7, new QTableWidgetItem(y2_y1));
        ui.tblValues->setItem(i, 8, new QTableWidgetItem(vmin));
        ui.tblValues->setItem(i, 9, new QTableWidgetItem(vmean));
        ui.tblValues->setItem(i, 10, new QTableWidgetItem(vmax));
  }
  ui.tblValues->resizeColumnsToContents();
            
    ui.tblValues->horizontalHeader()->setSortIndicatorShown(true);
    ui.tblValues->sortByColumn(ui.tblValues->horizontalHeader()->sortIndicatorSection(), ui.tblValues->horizontalHeader()->sortIndicatorOrder());
}

void graphPanel::tableUpdateAlter(wdgGraph* graph){

  if (graph->getAllAlterSignals().isEmpty()) return;

  QPoint leftMarkPos, rightMarkPos;
  graph->getMarkersPos(leftMarkPos, rightMarkPos);
    
  int leftMarkP = leftMarkPos.x(), rightMarkP = rightMarkPos.x();

  QPair<qint64, qint64> tmInterv = ui.axisTime->getTimeInterval();
  double tmScale = ui.axisTime->getTimeScale();

  QVector<wdgGraph::graphSignPoint> leftMarkVal = graph->getSignalAlterValueByMarkerPos(leftMarkP);
  QVector<wdgGraph::graphSignPoint> rightMarkVal = graph->getSignalAlterValueByMarkerPos(rightMarkP);

    QVector<wdgGraph::graphSignStat> statVal = graph->getStatAlterParams(leftMarkP, rightMarkP);

  QString x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale + tmInterv.first).toString("dd.MM.yy hh:mm:ss:zzz");
  QString x2_x1 = QDateTime::fromMSecsSinceEpoch(rightMarkP * tmScale - leftMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");
  if (rightMarkP < leftMarkP) x2_x1 = QDateTime::fromMSecsSinceEpoch(leftMarkP * tmScale - rightMarkP * tmScale).toUTC().toString("hh:mm:ss:zzz");

  int sz = leftMarkVal.size();
  while (sz > ui.tblValues->rowCount()){
    ui.tblValues->insertRow(ui.tblValues->rowCount());
  }

  int offs = graph->getAllSignals().size();
  for (int i = 0; i < sz; ++i){

    ValueType vt = leftMarkVal[i].type;

    QString y1 = getSValue(vt, leftMarkVal[i].val).c_str();
    QString y2 = getSValue(vt, rightMarkVal[i].val).c_str();
    QString y2_y1 = getSValue(vt, rightMarkVal[i].val - leftMarkVal[i].val).c_str();
        QString vmin = getSValue(vt, statVal[i].vmin).c_str();
        QString vmax = getSValue(vt, statVal[i].vmax).c_str();
        QString vmean = getSValue(vt, statVal[i].vmean).c_str();
       
        if (vt == ValueType::BOOL)
            vmean = QString::number(int(statVal[i].vmean + 0.5));

    QTableWidgetItem* nameItem = new QTableWidgetItem(leftMarkVal[i].name);
    nameItem->setForeground(leftMarkVal[i].color);
        ui.tblValues->setItem(offs + i, 0, nameItem);
        
    ui.tblValues->setItem(offs + i, 1, new QTableWidgetItem(getSVTypeStr(vt).c_str()));
    ui.tblValues->setItem(offs + i, 2, new QTableWidgetItem(x1));
    ui.tblValues->setItem(offs + i, 3, new QTableWidgetItem(x2));
    ui.tblValues->setItem(offs + i, 4, new QTableWidgetItem(x2_x1));
    ui.tblValues->setItem(offs + i, 5, new QTableWidgetItem(y1));
    ui.tblValues->setItem(offs + i, 6, new QTableWidgetItem(y2));
    ui.tblValues->setItem(offs + i, 7, new QTableWidgetItem(y2_y1));
        ui.tblValues->setItem(offs + i, 8, new QTableWidgetItem(vmin));
        ui.tblValues->setItem(offs + i, 9, new QTableWidgetItem(vmean));
        ui.tblValues->setItem(offs + i, 10, new QTableWidgetItem(vmax));
  }
  ui.tblValues->resizeColumnsToContents();
    
    ui.tblValues->horizontalHeader()->setSortIndicatorShown(true);
    ui.tblValues->sortByColumn(ui.tblValues->horizontalHeader()->sortIndicatorSection(), ui.tblValues->horizontalHeader()->sortIndicatorOrder());
}

void graphPanel::diapTimeUpdate(){

  QPair<qint64, qint64> tIntl = ui.axisTime->getTimeInterval();

  ui.dTimeBegin->setDateTime(QDateTime::fromMSecsSinceEpoch(tIntl.first));
  ui.dTimeEnd->setDateTime(QDateTime::fromMSecsSinceEpoch(tIntl.second));
    
    if (selGraph_){
        tableUpdate(selGraph_);
        tableUpdateAlter(selGraph_);
    }
}

void graphPanel::axisTimeChange(QString obj){
    
  diapTimeUpdate();

  for (auto ob : graphObj_){

    if (ob->objectName() != obj) 
      ob->plotUpdate();
  }
}

void graphPanel::markerChange(QString obj){

  wdgGraph* graph = qobject_cast<wdgGraph*>(sender());

  if (!graph) return;
    
  tableUpdate(graph);
  tableUpdateAlter(graph);

  QPoint leftMarkPos, rightMarkPos;
  graph->getMarkersPos(leftMarkPos, rightMarkPos);
  
  for (auto ob : graphObj_){

    if (ob->objectName() != obj){
      ob->setMarkersPos(leftMarkPos, rightMarkPos);
      ob->ui.wPlot->update();
    }
  }
}

void graphPanel::selectGraph(QString obj){
  
    for (auto ob : graphObj_){

        if (ob->objectName() == obj){
            selGraph_ = ob;
            break;
        }
    }

  markerChange(obj);
}

void graphPanel::closeGraph(){

  wdgGraph* obj = qobject_cast<wdgGraph*>(sender());
  
  if (obj){
    int sz = graphObj_.size();
    for (int i = 0; i < sz; ++i){
      if (graphObj_[i]->objectName() == obj->objectName()){
                
        graphObj_.remove(i);
        sz--;
        break;
      }
    }

    if (obj == selGraph_){
      if (sz > 1) selectGraph(graphObj_[0]->objectName());
      else selGraph_ = nullptr;
    }    
    obj->deleteLater();
    ui.scrollAreaWidgetContents->setMinimumHeight(sz * MIN_HEIGHT_GRAPH);
  }
    
}

void graphPanel::resizeByTime(){

  if (selGraph_) selGraph_->resizeByTime();

}

void graphPanel::resizeByValue(){

    if (selGraph_){
        selGraph_->resizeByValue();
    }
}

void graphPanel::scaleGraph(){

  if (selGraph_){

    if (sender()->objectName() == "btnScalePos")
      selGraph_->scale(true);
    else 
      selGraph_->scale(false);
  }
}

void graphPanel::undoCmd(){

  if (selGraph_) selGraph_->undoCmd();
}

void graphPanel::colorUpdate(){

  if (selGraph_) selGraph_->colorUpdate();
}

void graphPanel::updateSignals(){
  
  if (graphObj_.isEmpty() || !isPlay_) return;

    if (ui.btnAScale->isChecked()){
        for (auto ob : graphObj_)
            ob->resizeByValue();
    }

    if (cng.mode == SV_Graph::modeGr::player){

        qint64 bTm = QDateTime::currentDateTime().toMSecsSinceEpoch() - SV_CYCLESAVE_MS;

        QPair<qint64, qint64> tmIntl = ui.axisTime->getTimeInterval();

        ui.axisTime->setTimeInterval(bTm - (tmIntl.second - tmIntl.first), bTm);

        ui.axisTime->update();
    }

  for (auto ob : graphObj_)      
    ob->plotUpdate();    

    if (selGraph_){
        tableUpdate(selGraph_);
        tableUpdateAlter(selGraph_);
    }
}

void graphPanel::graphToDn(QString obj){

  wdgGraph* graph = qobject_cast<wdgGraph*>(sender());

  if (!graph) return;
    
  for (auto ob : graphObj_){

    if (ob->objectName() == obj){
      
      int ind = splitterGraph_->indexOf(ob);

      splitterGraph_->insertWidget(ind - 1, graph);
      
      QScrollBar* vscr = ui.scrollArea->verticalScrollBar();
      vscr->setValue(graph->pos().y());
      break;
    }
  }

  
}

void graphPanel::graphToUp(QString obj){

  wdgGraph* graph = qobject_cast<wdgGraph*>(sender());

  if (!graph) return;

  int sz = graphObj_.size();
  for (auto ob : graphObj_){

    if (ob->objectName() == obj){

      int ind = splitterGraph_->indexOf(ob);

      splitterGraph_->insertWidget(ind + 1 >= sz ? 0 : ind + 1, graph);
      QScrollBar* vscr = ui.scrollArea->verticalScrollBar();
      vscr->setValue(graph->pos().y());
      break;
    }
  }
}

QPair<qint64, qint64> graphPanel::getTimeInterval(){

  return ui.axisTime->getTimeInterval();

}

void graphPanel::setTimeInterval(qint64 stTime, qint64 enTime){

  ui.axisTime->setTimeInterval(stTime, enTime);

}

QVector<QVector<QString>> graphPanel::getLocateSignals(){

    QVector<QVector<QString>> res;
    for (auto ob : graphObj_){

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