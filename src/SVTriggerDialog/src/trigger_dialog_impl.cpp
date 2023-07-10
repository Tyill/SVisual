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

#include "SVTriggerDialog/forms/trigger_dialog_impl.h"

using namespace SV_Base;
using namespace SV_Trigger;

#include <QFileDialog>
#include <QTranslator>

TriggerDialog::TriggerDialog(QWidget *parent, SV_Trigger::Config cng_):
    QDialog(parent)
{
#ifdef SV_EN
  QTranslator translator;
  translator.load(":/SVTrigger/trigger_dialog_en.qm");
  QCoreApplication::installTranslator(&translator);
#endif

  ui.setupUi(this);

  cng = cng_;

  connect(ui.btnAddTrigger, SIGNAL(clicked()), this, SLOT(addTrigger()));
  connect(ui.btnDelTrigger, SIGNAL(clicked()), this, SLOT(delTrigger()));
  connect(ui.btnChangeTrigger, SIGNAL(clicked()), this, SLOT(changeTrigger()));

  connect(ui.btnLess, &QPushButton::clicked, [this](){
    paramChange(); selCondition(EventType::LESS); });
  connect(ui.btnEqual, &QPushButton::clicked, [this](){
    paramChange(); selCondition(EventType::EQUALS); });
  connect(ui.btnMore, &QPushButton::clicked, [this](){
    paramChange(); selCondition(EventType::MORE); });
  connect(ui.btnNegFront, &QPushButton::clicked, [this](){
    paramChange(); selCondition(EventType::NEG_FRONT); });
  connect(ui.btnPosFront, &QPushButton::clicked, [this](){
    paramChange(); selCondition(EventType::POS_FRONT); });

  connect(ui.listModule, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selModule(QListWidgetItem*)));
  connect(ui.tableTrigger, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(selTrigger(QTableWidgetItem*)));
  connect(ui.tableSignal, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(selSignal(QTableWidgetItem*)));

  connect(ui.rbtnActiv, SIGNAL(clicked()), this, SLOT(paramChange()));
  connect(ui.txtCondition, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtTOut, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtUserProcPath, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtUserProcArgs, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));

  connect(ui.btnOpenProc, SIGNAL(clicked()), this, SLOT(selDirProc()));

  ui.tableTrigger->hideColumn(2);
}

TriggerDialog::~TriggerDialog(){

  thrStop_ = true;
  if (workThr_.joinable()) workThr_.join();
}

void TriggerDialog::startUpdateThread(){

  if (workThr_.joinable()) return;

  workThr_ = std::thread(&TriggerDialog::workCycle, this);
}

void TriggerDialog::showEvent(QShowEvent* event){

  ui.listModule->clear();

  auto mref = pfGetCopyModuleRef();
  for (auto mod : mref){
    if (!mod->isDelete) ui.listModule->addItem(mod->module.c_str());
  }
  ui.tableSignal->clearContents();
  ui.tableTrigger->clearContents();

  if (ui.listModule->count() > 0) selModule(ui.listModule->item(0));
}

void TriggerDialog::selModule(QListWidgetItem* item){

  selModule_ = item->text();

  isSelModule_ = true;

  enaBtnCondition(false);

  updateTableSignal();

  updateTableTrigger();

}

void TriggerDialog::selSignal(QTableWidgetItem* item){

  selSignal_ = ui.tableSignal->item(item->row(), 0)->text();

  isSelModule_ = false;

  enaBtnCondition(true);

  EventType cond = EventType::LESS;

  SignalData* sd = pfGetSignalData(selSignal_ + selModule_);

  if (sd){
    if (sd->type == ValueType::BOOL) cond = EventType::POS_FRONT;

    ui.btnEqual->setEnabled(sd->type != ValueType::BOOL);
    ui.btnLess->setEnabled(sd->type != ValueType::BOOL);
    ui.btnMore->setEnabled(sd->type != ValueType::BOOL);
    ui.btnPosFront->setEnabled(sd->type == ValueType::BOOL);
    ui.btnNegFront->setEnabled(sd->type == ValueType::BOOL);
  }
  selCondition(cond);

  updateTableTrigger();

}

void TriggerDialog::selTrigger(QTableWidgetItem* item){

  enaBtnCondition(!isSelModule_);

  updateStateSignal();
}

void TriggerDialog::updateTableTrigger(){

  ui.tableTrigger->clearContents();

  auto tref = getCopyTriggerRef();

  int row = 0, rowCnt = ui.tableTrigger->rowCount();
  for (auto t : tref){

    bool isModule = isSelModule_ && (t->module == selModule_) &&
      ((t->condType == EventType::CONNECT_MODULE) || (t->condType == EventType::DISCONNECT_MODULE));

    bool isSignal = !isSelModule_ && (t->signal == selSignal_);

    if (isModule || isSignal){

      if (row > rowCnt){
        ui.tableTrigger->insertRow(rowCnt);  ++rowCnt;
      }

      ui.tableTrigger->setItem(row, 0, new QTableWidgetItem(t->name));

      if (isModule)
        ui.tableTrigger->setItem(row, 1, new QTableWidgetItem(SV_Trigger::getEventTypeStr(t->condType)));
      else
        ui.tableTrigger->setItem(row, 1, new QTableWidgetItem(SV_Trigger::getEventTypeStr(t->condType) +
        QString(" Value ") + QString::number(t->condValue) + " tout " + QString::number(t->condTOut)));

      ui.tableTrigger->setItem(row, 2, new QTableWidgetItem(t->signal));

      ++row;
    }
  }

  if (row > 0){
    ui.tableTrigger->selectRow(qMin(cTriggRow_, row - 1));

    updateStateSignal();
  }
}

void TriggerDialog::updateTableSignal(){

  ui.tableSignal->clearContents();
  auto sref = pfGetCopySignalRef();

  int row = 0, rowCnt = ui.tableSignal->rowCount();
  for (auto s : sref){

    if ((selModule_ == s->module.c_str()) && !s->isDelete){

      if (row > rowCnt){
        ui.tableSignal->insertRow(rowCnt);  ++rowCnt;
      }
      ui.tableSignal->setItem(row, 0, new QTableWidgetItem(s->name.c_str()));
      ui.tableSignal->setItem(row, 1, new QTableWidgetItem(getSVTypeStr(s->type).c_str()));
      ui.tableSignal->setItem(row, 2, new QTableWidgetItem(s->comment.c_str()));

      ++row;
    }
  }
}

void TriggerDialog::updateStateSignal(){

  int crow = ui.tableTrigger->currentRow();
  if (crow < 0) return;

  QString trigr = ui.tableTrigger->item(crow, 0)->text();
  if (trigr.isEmpty()) return;

  TriggerData* td = getTriggerData(trigr);

  if (!td) return;

  if (!isSelModule_){
    ui.txtCondition->setText(QString::number(td->condValue));
    ui.txtTOut->setText(QString::number(td->condTOut));
    selCondition(td->condType);
  }
  ui.txtTrigg->setText(trigr);

  ui.rbtnActiv->setChecked(td->isActive);

  ui.txtUserProcPath->setText(td->userProcPath);
  ui.txtUserProcArgs->setText(td->userProcArgs);


}

void TriggerDialog::updateWin(){

  showEvent(nullptr);
}

void TriggerDialog::enaBtnCondition(bool ena){

  ui.btnLess->setEnabled(ena);
  ui.btnEqual->setEnabled(ena);
  ui.btnMore->setEnabled(ena);
  ui.btnNegFront->setEnabled(ena);
  ui.btnPosFront->setEnabled(ena);
  ui.txtTrigg->setEnabled(ena); ui.txtTrigg->setText("");
  ui.txtCondition->setEnabled(ena); ui.txtCondition->setText("");
  ui.txtTOut->setEnabled(ena); ui.txtTOut->setText("");
  ui.btnAddTrigger->setEnabled(ena);
  ui.btnDelTrigger->setEnabled(ena);

  ui.lbIsChange->setText("");
}

void TriggerDialog::selCondition(EventType type){

  ui.btnLess->setChecked(false);
  ui.btnEqual->setChecked(false);
  ui.btnMore->setChecked(false);
  ui.btnNegFront->setChecked(false);
  ui.btnPosFront->setChecked(false);

  switch (type)
  {
  case EventType::NONE:
    break;
  case EventType::CONNECT_MODULE:
    break;
  case EventType::DISCONNECT_MODULE:
    break;
  case EventType::LESS:
    if (ui.btnLess->isEnabled())
      currCondition_ = EventType::LESS; ui.btnLess->setChecked(true);
    break;
  case EventType::EQUALS:
    if (ui.btnEqual->isEnabled())
      currCondition_ = EventType::EQUALS; ui.btnEqual->setChecked(true);
    break;
  case EventType::MORE:
    if (ui.btnMore->isEnabled())
      currCondition_ = EventType::MORE; ui.btnMore->setChecked(true);
    break;
  case EventType::POS_FRONT:
    if (ui.btnPosFront->isEnabled())
      currCondition_ = EventType::POS_FRONT; ui.btnPosFront->setChecked(true);
    break;
  case EventType::NEG_FRONT:
    if (ui.btnNegFront->isEnabled())
      currCondition_ = EventType::NEG_FRONT; ui.btnNegFront->setChecked(true);
    break;
  default:
    break;
  }
}

void TriggerDialog::addTrigger(){

  if (selSignal_.isEmpty()) return;

  QString tname = ui.txtTrigg->text();

  if (tname.isEmpty() || getTriggerData(tname)) return;

  TriggerData* td = new TriggerData();

  td->name = tname.toUtf8().data();
  td->signal = selSignal_.toUtf8().data();
  td->module = selModule_.toUtf8().data();
  td->condType = currCondition_;
  td->isActive = ui.rbtnActiv->isChecked();
  td->condValue = ui.txtCondition->text().toInt();
  td->condTOut = ui.txtTOut->text().toInt();
  td->userProcPath = ui.txtUserProcPath->text();
  td->userProcArgs = ui.txtUserProcArgs->text();

  addTrigger(tname, td);

  ui.lbIsChange->setText("");
  updateTableTrigger();
}

void TriggerDialog::delTrigger(){

  int crow = ui.tableTrigger->currentRow();
  if (crow < 0) return;

  QString sign = ui.tableTrigger->item(crow, 0) ? ui.tableTrigger->item(crow, 0)->text() : "";
  if (sign.isEmpty()) return;

  delTrigger(sign);

  ui.lbIsChange->setText("");
  updateTableTrigger();
}

void TriggerDialog::changeTrigger(){

  int crow = ui.tableTrigger->currentRow();
  if (crow < 0) return;

  QString name = ui.tableTrigger->item(crow, 0)->text();
  if (name.isEmpty()) return;

  auto td = getTriggerData(name);

  if (!td) return;

  if (!isSelModule_){
    td->condValue = ui.txtCondition->text().toInt();
    td->condTOut = ui.txtTOut->text().toInt();
    td->condType = currCondition_;
  }

  td->isActive = ui.rbtnActiv->isChecked();

  td->userProcArgs = ui.txtUserProcArgs->text();
  td->userProcPath = ui.txtUserProcPath->text();

  ui.lbIsChange->setText("");
  cTriggRow_ = crow;
  updateTableTrigger();
}

void TriggerDialog::paramChange(){

  ui.lbIsChange->setText("*");

}

void TriggerDialog::selDirProc(){

  QString fl = QFileDialog::getOpenFileName(this,
    tr("Выбор процесса"), selDirProc_);

  if (fl.isEmpty()) return;

  ui.txtUserProcPath->setText(fl);

  selDirProc_ = fl;

}


///////////////////////////////////////////////////////////

// вернуть все триггеры
QMap<QString, TriggerData*> TriggerDialog::getCopyTriggerRef(){

  std::unique_lock<std::mutex> lck(mtx_);

  QMap<QString, TriggerData*> tref = triggerData_;

  return tref;
}

// вернуть данные триггера
SV_Trigger::TriggerData* TriggerDialog::getTriggerData(const QString& trg){

  std::unique_lock<std::mutex> lck(mtx_);

  return triggerData_.find(trg) != triggerData_.end() ? triggerData_[trg] : nullptr;
}

// добавить триггер
bool TriggerDialog::addTrigger(const QString& trg, SV_Trigger::TriggerData* td){

  std::unique_lock<std::mutex> lck(mtx_);

  bool ok = false;
  if (td && !triggerData_.contains(trg)){
    triggerData_[trg] = td;
    ok = true;
  }

  return ok;
}

// удалить триггер
bool TriggerDialog::delTrigger(const QString& trg){

  std::unique_lock<std::mutex> lck(mtx_);

  bool ok = true;
  if (triggerData_.contains(trg)){
    triggerData_[trg]->isActive = false;

    // память не очищается специально!!
    triggerData_.remove(trg);
  }
  else ok = false;

  return ok;
}


///////////////////////////////////////////////////////////

bool TriggerDialog::checkCondition(SV_Trigger::TriggerData* tr, SignalData* sd){

  SV_Base::RecData lastData;
  QVector<SV_Base::Value> values(SV_PACKETSZ);
  {LockerReadSDataTrigger lock;
      lastData.beginTime = sd->lastData.beginTime;
      memcpy(values.data(), sd->lastData.vals, sizeof(SV_Base::Value) * SV_PACKETSZ);
      lastData.vals = values.data();
  }

  bool ena = true, isImpulse = tr->condTOut <= 0;
  switch (tr->condType){
  case EventType::EQUALS:
    if (isImpulse){
      ena = false;
      if (sd->type == ValueType::INT){
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vInt == tr->condValue){
            ena = true; break;
          }
        }
      }
      else{
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vFloat == tr->condValue){
            ena = true; break;
          }
        }
      }
    }
    else{
      ena = true;
      if (sd->type == ValueType::INT){
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vInt != tr->condValue){
            ena = false; break;
          }
        }
      }
      else{
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vFloat != tr->condValue){
            ena = false; break;
          }
        }
      }
    }
    break;
  case EventType::LESS:
    if (isImpulse){
      ena = false;
      if (sd->type == ValueType::INT){
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vInt < tr->condValue){
            ena = true; break;
          }
        }
      }
      else{
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vFloat < tr->condValue){
            ena = true; break;
          }
        }
      }
    }
    else{
      ena = true;
      if (sd->type == ValueType::INT){
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vInt >= tr->condValue){
            ena = false; break;
          }
        }
      }
      else{
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vFloat >= tr->condValue){
            ena = false; break;
          }
        }
      }
    }
    break;
  case EventType::MORE:
    if (isImpulse){
      ena = false;
      if (sd->type == ValueType::INT){
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vInt > tr->condValue){
            ena = true; break;
          }
        }
      }
      else{
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vFloat > tr->condValue){
            ena = true; break;
          }
        }
      }
    }
    else{
      ena = true;
      if (sd->type == ValueType::INT){
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vInt <= tr->condValue){
            ena = false; break;
          }
        }
      }
      else{
        for (int i = 0; i < SV_PACKETSZ; ++i) {
          if (lastData.vals[i].vFloat <= tr->condValue){
            ena = false; break;
          }
        }
      }
    }
    break;
  case EventType::POS_FRONT:
    if (isImpulse){
      ena = false;
      for (int i = 0; i < SV_PACKETSZ; ++i) {
        if (lastData.vals[i].vBool){
          ena = true; break;
        }
      }
    }
    else{
      ena = true;
      for (int i = 0; i < SV_PACKETSZ; ++i) {
        if (!lastData.vals[i].vBool){
          ena = false; break;
        }
      }
    }
    break;
  case EventType::NEG_FRONT:
    if (isImpulse){
      ena = false;
      for (int i = 0; i < SV_PACKETSZ; ++i) {
        if (!lastData.vals[i].vBool){
          ena = true; break;
        }
      }
    }
    else{
      ena = true;
      for (int i = 0; i < SV_PACKETSZ; ++i) {
        if (lastData.vals[i].vBool){
          ena = false; break;
        }
      }
    }
    break;

  case EventType::CONNECT_MODULE:
    ena = tr->condValue == 1;
    break;

  case EventType::DISCONNECT_MODULE:
    ena = tr->condValue == 1;
    break;

  default: ena = false;
  }

  return ena;
}

void TriggerDialog::workCycle(){

  QMap<QString, int> trgId; int cid = 0;
  QMap<QString, SignalData*> sdata;

  SV_Misc::TimerDelay tmDelay;
  SV_Misc::Front front;

  while (!thrStop_){

    auto tref = getCopyTriggerRef();

    tmDelay.update();

    for (auto& t : tref){

      if (!trgId.contains(t->name)) {
        ++cid;
        trgId[t->name] = cid;
        sdata[t->name] = pfGetSignalData(qUtf8Printable(t->signal + t->module));
      }

      if (!sdata[t->name] && (t->condType != EventType::CONNECT_MODULE) &&
        (t->condType != EventType::DISCONNECT_MODULE)) continue;

      if (front.PosFront(tmDelay.onDelaySec(t->isActive && checkCondition(t, sdata[t->name]),
        t->condTOut, trgId[t->name]), trgId[t->name])){

        if (pfOnTriggerCBack)
          pfOnTriggerCBack(t->name);
      }
    }

    int ms = SV_CYCLESAVE_MS - (int)tmDelay.getCTime();
    if (ms > 0)
      SV_Misc::sleepMs(ms);
  }
}


