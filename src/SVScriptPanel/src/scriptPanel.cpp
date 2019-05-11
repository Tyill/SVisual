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
#include "forms/scriptPanel.h"
#include "SVConfig/SVConfigLimits.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"


using namespace SV_Cng;
namespace lub = luabridge;

bool getBoolValue(const std::string& name){

    return false;
}

int getIntValue(const std::string& name){

    return 0;
}

float getFloatValue(const std::string& name){

    return 0;
}

void setBoolValue(const std::string& name, bool value){

    bool ok = true;
}
     
void setIntValue(const std::string& name, int value){

    bool ok = true;
}
     
void setFloatValue(const std::string& name, float value){

    bool ok = true;
}

scriptPanel::scriptPanel(QWidget *parent, SV_Script::config cng_){
		
	setParent(parent);
	
#ifdef SV_EN
	QTranslator translator;
	translator.load(":/SVScript/svscriptpanel_en.qm");
	QCoreApplication::installTranslator(&translator);
#endif
    	
	cng = cng_;

	ui.setupUi(this);
               
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

    luaState_ = luaL_newstate();
    luaL_openlibs(luaState_);

    lub::getGlobalNamespace(luaState_)                
                .addFunction("getBoolValue", getBoolValue)
                .addFunction("getIntValue", getIntValue)
                .addFunction("getFloatValue", getFloatValue)
                .addFunction("setBoolValue", setBoolValue)
                .addFunction("setIntValue", setIntValue)
                .addFunction("setFloatValue", setFloatValue);
       
    luaL_dofile(luaState_, (QApplication::applicationDirPath() + "/scripts/load.lua").toStdString().c_str());    
    lua_pcall(luaState_, 0, 0, 0);   
}

scriptPanel::~scriptPanel(){


}

void scriptPanel::addScript(QString name){
      
    bool isNew = name.isEmpty();
    if (isNew)
        name = "NewScript.lua";
    
    name = exlName(name);

    int rowCnt = ui.tblScripts->rowCount();
    ui.tblScripts->insertRow(rowCnt);

    ui.tblScripts->setItem(rowCnt, 0, new QTableWidgetItem(name));
       
    int nameMaxWidth = 0;
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

  
    QFile file(QApplication::applicationDirPath() + "/scripts/" + sname);

    QTextStream txtStream(&file);
    
    file.open(QIODevice::WriteOnly);

    txtStream << it->text;

    file.close();

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

        ui.lbStatusMess->setText("");
    }
}
