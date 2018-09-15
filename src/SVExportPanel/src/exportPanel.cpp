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
#include "Lib/xlsx/xlsxdocument.h"


using namespace SV_Cng;

exportPanel::exportPanel(QWidget *parent, SV_Exp::config cng_){
		
	setParent(parent);
	
#ifdef SV_EN
	QTranslator translator;
	translator.load(":/SVExp/svexppanel_en.qm");
	QCoreApplication::installTranslator(&translator);
#endif
    
	cng = cng_;

	ui.setupUi(this);

    ui.dTimeBegin->setDateTime(QDateTime::currentDateTime().addSecs(-10));
    ui.dTimeEnd->setDateTime(QDateTime::currentDateTime());

    connect(ui.btnAddSignal, &QPushButton::clicked, [this]() {
        addSignalOnExport();
    });

    connect(ui.btnDelSignal, &QPushButton::clicked, [this]() {
        delSignalFromExport();
    });

    connect(ui.btnExport, &QPushButton::clicked, [this]() {
       
        QFileDialog dialog(this);

        QString selectedFilter;
        QString fileName = dialog.getSaveFileName(this,
            tr("Открытие файла параметров"), ".",
            tr("txt file (*.txt);; xlsx file (*.xlsx);; json file (*.jsn)"), &selectedFilter);

        if (!fileName.isEmpty()){
                                  
            if (selectedFilter == "txt file (*.txt)")
                exportToTXT(fileName);
            else if (selectedFilter == "xlsx file (*.xlsx)")
                exportToXLSX(fileName);
            else if (selectedFilter == "json file (*.jsn)")
                exportToJSON(fileName);
        }
    });
}

exportPanel::~exportPanel(){


}

void exportPanel::showEvent(QShowEvent * event){
        
  
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
        if (!item) continue;
        if (item->isSelected()){

            QString nm = item->text() + selModule_;
            if (!expSignals_.contains(nm))
                expSignals_.insert(nm);
        }
    }   

    updateTableExport();
}

void exportPanel::delSignalFromExport(){

    int rows = ui.tableExport->rowCount();

    for (int i = 0; i < rows; ++i){

        auto item = ui.tableExport->item(i, 1);
        if (!item) continue;
        if (item->isSelected()){

            QString nm = item->text() + selModule_;
            if (expSignals_.contains(nm))
                expSignals_.remove(nm);
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

void exportPanel::exportToXLSX(QString fileName){

    QXlsx::Document xlsx;

    uint64_t beginTime = ui.dTimeBegin->dateTime().toMSecsSinceEpoch();
    uint64_t endTime = ui.dTimeEnd->dateTime().toMSecsSinceEpoch();

    QString module = "";
    int row = 1, col = 1;
    for (auto& s : expSignals_){

        auto sd = pfGetSignalData(s);

        if (module != sd->module.c_str()){
            module = sd->module.c_str();
            xlsx.write(1, col, sd->module.c_str()); ++col;
        }

        xlsx.write(1, col, sd->name.c_str()); ++col;

        row = 2;
        auto bsz = sd->buffData.size();
        for (int i = 0; i < bsz; ++i){
                       
            uint64_t dt = sd->buffData[i].beginTime;
            if (((beginTime < dt) && (dt < endTime)) || ui.chbAllTime->isChecked()){

                // время                
                xlsx.write(row, 1, QDateTime::fromMSecsSinceEpoch(dt));
                
                // значения
                if (ui.rbtnEveryVal->isChecked()){
                    for (int j = 0; j < SV_PACKETSZ; ++j){
                        switch (sd->type){
                        case valueType::tBool:  xlsx.write(row, col - 1, sd->buffData[i].vals[j].tBool ? 1 : 0); break;
                        case valueType::tInt:   xlsx.write(row, col - 1, sd->buffData[i].vals[j].tInt); break;
                        case valueType::tFloat: xlsx.write(row, col - 1, sd->buffData[i].vals[j].tFloat); break;
                        }
                        ++row;
                     }
                }
                else if (ui.rbtnEveryMin->isChecked()){
                    int evr = 60000 / SV_CYCLESAVE_MS;

                    if ((i % evr) == 0){
                        switch (sd->type){
                        case valueType::tBool:  xlsx.write(row, col - 1, sd->buffData[i].vals[0].tBool ? 1 : 0); break;
                        case valueType::tInt:   xlsx.write(row, col - 1, sd->buffData[i].vals[0].tInt); break;
                        case valueType::tFloat: xlsx.write(row, col - 1, sd->buffData[i].vals[0].tFloat); break;
                        }
                        ++row;
                    }                    
                }
                else if (ui.rbtnEverySec->isChecked()){
                    int evr = 1000 / SV_CYCLESAVE_MS;

                    if ((i % evr) == 0){
                        switch (sd->type){
                        case valueType::tBool:  xlsx.write(row, col - 1, sd->buffData[i].vals[0].tBool ? 1 : 0); break;
                        case valueType::tInt:   xlsx.write(row, col - 1, sd->buffData[i].vals[0].tInt); break;
                        case valueType::tFloat: xlsx.write(row, col - 1, sd->buffData[i].vals[0].tFloat); break;
                        }
                        ++row;
                    }
                }
            }            
        }
    }   
      
    if (xlsx.saveAs(fileName))
        ui.lbMessage->setText(tr("Успешно сохранен: ") + fileName);
    else
        ui.lbMessage->setText(tr("Не удалось сохранить: ") + fileName);

    QTimer* tmr = new QTimer(this);
    connect(tmr, &QTimer::timeout, [=]() {            
        ui.lbMessage->setText("");
        tmr->stop();
        tmr->deleteLater();
    });
    tmr->setInterval(5000);
    tmr->start();
}

void exportPanel::exportToTXT(QString fileName){
    
    QFile data(fileName);
    QTextStream out(&data);
       
    if (data.open(QFile::WriteOnly)){

        uint64_t beginTime = ui.dTimeBegin->dateTime().toMSecsSinceEpoch();
        uint64_t endTime = ui.dTimeEnd->dateTime().toMSecsSinceEpoch();

        for (auto& s : expSignals_){

            auto sd = pfGetSignalData(s);

            out << "module = " << sd->module.c_str() << '\n';
            out << "signal = " << sd->name.c_str() << '\n';

            // время 
            out << "time = ";
            auto bsz = sd->buffData.size();
            for (int i = 0; i < bsz; ++i){

                uint64_t dt = sd->buffData[i].beginTime;
                if (((beginTime < dt) && (dt < endTime)) || ui.chbAllTime->isChecked()){

                    // время                
                    out << QDateTime::fromMSecsSinceEpoch(dt).toString("dd-MM-yy HH:mm:ss") << "  ";
                    break;
                }
            }
            out << '\n';

            out << "value = ";
            for (int i = 0; i < bsz; ++i){

                uint64_t dt = sd->buffData[i].beginTime;
                if (((beginTime < dt) && (dt < endTime)) || ui.chbAllTime->isChecked()){

                    // значения
                    if (ui.rbtnEveryVal->isChecked()){
                        for (int j = 0; j < SV_PACKETSZ; ++j){
                            switch (sd->type){
                            case valueType::tBool:  out << QString::number(sd->buffData[i].vals[j].tBool ? 1 : 0) << ' '; break;
                            case valueType::tInt:   out << QString::number(sd->buffData[i].vals[j].tInt) << ' '; break;
                            case valueType::tFloat: out << QString::number(sd->buffData[i].vals[j].tFloat) << ' '; break;
                            }
                        }
                    }
                    else if (ui.rbtnEveryMin->isChecked()){
                        int evr = 60000 / SV_CYCLESAVE_MS;

                        if ((i % evr) == 0){
                            switch (sd->type){
                            case valueType::tBool:  out << QString::number(sd->buffData[i].vals[0].tBool ? 1 : 0) << ' '; break;
                            case valueType::tInt:   out << QString::number(sd->buffData[i].vals[0].tInt) << ' '; break;
                            case valueType::tFloat: out << QString::number(sd->buffData[i].vals[0].tFloat) << ' '; break;
                            }
                        }
                    }
                    else if (ui.rbtnEverySec->isChecked()){
                        int evr = 1000 / SV_CYCLESAVE_MS;

                        if ((i % evr) == 0){
                            switch (sd->type){
                            case valueType::tBool:  out << QString::number(sd->buffData[i].vals[0].tBool ? 1 : 0) << ' '; break;
                            case valueType::tInt:   out << QString::number(sd->buffData[i].vals[0].tInt) << ' '; break;
                            case valueType::tFloat: out << QString::number(sd->buffData[i].vals[0].tFloat) << ' '; break;
                            }
                        }
                    }
                }
            }
            out << '\n';
        }

        data.close();
        ui.lbMessage->setText(tr("Успешно сохранен: ") + fileName);
    }   
    else
        ui.lbMessage->setText(tr("Не удалось создать файл: ") + fileName);
     
    QTimer* tmr = new QTimer(this);
    connect(tmr, &QTimer::timeout, [=]() {
        ui.lbMessage->setText("");
        tmr->stop();
        tmr->deleteLater();
    });
    tmr->setInterval(5000);
    tmr->start();
}

void exportPanel::exportToJSON(QString fileName){


}



