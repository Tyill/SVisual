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
#include "SVAuxFunc/Front.h"
#include "SVAuxFunc/auxFunc.h"

using namespace SV_Cng;
namespace lub = luabridge;

scriptPanel* scrPanelRef = nullptr;

void printMess(const std::string& mess){

    if ((scrPanelRef->buffCPos_ == 0) && (scrPanelRef->iterValue_ == 0)){
        QString qmess = QString::fromStdString(SV_Aux::CurrDateTime()) + " " + mess.c_str();
        QMetaObject::invokeMethod(scrPanelRef->ui.txtStatusMess, "append", Qt::AutoConnection, Q_ARG(QString, qmess));
    }
}

uint64_t getTimeValue(const std::string& module, const std::string& signal){

    QString md = qUtf8Printable(module.c_str()),
            sn = qUtf8Printable(signal.c_str()),
            sign = sn + md;
    if (scrPanelRef->updateBuffValue(md, sn, SV_Cng::valueType::tBool)){
     
        if (scrPanelRef->mode_ == SV_Script::modeGr::player)
            return scrPanelRef->signBuff_[sign]->lastData.beginTime;
        else{
            if ((scrPanelRef->buffCPos_ == 0) && (scrPanelRef->iterValue_ == 0))
               scrPanelRef->buffSz_ = qMax(scrPanelRef->buffSz_, int(scrPanelRef->signBuff_[sign]->buffData.size()));

            int inx = qMin(scrPanelRef->buffCPos_, int(scrPanelRef->signBuff_[sign]->buffData.size() - 1));

            return scrPanelRef->signBuff_[sign]->buffData[inx].beginTime;
        }
    }
    else
        return 0;
}

bool getBoolValue(const std::string& module, const std::string& signal){

    QString md = qUtf8Printable(module.c_str()),
            sn = qUtf8Printable(signal.c_str()),
            sign = sn + md;
    if (scrPanelRef->updateBuffValue(md, sn, SV_Cng::valueType::tBool)){
        
        if (scrPanelRef->mode_ == SV_Script::modeGr::player)
            return scrPanelRef->signBuff_[sign]->lastData.vals[scrPanelRef->iterValue_].tBool;
        else{
            if ((scrPanelRef->buffCPos_ == 0) && (scrPanelRef->iterValue_ == 0))
                scrPanelRef->buffSz_ = qMax(scrPanelRef->buffSz_, int(scrPanelRef->signBuff_[sign]->buffData.size()));

            int inx = qMin(scrPanelRef->buffCPos_, int(scrPanelRef->signBuff_[sign]->buffData.size() - 1));

            return scrPanelRef->signBuff_[sign]->buffData[inx].vals[scrPanelRef->iterValue_].tBool;
        }
    }
    else
        return false;
}

int getIntValue(const std::string& module, const std::string& signal){

    QString md = qUtf8Printable(module.c_str()),
            sn = qUtf8Printable(signal.c_str()),
            sign = sn + md;
    if (scrPanelRef->updateBuffValue(md, sn, SV_Cng::valueType::tInt)){

        if (scrPanelRef->mode_ == SV_Script::modeGr::player)
            return scrPanelRef->signBuff_[sign]->lastData.vals[scrPanelRef->iterValue_].tInt;
        else{
            if ((scrPanelRef->buffCPos_ == 0) && (scrPanelRef->iterValue_ == 0))
               scrPanelRef->buffSz_ = qMax(scrPanelRef->buffSz_, int(scrPanelRef->signBuff_[sign]->buffData.size()));

            int inx = qMin(scrPanelRef->buffCPos_, int(scrPanelRef->signBuff_[sign]->buffData.size() - 1));

            return scrPanelRef->signBuff_[sign]->buffData[inx].vals[scrPanelRef->iterValue_].tInt;
        }
    }
    else
        return 0;
}

float getFloatValue(const std::string& module, const std::string& signal){

    QString md = qUtf8Printable(module.c_str()),
            sn = qUtf8Printable(signal.c_str()),
            sign = sn + md;
    if (scrPanelRef->updateBuffValue(md, sn, SV_Cng::valueType::tInt)){

        if (scrPanelRef->mode_ == SV_Script::modeGr::player)
            return scrPanelRef->signBuff_[sign]->lastData.vals[scrPanelRef->iterValue_].tFloat;
        else{
            if ((scrPanelRef->buffCPos_ == 0) && (scrPanelRef->iterValue_ == 0))
               scrPanelRef->buffSz_ = qMax(scrPanelRef->buffSz_, int(scrPanelRef->signBuff_[sign]->buffData.size()));

            int inx = qMin(scrPanelRef->buffCPos_, int(scrPanelRef->signBuff_[sign]->buffData.size() - 1));

            return scrPanelRef->signBuff_[sign]->buffData[inx].vals[scrPanelRef->iterValue_].tFloat;
        }
    }
    else
        return 0.F;
}

void setBoolValue(const std::string& signal, bool bval, uint64_t time){

    QString md = "Virtual",
            sn = qUtf8Printable(signal.c_str()),
            sign = sn + md;
      
   SV_Cng::value val;
   val.tBool = bval;

   if (scrPanelRef->updateBuffValue(md, sn, SV_Cng::valueType::tBool))
       scrPanelRef->setValue(sign, val, time);
}
     
void setIntValue(const std::string& signal, int ival, uint64_t time){

    QString md = "Virtual",
            sn = qUtf8Printable(signal.c_str()),
            sign = sn + md;

    SV_Cng::value val;
    val.tInt = ival;

    if (scrPanelRef->updateBuffValue(md, sn, SV_Cng::valueType::tInt))
        scrPanelRef->setValue(sign, val, time);
}
     
void setFloatValue(const std::string& signal, float fval, uint64_t time){

    QString md = "Virtual",
            sn = qUtf8Printable(signal.c_str()),
            sign = sn + md;

    SV_Cng::value val;
    val.tFloat = fval;

    if (scrPanelRef->updateBuffValue(md, sn, SV_Cng::valueType::tFloat))
        scrPanelRef->setValue(sign, val, time);
}

void scriptPanel::setValue(const QString& sign, SV_Cng::value val, uint64_t time){

    auto sd = signBuff_[sign];    
     
    sd->lastData.vals[iterValue_] = val;

    // заполняем буфер
    int vp = (mode_ == SV_Script::modeGr::player) ? sd->buffValuePos : buffCPos_;
    
    sd->buffData[vp].vals[iterValue_] = val;
    
    if (iterValue_ == (SV_PACKETSZ - 1)){

        sd->lastData.beginTime = time;
        sd->buffData[vp].beginTime = time;

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
        else{

            sd->buffValuePos = vp;
            
            if (vp >= sd->buffData.size()){
                SV_Cng::recData rd;

                rd.beginTime = time;
                rd.vals = new SV_Cng::value[SV_PACKETSZ];

                sd->buffData.push_back(rd);
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

bool scriptPanel::updateBuffValue(const QString& module, const QString& signal, SV_Cng::valueType stype){

    QString sign = signal + module;
             
    if (signBuff_.contains(sign))
        return true;
    
    if (module == "Virtual"){

        if (!pfAddSignal || !pfAddModule || !pfGetModuleData || !pfLoadSignalData)
            return false;

        signalData* sd = new signalData();
        signBuff_[sign] = sd;

        sd->isActive = true;
        sd->isBuffEnable = false;
        sd->isDelete = false;

        sd->name = qUtf8Printable(signal);
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
            md->isActive = true;
            md->isDelete = false;
            md->isEnable = true;
            pfAddModule("Virtual", md);

            if (pfModuleConnectCBack)
                pfModuleConnectCBack("Virtual");
        }

        md->signls.push_back(qUtf8Printable(sign));
           
        pfAddSignal(sign, sd);

        if (mode_ == SV_Script::modeGr::player)
            pfLoadSignalData(sign);
        else{
            SV_Cng::recData rd;
            rd.vals = new SV_Cng::value[SV_PACKETSZ];

            sd->buffData.push_back(rd);
        }

        if (pfAddSignalsCBack)
            pfAddSignalsCBack();
    }
    else{

        if (!pfGetCopySignalRef || !pfLoadSignalData)
            return false;

        auto signRef = pfGetCopySignalRef();

        if (!signRef.contains(sign) || !pfLoadSignalData(sign))
            return false;

        signBuff_[sign] = signRef[sign];
    }

    return true;
}

/////////////////////////////////

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
    connect(ui.btnSetActive, &QPushButton::clicked, [this](){

        auto items = ui.tblScripts->selectedItems();
        for (auto it : items){
            QString sname = it->text();
            auto sts = std::find_if(scrState_.begin(), scrState_.end(),
                [sname](const scriptState& st) {
                   return st.name == sname;
                }
            );
            
            if (!sts->isActive){
                int rowCnt = ui.tblActiveScripts->rowCount();
                ui.tblActiveScripts->insertRow(rowCnt);
                auto itm = new QTableWidgetItem(sname);
                itm->setFlags(itm->flags() ^ Qt::ItemFlag::ItemIsEditable);
                ui.tblActiveScripts->setItem(rowCnt, 0, itm);

                sts->isActive = true;
            }            
        }
    });
    connect(ui.btnResetActive, &QPushButton::clicked, [this](){

        auto items = ui.tblActiveScripts->selectedItems();
        for (auto it : items){
            QString sname = it->text();
            std::find_if(scrState_.begin(), scrState_.end(),
                [sname](const scriptState& st) {
                return st.name == sname;
            }
            )->isActive = false;
            
            ui.tblActiveScripts->removeRow(it->row());
        }
    });
    connect(ui.btnClear, &QPushButton::clicked, [this](){

        ui.txtStatusMess->clear();
    });
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
        
            if (scrState_[i].tabInx == inx)
                scrState_[i].tabInx = -1;
            else if (scrState_[i].tabInx > inx)
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

    ss[0] = 300; ss[1] = 100;
    ui.splitter_2->setSizes(ss);

    ui.tblScripts->setColumnWidth(0, 150);
    ui.tblActiveScripts->setColumnWidth(0, 150);
     
}

scriptPanel::~scriptPanel(){

    isStopWork_ = true;
    if (workThr_.joinable()) workThr_.join();
}

void scriptPanel::contextMenuEvent(QContextMenuEvent * event){
        
    if (!qobject_cast<QTableWidget*>(focusWidget()) || 
        (qobject_cast<QTableWidget*>(focusWidget())->objectName() != "tblScripts")) return;
    
        QTableWidgetItem* item = ui.tblScripts->currentItem();
        if (!item) return;

        QMenu* menu = new QMenu(this);

        menu->addAction(tr("Удалить"));

        connect(menu, &QMenu::triggered, [this, item](QAction*){
            std::unique_lock<std::mutex> lck(mtx_);
            
            int row = item->row();
            QString scrName = ui.tblScripts->item(row, 0)->text();
            
            if (scrState_[row].tabInx >= 0){
                ui.tabWidget->removeTab(scrState_[row].tabInx);

                for (int i = 0; i < scrState_.size(); ++i)
                    if (scrState_[i].tabInx > scrState_[row].tabInx)
                        --scrState_[i].tabInx;
            }
            scrState_.remove(row);
            ui.tblScripts->removeRow(row);
                        
            int sz = ui.tblActiveScripts->rowCount();
            for (int i = 0; i < sz; ++i){
                if (ui.tblActiveScripts->item(i, 0)->text() == scrName){
                    ui.tblActiveScripts->removeRow(i);
                    break;
                }
            }

            QFile::remove(QApplication::applicationDirPath() + "/scripts/" + scrName);
        });
               
        menu->exec(event->globalPos());
                    
}

void scriptPanel::startUpdateThread(){

    if (workThr_.joinable()) return;

    luaState_ = luaL_newstate();
    luaL_openlibs(luaState_);

    lub::getGlobalNamespace(luaState_)
        .addFunction("printMess", printMess)
        .addFunction("getTimeValue", getTimeValue)
        .addFunction("getBoolValue", getBoolValue)
        .addFunction("getIntValue", getIntValue)
        .addFunction("getFloatValue", getFloatValue)
        .addFunction("setBoolValue", setBoolValue)
        .addFunction("setIntValue", setIntValue)
        .addFunction("setFloatValue", setFloatValue);
        
    workThr_ = std::thread([](scriptPanel* sp){ sp->workCycle(); }, this);
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
        txtStream.setCodec("utf8");

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
            ui.txtStatusMess->append(QString::fromStdString(SV_Aux::CurrDateTime()) + " " + tr("Скрипт с таким именем уже существует"));

            ui.tblScripts->item(row, 0)->setText(scrState_[row].name);
            return;
        }
                
        if (scrState_[row].tabInx >= 0)
            ui.tabWidget->setTabText(scrState_[row].tabInx, sname);

        int sz = ui.tblActiveScripts->rowCount();
        for (int i = 0; i < sz; ++i){
            if (ui.tblActiveScripts->item(i, 0)->text() == scrState_[row].name){
                ui.tblActiveScripts->item(i, 0)->setText(sname);
                break;
            }
        }

        QFile::rename(QApplication::applicationDirPath() + "/scripts/" + scrState_[row].name,
                      QApplication::applicationDirPath() + "/scripts/" + sname);

        scrState_[row].name = sname;
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
   
    if (it->isChange && mtx_.try_lock()){

        QFile file(QApplication::applicationDirPath() + "/scripts/" + sname);

        QTextStream txtStream(&file);
        txtStream.setCodec("utf8");

        file.open(QIODevice::WriteOnly);

        it->text = ((QTextEdit*)ui.tabWidget->currentWidget())->toPlainText();
        it->isChange = false;
                
        txtStream << it->text;

        file.close();

        buffCPos_ = 0;

        mtx_.unlock();
                
        ui.lbChange->setText("");
    }
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
    
    SV_Aux::Front fp;
    SV_Aux::TimerDelay tmDelay;
    tmDelay.UpdateCycTime();
          
    while (!isStopWork_){

        tmDelay.UpdateCycTime();

        mtx_.lock();

        QString serr;
        
        // load.lua
        auto loadScr = std::find_if(scrState_.begin(), scrState_.end(),
            [](const scriptState& st) {
            return st.name == "load.lua";
        });
        
        if ((loadScr != scrState_.end()) && fp.PosFront(loadScr->isActive, 0)){

            luaL_loadfile(luaState_, qPrintable(path + loadScr->name));

            lua_pcall(luaState_, 0, 0, 0);

            const char* err = lua_tostring(luaState_, -1);
            if (err && serr.isEmpty()){
                serr = QString(err);
                lua_pop(luaState_, -1);
            }
        }

        bool isActive = false, 
             isNewCycle = (buffCPos_ == 0);

        do {           
            // other scripts
            for (iterValue_ = 0; iterValue_ < SV_PACKETSZ; ++iterValue_){

                for (auto& s : scrState_){
                    if (s.isActive && (s.name != "load.lua")){
                        isActive = true;

                        luaL_loadfile(luaState_, qPrintable(path + s.name));

                        lua_pcall(luaState_, 0, 0, 0);

                        const char* err = lua_tostring(luaState_, -1);
                        if (err && serr.isEmpty()){
                            serr = QString(err);
                            lua_pop(luaState_, -1);
                        }
                    }
                }
            }
            iterValue_ = 0;
            ++buffCPos_;
        } while (buffCPos_ < buffSz_);
               
        buffCPos_ = qMax(0, buffSz_ - 1);
     
        mtx_.unlock();
     
        if (isActive && isNewCycle && pfUpdateSignalsCBack)
            pfUpdateSignalsCBack();

        if (!serr.isEmpty()){
            QString qmess = QString::fromStdString(SV_Aux::CurrDateTime()) + " " + serr;
            QMetaObject::invokeMethod(scrPanelRef->ui.txtStatusMess, "append", Qt::AutoConnection, Q_ARG(QString, qmess));
        }

        int ms = SV_CYCLESAVE_MS - (int)tmDelay.GetCTime();
        if (ms > 0)
            SV_Aux::SleepMs(ms);
    }
}
