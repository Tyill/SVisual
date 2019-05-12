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

#include <thread>

#include "stdafx.h"
#include "forms/scriptPanel.h"
#include "SVConfig/SVConfigLimits.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "SVAuxFunc/TimerDelay.h"
#include "SVAuxFunc/auxFunc.h"

using namespace SV_Cng;
namespace lub = luabridge;

scriptPanel* scrPanelRef = nullptr;

bool getBoolValue(const std::string& module, const std::string& signal){

    std::string sign = signal + module;
    if (scrPanelRef->updateBuffValue(module, signal, SV_Cng::valueType::tBool))
        return scrPanelRef->signBuff_[sign]->lastData.vals[scrPanelRef->iterValue_].tBool;
    else
        return false;
}

int getIntValue(const std::string& module, const std::string& signal){

    std::string sign = signal + module;
    if (scrPanelRef->updateBuffValue(module, signal, SV_Cng::valueType::tInt))
        return scrPanelRef->signBuff_[sign]->lastData.vals[scrPanelRef->iterValue_].tInt;
    else
        return 0;
}

float getFloatValue(const std::string& module, const std::string& signal){

    std::string sign = signal + module;
    if (scrPanelRef->updateBuffValue(module, signal, SV_Cng::valueType::tFloat))
        return scrPanelRef->signBuff_[sign]->lastData.vals[scrPanelRef->iterValue_].tFloat;
    else
        return 0.F;
}

void setBoolValue(const std::string& signal, bool value){

   std::string sign = signal + "Virtual";

   SV_Cng::value val;
   val.tBool = value;

   if (scrPanelRef->updateBuffValue("Virtual", signal, SV_Cng::valueType::tBool))
       scrPanelRef->setValue(sign, val);
}
     
void setIntValue(const std::string& signal, int value){

    std::string sign = signal + "Virtual";

    SV_Cng::value val;
    val.tInt = value;

    if (scrPanelRef->updateBuffValue("Virtual", signal, SV_Cng::valueType::tInt))
        scrPanelRef->setValue(sign, val);
}
     
void setFloatValue(const std::string& signal, float value){

    std::string sign = signal + "Virtual";

    SV_Cng::value val;
    val.tFloat = value;

    if (scrPanelRef->updateBuffValue("Virtual", signal, SV_Cng::valueType::tFloat))
        scrPanelRef->setValue(sign, val);
}

void scriptPanel::setValue(const std::string& sign, SV_Cng::value val){

    auto sd = signBuff_[sign];    
     
    sd->lastData.vals[iterValue_] = val;

    // заполняем буфер
    int vp = sd->buffValuePos;
    
    sd->buffData[vp].vals[iterValue_] = val;
    
    if (iterValue_ == (SV_PACKETSZ - 1)){

        sd->lastData.beginTime = cTm_;
        sd->buffData[vp].beginTime = cTm_;

        updateSign(sd, sd->buffBeginPos, vp);

        ++vp;

        if (mode_ == SV_Script::modeGr::player){
            int buffSz = 2 * 3600000 / SV_CYCLESAVE_MS; // 2 часа жестко

            if (vp == buffSz) vp = 0;
            sd->buffValuePos = vp;

            if (vp == sd->buffBeginPos) {
                ++sd->buffBeginPos;
                if (sd->buffBeginPos >= buffSz) sd->buffBeginPos = 0;
            }
        }
    }
}

void scriptPanel::updateSign(signalData* sign, int beginPos, int valuePos){

    sign->buffMinTime = sign->buffData[beginPos].beginTime;
    sign->buffMaxTime = sign->buffData[valuePos].beginTime + SV_CYCLESAVE_MS;

    double minValue = sign->buffMinValue, maxValue = sign->buffMaxValue;

    if (sign->type == valueType::tInt){

        value* vl = sign->buffData[valuePos].vals;

        for (int i = 0; i < SV_PACKETSZ; ++i){

            if (vl[i].tInt > maxValue) maxValue = vl[i].tInt;
            if (vl[i].tInt < minValue) minValue = vl[i].tInt;
        }

    }
    else if (sign->type == valueType::tFloat){

        value* vl = sign->buffData[valuePos].vals;
        for (int i = 0; i < SV_PACKETSZ; ++i){

            if (vl[i].tFloat > maxValue) maxValue = vl[i].tFloat;
            if (vl[i].tFloat < minValue) minValue = vl[i].tFloat;
        }
    }

    sign->buffMinValue = minValue;
    sign->buffMaxValue = maxValue;

}

bool scriptPanel::updateBuffValue(const std::string& module, const std::string& signal, SV_Cng::valueType stype){

    std::string sign = signal + module;
             
    if (signBuff_.find(sign) == signBuff_.end()){

        if (module == "Virtual"){

            if (!pfAddSignal || !pfAddModule || !pfGetModuleData || !pfLoadSignalData)
                return false;

            signalData* sd = new signalData();
            signBuff_[sign] = sd;

            sd->isActive = true;
            sd->isBuffEnable = false;
            sd->isDelete = false;

            sd->name = signal;
            sd->module = "Virtual";
            sd->type = stype;

            sd->lastData.vals = new SV_Cng::value[SV_PACKETSZ];
            sd->lastData.beginTime = SV_Aux::CurrDateTimeSinceEpochMs();
            memset(sd->lastData.vals, 0, sizeof(SV_Cng::value) * SV_PACKETSZ);

            sd->buffMinTime = sd->lastData.beginTime - 5000;
            sd->buffMaxTime = sd->lastData.beginTime + 5000;
            sd->buffMaxValue = 1;
            sd->buffMinValue = 0;

            auto md = pfGetModuleData("Virtual");
            if (!md){
                md = new SV_Cng::moduleData("Virtual");
                md->isActive = false;
                md->isDelete = false;
                md->isEnable = true;
                pfAddModule("Virtual", md);

                if (pfModuleConnectCBack)
                    pfModuleConnectCBack("Virtual");
            }

            md->signls.push_back(sign);
           
            pfAddSignal(sign, sd);
            pfLoadSignalData(sign);
        }
        else{

            if (!pfGetCopySignalRef || !pfLoadSignalData)
                return false;

            auto signRef = pfGetCopySignalRef();

            if (signRef.find(sign) == signRef.end())
                return false;

            pfLoadSignalData(sign);

            signBuff_[sign] = signRef[sign];
        }

        if (pfAddSignalsCBack)
            pfAddSignalsCBack();
    }

    return true;
}

scriptPanel::scriptPanel(QWidget *parent, SV_Script::config cng_, SV_Script::modeGr mode){
		
	setParent(parent);
	
#ifdef SV_EN
	QTranslator translator;
	translator.load(":/SVScript/svscriptpanel_en.qm");
	QCoreApplication::installTranslator(&translator);
#endif
    	
    mode_ = mode;
	cng = cng_;

	ui.setupUi(this);

    scrPanelRef = this;
               
    connect(ui.btnNewScript, SIGNAL(clicked()), SLOT(addScript()));
    connect(ui.btnSave, SIGNAL(clicked()), SLOT(saveScript()));  
    connect(ui.tblScripts, SIGNAL(cellChanged(int, int)), SLOT(nameScriptChange(int,int)));
    connect(ui.tabWidget, &QTabWidget::tabCloseRequested, [this](int inx){
        
        QString sname = ui.tabWidget->tabText(inx);
        auto it = std::find_if(scrState_.begin(), scrState_.end(),

            [sname](const scriptState& st) {

            return st.name == sname;
        });

        if (it->isChange){
            QMessageBox msgBox;
            msgBox.setText(sname + tr(" изменен."));
            msgBox.setInformativeText(tr("Сохранить изменения?"));
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Save);

            if (msgBox.exec() == QMessageBox::Save)
                saveScript();
        }

        it->isChange = false;

        ui.tabWidget->removeTab(inx);

        for (int i = 0; i < scrState_.size(); ++i){
        
            if (i == inx)
                scrState_[i].tabInx = -1;
            else if (i > inx)
                --scrState_[i].tabInx;
        }
                      
        if ((inx - 1) >= 0){
            QString sname = ui.tabWidget->tabText(inx - 1);
            auto it = std::find_if(scrState_.begin(), scrState_.end(),

                [sname](const scriptState& st) {

                return st.name == sname;
            });

            ui.lbChange->setText(it->isChange ? "*" : "");
        }
    });
    connect(ui.tabWidget, &QTabWidget::tabBarClicked, [this](int inx){

        QString sname = ui.tabWidget->tabText(inx);
        auto it = std::find_if(scrState_.begin(), scrState_.end(),

            [sname](const scriptState& st) {

            return st.name == sname;
        });

        ui.lbChange->setText(it->isChange ? "*" : "");
    });
    connect(ui.tblScripts, &QTableWidget::itemDoubleClicked, [this](QTableWidgetItem* item){

        int row = item->row();
        QString scrName = ui.tblScripts->item(row, 0)->text();

        int sz = ui.tabWidget->count();
        for (int i = 0; i < sz; ++i){
            if (ui.tabWidget->tabText(i) == scrName)
                return;
        }
               
        scrState_[row].tabInx = ui.tabWidget->count();

        auto te = new QTextEdit();
        te->setFrameShape(QFrame::NoFrame);

        te->setText(scrState_[row].text);
                        
        connect(te, &QTextEdit::textChanged, [this, row](){
            scrState_[row].isChange = true;
            ui.lbChange->setText("*");
        });

        ui.tabWidget->addTab(te, scrName);

        ui.tabWidget->setCurrentIndex(scrState_[row].tabInx);

        ui.lbChange->setText(scrState_[row].isChange ? "*" : "");
    });
        
    QDir dir(QApplication::applicationDirPath() + "/scripts/");

    if (!dir.exists())
        dir.mkdir(dir.absolutePath());

    for (auto& f : dir.entryList(QDir::Files)){

        QString name = f.right(f.size() - f.lastIndexOf('/') - 1);
             
        addScript(name);
    }

    QList<int> ss; ss.append(150); ss.append(500);
    ui.splitter->setSizes(ss);   
    ui.tblScripts->setColumnWidth(0, 150);
    ui.tblActiveScripts->setColumnWidth(0, 150);

    luaState_ = luaL_newstate();
    luaL_openlibs(luaState_);

    lub::getGlobalNamespace(luaState_)
        .addFunction("getBoolValue", getBoolValue)
        .addFunction("getIntValue", getIntValue)
        .addFunction("getFloatValue", getFloatValue)
        .addFunction("setBoolValue", setBoolValue)
        .addFunction("setIntValue", setIntValue)
        .addFunction("setFloatValue", setFloatValue);
       
    luaL_loadfile(luaState_, qPrintable(QApplication::applicationDirPath() + "/scripts/load.lua"));
    lua_pcall(luaState_, 0, 0, 0);   
       
    if (mode_ == SV_Script::modeGr::player)
       workThr_ = std::thread([](scriptPanel* sp){ sp->workCycle(); }, this);
}

scriptPanel::~scriptPanel(){

    isStopWork_ = true;
    if (workThr_.joinable()) workThr_.join();
}

void scriptPanel::addScript(QString name){
      
    std::unique_lock<std::mutex> lck(mtx_);

    bool isNew = name.isEmpty();
    if (isNew)
        name = "NewScript.lua";
    
    name = exlName(name);

    int rowCnt = ui.tblScripts->rowCount();
    ui.tblScripts->insertRow(rowCnt);

    ui.tblScripts->setItem(rowCnt, 0, new QTableWidgetItem(name));
       
    int nameMaxWidth = 150;
    for (int i = 0; i < (rowCnt + 1); ++i){
       
        int nameFontMetr = int(this->fontMetrics().width(ui.tblScripts->item(i, 0)->text()) * 1.5);
        
        nameMaxWidth = qMax(nameFontMetr, nameMaxWidth);       
    }

    ui.tblScripts->setColumnWidth(0, nameMaxWidth);
  
    if (isNew){

        auto te = new QTextEdit();
        te->setFrameShape(QFrame::NoFrame);
        int tinx = ui.tabWidget->addTab(te, name);
        ui.tabWidget->setCurrentIndex(tinx);

        connect(te, &QTextEdit::textChanged, [this, rowCnt](){
            scrState_[rowCnt].isChange = true;
            ui.lbChange->setText("*");
        });

        scrState_.push_back(scriptState(name, "", tinx));

    }
    else{

        QFile file(QApplication::applicationDirPath() + "/scripts/" + name);

        QTextStream txtStream(&file);

        file.open(QIODevice::ReadOnly);

        QString text = txtStream.readAll();

        file.close();

        scrState_.push_back(scriptState(name, text));
    }
}

void scriptPanel::nameScriptChange(int row, int col){

    if ((col == 0) && (row < scrState_.size())){

        QString sname = ui.tblScripts->item(row, 0)->text();

        if (std::find_if(scrState_.begin(), scrState_.end(),

            [sname](const scriptState& st) {

            return st.name == sname;

        }) != scrState_.end())
        {
            ui.lbStatusMess->setText(tr("Скрипт с таким именем уже существует"));

            ui.tblScripts->item(row, 0)->setText(scrState_[row].name);
            return;
        }

        scrState_[row].name = sname;

        if (scrState_[row].tabInx >= 0)
            ui.tabWidget->setTabText(scrState_[row].tabInx, scrState_[row].name);

        saveScript();
    }
}

void scriptPanel::saveScript(){

    if (ui.tabWidget->currentIndex() < 0)
        return;

    QString sname = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
    auto it = std::find_if(scrState_.begin(), scrState_.end(),

        [sname](const scriptState& st) {

           return st.name == sname;
        }
    );

    QString script = ((QTextEdit*)ui.tabWidget->currentWidget())->toPlainText();

    /// TODO CHECK



    //////////

    it->text = script;
    it->isChange = false;

    mtx_.lock();

    QFile file(QApplication::applicationDirPath() + "/scripts/" + sname);

    QTextStream txtStream(&file);
    
    file.open(QIODevice::WriteOnly);

    txtStream << it->text;

    file.close();

    mtx_.unlock();

    ui.lbChange->setText("");
    ui.lbStatusMess->setText("");

   
}

QString scriptPanel::exlName(QString name){

    int ptrInx = name.lastIndexOf(".");
    QString begName = name.left(ptrInx),
            endName = ptrInx > 0 ? name.right(name.size() - ptrInx) : "";

    int rowCnt = ui.tblScripts->rowCount(), newScpCnt = 0;
    for (int i = 0; i < rowCnt; ++i){

        if (ui.tblScripts->item(i, 0)->text() == name){
            ++newScpCnt;

            name = begName + QString::number(newScpCnt) + endName;
            i = -1;
        }
    }

    return name;
}

void scriptPanel::workCycle(){

    QString path = QApplication::applicationDirPath() + "/scripts/";

    SV_Aux::TimerDelay tmDelay;
    tmDelay.UpdateCycTime();

    while (!isStopWork_){

        tmDelay.UpdateCycTime();

        mtx_.lock();

        cTm_ = SV_Aux::CurrDateTimeSinceEpochMs();
        iterValue_ = 0;
        for (int i = 0; i < SV_PACKETSZ; ++i){

            for (auto& s : scrState_){
                if (s.name != "load.lua"){
                    luaL_loadfile(luaState_, qPrintable(path + s.name));

                    lua_pcall(luaState_, 0, 0, 0);
                }
            }
            ++iterValue_;
        }

        if ((scrState_.size() > 1) && pfUpdateSignalsCBack)
            pfUpdateSignalsCBack();

        mtx_.unlock();

        int ms = SV_CYCLESAVE_MS - (int)tmDelay.GetCTime();
        if (ms > 0)
            SV_Aux::SleepMs(ms);
    }
}
