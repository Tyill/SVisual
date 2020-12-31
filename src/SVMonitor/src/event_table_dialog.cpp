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

#include "SVMonitor/forms/event_table_dialog.h"
#include "db_provider.h"

EventTableDialog::EventTableDialog(DbProvider* db, QWidget *parent):
  QDialog(parent), db_(db){
    
  ui.setupUi(this);

  ui.txtBeginDate->setCalendarPopup(true);
  ui.txtEndDate->setCalendarPopup(true);

  connect(ui.btnShowOrder, SIGNAL(clicked()), this, SLOT(showOrder()));
}

EventTableDialog::~EventTableDialog(){}

void EventTableDialog::showOrder(){

  QVector<UserEvent> events = db_ ? db_->getEvents(ui.txtBeginDate->dateTime(), ui.txtEndDate->dateTime()) : QVector<UserEvent>();

  ui.tableEvents->clearContents();
  int sz = events.size(), rowCnt = ui.tableEvents->rowCount();

  ui.tableEvents->setSortingEnabled(false);
  for (int i = 0; i < sz; ++i){

    if (i >= rowCnt){
      ui.tableEvents->insertRow(rowCnt);  ++rowCnt;
    }

    ui.tableEvents->setItem(i, 0, new QTableWidgetItem(events[i].sendDateTime));
    ui.tableEvents->setItem(i, 1, new QTableWidgetItem(events[i].triggName));
    ui.tableEvents->setItem(i, 2, new QTableWidgetItem(events[i].module));
    ui.tableEvents->setItem(i, 3, new QTableWidgetItem(events[i].signal));

    QString cond = SV_Trigger::getEventTypeStr(events[i].condType) + QString(" ") + QString::number(events[i].condValue) + " T" + QString::number(events[i].condTOut);

    ui.tableEvents->setItem(i, 4, new QTableWidgetItem(cond));
  }
  ui.tableEvents->setSortingEnabled(true);

}

void EventTableDialog::showEvent(QShowEvent * event){

  ui.txtBeginDate->setDateTime(QDateTime::currentDateTime());
  ui.txtBeginDate->setTime(QTime::fromString("00:00"));

  ui.txtEndDate->setDateTime(QDateTime::currentDateTime());

  showOrder();
}