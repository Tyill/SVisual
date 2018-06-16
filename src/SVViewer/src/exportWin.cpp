
#include "stdafx.h"
#include "forms/exportWin.h"

exportWin::exportWin(QWidget *parent){

	setParent(parent);
		
	ui.setupUi(this);

	/*ui.txtBeginDate->setCalendarPopup(true);
	ui.txtEndDate->setCalendarPopup(true);


	connect(ui.btnShowOrder, SIGNAL(clicked()), this, SLOT(showOrder()));
*/
}

exportWin::~exportWin(){}

void exportWin::showEvent(QShowEvent * event){

	/*ui.txtBeginDate->setDateTime(QDateTime::currentDateTime());
	ui.txtBeginDate->setTime(QTime::fromString("00:00"));

	ui.txtEndDate->setDateTime(QDateTime::currentDateTime());

	showOrder();*/
}