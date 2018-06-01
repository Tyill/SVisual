
#include "stdafx.h"
#include "forms/eventOrderWin.h"
#include "structurs.h"
#include "serverAPI.h"

eventOrderWin::eventOrderWin(QWidget *parent){

	setParent(parent);

	mainWin_ = (MainWin*)parent;

	ui.setupUi(this);

	ui.txtBeginDate->setCalendarPopup(true);
	ui.txtEndDate->setCalendarPopup(true);


	connect(ui.btnShowOrder, SIGNAL(clicked()), this, SLOT(showOrder()));

}

eventOrderWin::~eventOrderWin(){}

void eventOrderWin::showOrder(){
		
	QVector<uEvent> events = mainWin_->getEvents(ui.txtBeginDate->dateTime(), ui.txtEndDate->dateTime());

	ui.tableEvents->clearContents();
	int sz = events.size(), rowCnt = ui.tableEvents->rowCount();

	ui.tableEvents->setSortingEnabled(false);
	for (int i = 0; i < sz; ++i){

		if (i >= rowCnt){
			ui.tableEvents->insertRow(rowCnt);	++rowCnt;
		}
	
		ui.tableEvents->setItem(i, 0, new QTableWidgetItem(events[i].sendDateTime));
		ui.tableEvents->setItem(i, 1, new QTableWidgetItem(events[i].triggName));
		ui.tableEvents->setItem(i, 2, new QTableWidgetItem(events[i].module));
		ui.tableEvents->setItem(i, 3, new QTableWidgetItem(events[i].signal));

		QString cond = SV_Cng::getEventTypeStr(events[i].condType).c_str() + QString(" ") + QString::number(events[i].condValue) + " T" + QString::number(events[i].condTOut);

		ui.tableEvents->setItem(i, 4, new QTableWidgetItem(cond));
	}
	ui.tableEvents->setSortingEnabled(true);

}

void eventOrderWin::showEvent(QShowEvent * event){

	ui.txtBeginDate->setDateTime(QDateTime::currentDateTime());
	ui.txtBeginDate->setTime(QTime::fromString("00:00"));

	ui.txtEndDate->setDateTime(QDateTime::currentDateTime());

	showOrder();
}