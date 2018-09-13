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
#include "forms/exportPanel.h"
#include "SVConfig/SVConfigLimits.h"


using namespace SV_Cng;

exportPanel::exportPanel(QWidget *parent, SV_Exp::config cng_){
		
	setParent(parent);
	
#ifdef SV_EN
	QTranslator translator;
	translator.load(":/SVExp/svexppanel_en.qm");
	QCoreApplication::installTranslator(&translator);
#endif

	setAcceptDrops(true);

	cng = cng_;

	ui.setupUi(this);

    connect(ui.btnAddSignal, &QPushButton::clicked, [this]() {
        addSignalOnExport();
    });

    connect(ui.btnDelSignal, &QPushButton::clicked, [this]() {
        delSignalFromExport();
    });

}

exportPanel::~exportPanel(){


}

void exportPanel::showEvent(QShowEvent * event){

    ui.dTimeBegin->setDateTime(QDateTime::currentDateTime());
  //  ui.dTimeEnd->setDateTime(QTime::fromString("00:00"));

    auto mref = pfGetCopyModuleRef();

    ui.tableModule->clear(); 
    for (auto mod : mref){
        if (!mod->isDelete) ui.tableModule->addItem(mod->module.c_str());
    }
    ui.tableSignal->clearContents();
     
    if (ui.tableModule->count() > 0)
        selModule(ui.tableModule->item(0));
}


void exportPanel::selModule(QListWidgetItem* item){

    selModule_ = item->text();

    updateTableSignal();
}



void exportPanel::addSignalOnExport(){

    int rows = ui.tableSignal->rowCount();

    for (int i = 0; i < rows; ++i){

        auto item = ui.tableSignal->item(i, 0);
        if (item->isSelected()){

            QString nm = item->text() + selModule_;
            if (!expSignals_.contains(nm))
                expSignals_.insert(nm);
        }
    }   

    updateTableExport();
}

void exportPanel::updateTableSignal(){

    ui.tableSignal->clearContents();
    auto sref = pfGetCopySignalRef();

    int row = 0, rowCnt = ui.tableSignal->rowCount();
    for (auto s : sref){

        if ((selModule_ == s->module.c_str()) && !s->isDelete){

            if (row >= rowCnt){
                ui.tableSignal->insertRow(rowCnt);	++rowCnt;
            }
            ui.tableSignal->setItem(row, 0, new QTableWidgetItem(s->name.c_str()));
            ui.tableSignal->setItem(row, 1, new QTableWidgetItem(getSVTypeStr(s->type).c_str()));
            ui.tableSignal->setItem(row, 2, new QTableWidgetItem(s->comment.c_str()));

            ++row;
        }
    }
    ui.tableSignal->update();
}

void exportPanel::updateTableExport(){

    ui.tableExport->clearContents();
    auto sref = pfGetCopySignalRef();

    int row = 0, rowCnt = ui.tableExport->rowCount();
    for (auto s : sref){

        if (expSignals_.contains((s->name + s->module).c_str())){

            if (row >= rowCnt){
                ui.tableExport->insertRow(rowCnt);	++rowCnt;
            }
            ui.tableExport->setItem(row, 0, new QTableWidgetItem(s->module.c_str()));
            ui.tableExport->setItem(row, 1, new QTableWidgetItem(s->name.c_str()));
            ui.tableExport->setItem(row, 2, new QTableWidgetItem(getSVTypeStr(s->type).c_str()));
            ui.tableExport->setItem(row, 3, new QTableWidgetItem(s->comment.c_str()));

            ++row;
        }
    }
    ui.tableExport->update();
}

void exportPanel::delSignalFromExport(){


}

void exportPanel::selectSignalChange(){

		
}


