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
#include "forms/triggerPanel.h"
#include "SVServer/SVServer.h"
#include "serverAPI.h"

using namespace SV_Cng;

void statusMess(QString mess);

triggerPanel::triggerPanel(QWidget *parent){

	setParent(parent);

	mainWin_ = (MainWin*)parent;

	ui.setupUi(this);

	connect(ui.btnAddTrigger, SIGNAL(clicked()), this, SLOT(addTrigger()));
	connect(ui.btnDelTrigger, SIGNAL(clicked()), this, SLOT(delTrigger()));
	connect(ui.btnChangeTrigger, SIGNAL(clicked()), this, SLOT(changeTrigger()));
	
	connect(ui.btnLess, &QPushButton::clicked, [this](){
		paramChange(); selCondition(SV_Cng::eventType::less); });
	connect(ui.btnEqual, &QPushButton::clicked, [this](){
		paramChange(); selCondition(SV_Cng::eventType::equals); });
	connect(ui.btnMore, &QPushButton::clicked, [this](){
		paramChange(); selCondition(SV_Cng::eventType::more); });
	connect(ui.btnNegFront, &QPushButton::clicked, [this](){
		paramChange(); selCondition(SV_Cng::eventType::negFront); });
	connect(ui.btnPosFront, &QPushButton::clicked, [this](){
		paramChange(); selCondition(SV_Cng::eventType::posFront); });

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

triggerPanel::~triggerPanel(){}

void triggerPanel::showEvent(QShowEvent* event){

	ui.listModule->clear(); auto mref = getCopyModuleRefSrv();
	for (auto mod : mref){
		if (!mod->isDelete) ui.listModule->addItem(mod->module.c_str());
	}
	ui.tableSignal->clearContents();
	ui.tableTrigger->clearContents();

	ctriggerCnt_ = getCopyTriggerRefSrv().size();

	if (ui.listModule->count() > 0) selModule(ui.listModule->item(0));
}

void triggerPanel::selModule(QListWidgetItem* item){

	selModule_ = item->text();

	isSelModule_ = true;
	
	enaBtnCondition(false);
		
	updateTableSignal();

	updateTableTrigger();

}

void triggerPanel::selSignal(QTableWidgetItem* item){

	selSignal_ = ui.tableSignal->item(item->row(), 0)->text();

	isSelModule_ = false;

	enaBtnCondition(true);

	eventType cond = eventType::less;

	signalData* sd = getSignalDataSrv(selSignal_ + selModule_);

	if (sd){
		if (sd->type == valueType::tBool) cond = eventType::posFront;

		ui.btnEqual->setEnabled(sd->type != valueType::tBool);
		ui.btnLess->setEnabled(sd->type != valueType::tBool);
		ui.btnMore->setEnabled(sd->type != valueType::tBool);
		ui.btnPosFront->setEnabled(sd->type == valueType::tBool);
		ui.btnNegFront->setEnabled(sd->type == valueType::tBool);
	}
	selCondition(cond);
		
	updateTableTrigger();

}

void triggerPanel::selTrigger(QTableWidgetItem* item){

	enaBtnCondition(!isSelModule_);

	updateStateSignal();
}


void triggerPanel::updateTableTrigger(){

	ui.tableTrigger->clearContents();

	auto tref = getCopyTriggerRefSrv();

	int row = 0, rowCnt = ui.tableTrigger->rowCount();
	for (auto t : tref){

		bool isModule = isSelModule_ && (t->module.c_str() == selModule_) &&
				((t->condType == eventType::connectModule) || (t->condType == eventType::disconnectModule));

		bool isSignal = !isSelModule_ && (t->signal.c_str() == selSignal_);

		if (isModule || isSignal){

			if (row > rowCnt){
				ui.tableTrigger->insertRow(rowCnt);	++rowCnt;
			}

			ui.tableTrigger->setItem(row, 0, new QTableWidgetItem(t->name.c_str()));

			if (isModule)
			    ui.tableTrigger->setItem(row, 1, new QTableWidgetItem(getEventTypeStr(t->condType).c_str()));
			else
				ui.tableTrigger->setItem(row, 1, new QTableWidgetItem(getEventTypeStr(t->condType).c_str() +
					 QString(" value ") + QString::number(t->condValue) + " tout " + QString::number(t->condTOut)));

			ui.tableTrigger->setItem(row, 2, new QTableWidgetItem(t->signal.c_str()));

			++row;
		}
	}

	if (row > 0){
		ui.tableTrigger->selectRow(qMin(cTriggRow_, row - 1));

		updateStateSignal();
	}
}

void triggerPanel::updateTableSignal(){

	ui.tableSignal->clearContents();
	auto sref = getCopySignalRefSrv();

	int row = 0, rowCnt = ui.tableSignal->rowCount();
	for (auto s : sref){

		if ((selModule_ == s->module.c_str()) && !s->isDelete){

			if (row > rowCnt){
				ui.tableSignal->insertRow(rowCnt);	++rowCnt;
			}
			ui.tableSignal->setItem(row, 0, new QTableWidgetItem(s->name.c_str()));
			ui.tableSignal->setItem(row, 1, new QTableWidgetItem(getSVTypeStr(s->type).c_str()));
			ui.tableSignal->setItem(row, 2, new QTableWidgetItem(s->comment.c_str()));
			
			++row;
		}
	}		
}

void triggerPanel::updateStateSignal(){
		
	int crow = ui.tableTrigger->currentRow();
	if (crow < 0) return;

	QString trigr = ui.tableTrigger->item(crow, 0)->text();
	if (trigr.isEmpty()) return;

	triggerData* td = getTriggerDataSrv(trigr);
	
    if (!td) return;

	if (!isSelModule_){
		ui.txtCondition->setText(QString::number(td->condValue));
		ui.txtTOut->setText(QString::number(td->condTOut));
		selCondition(td->condType);
	}
	ui.txtTrigg->setText(trigr);
	
	ui.rbtnActiv->setChecked(td->isActive);

	auto ue = mainWin_->getUserData(trigr);
	if (ue){
		ui.txtUserProcPath->setText(ue->userProcPath);
		ui.txtUserProcArgs->setText(ue->userProcArgs);
	}

}

void triggerPanel::updateWin(){

	showEvent(nullptr);
}


void triggerPanel::enaBtnCondition(bool ena){

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

void triggerPanel::selCondition(eventType type){
		
	ui.btnLess->setChecked(false);
	ui.btnEqual->setChecked(false);
	ui.btnMore->setChecked(false);
	ui.btnNegFront->setChecked(false);
	ui.btnPosFront->setChecked(false);
		
	switch (type)
	{
	case eventType::none:
		break;
	case eventType::connectModule:
		break;
	case eventType::disconnectModule:
		break;
	case eventType::less:
		if (ui.btnLess->isEnabled())
		   currCondition_ = eventType::less; ui.btnLess->setChecked(true);
		break;
	case eventType::equals:
		if (ui.btnEqual->isEnabled())
		   currCondition_ = eventType::equals; ui.btnEqual->setChecked(true);
		break;
	case eventType::more:
		if (ui.btnMore->isEnabled())
		   currCondition_ = eventType::more; ui.btnMore->setChecked(true);
		break;
	case eventType::posFront:
		if (ui.btnPosFront->isEnabled())
	    	currCondition_ = eventType::posFront; ui.btnPosFront->setChecked(true);
		break;
	case eventType::negFront:
		if (ui.btnNegFront->isEnabled())
    		currCondition_ = eventType::negFront; ui.btnNegFront->setChecked(true);
		break;
	default:
		break;
	}
}

void triggerPanel::addTrigger(){

	if (selSignal_.isEmpty()) return;

	QString tname = ui.txtTrigg->text();

	if (tname.isEmpty() || getTriggerDataSrv(tname)) return;

	triggerData* td = new triggerData();

	td->name = tname.toUtf8().data();
	td->signal = selSignal_.toUtf8().data();
	td->module = selModule_.toUtf8().data();
	td->condType = currCondition_;
	td->isActive = ui.rbtnActiv->isChecked();
	td->condValue = ui.txtCondition->text().toInt();
	td->condTOut = ui.txtTOut->text().toInt();	

	userEventData ud(tname, ui.txtUserProcPath->text(), ui.txtUserProcArgs->text());
	mainWin_->addUserData(ud);
	
	addTriggerSrv(tname, td);
	++ctriggerCnt_;

	if (ctriggerCnt_ > SV_TRIGGER_MAX_CNT)
		statusMess(tr("Превышен лимит количества триггеров: %1. Стабильная работа не гарантирована.").
		arg(SV_TRIGGER_MAX_CNT));
	
	ui.lbIsChange->setText("");
	updateTableTrigger();
}

void triggerPanel::delTrigger(){

	int crow = ui.tableTrigger->currentRow();
	if (crow < 0) return;
		
	QString sign = ui.tableTrigger->item(crow, 0) ? ui.tableTrigger->item(crow, 0)->text() : "";
	if (sign.isEmpty()) return;

	delTriggerSrv(sign);

	mainWin_->delUserData(sign);
	
	ui.lbIsChange->setText("");
	updateTableTrigger();
}

void triggerPanel::changeTrigger(){
	
	int crow = ui.tableTrigger->currentRow();
	if (crow < 0) return;
	
	QString name = ui.tableTrigger->item(crow, 0)->text();
	if (name.isEmpty()) return;

	auto td = getTriggerDataSrv(name);

	if (!td) return;
	
	if (!isSelModule_){
		td->condValue = ui.txtCondition->text().toInt();
		td->condTOut = ui.txtTOut->text().toInt();
		td->condType = currCondition_;
	}

	td->isActive = ui.rbtnActiv->isChecked();
				
	auto ue = mainWin_->getUserData(name);
	if (ue){
		ue->userProcArgs = ui.txtUserProcArgs->text();
		ue->userProcPath = ui.txtUserProcPath->text();
	}

	ui.lbIsChange->setText("");
	cTriggRow_ = crow;
	updateTableTrigger();
}

void triggerPanel::paramChange(){

	ui.lbIsChange->setText("*");

}

void triggerPanel::selDirProc(){

	QString fl = QFileDialog::getOpenFileName(this,
		tr("Выбор процесса"), selDirProc_);

	if (fl.isEmpty()) return;

	ui.txtUserProcPath->setText(fl);

	selDirProc_ = fl;

}