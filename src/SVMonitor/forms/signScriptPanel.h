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
#pragma once

#include "src/stdafx.h"
#include "SVScriptPanel/SVScriptPanel.h"
#include "SVConfig/config_limits.h"
#include "forms/ui_signScriptPanel.h"

class signScriptPanel : public QDialog
{
    Q_OBJECT

public:
    signScriptPanel(QWidget* parent, QDialog* mainScrPanel){

#ifdef SV_EN
        QTranslator translator;
        translator.load(":/SVMonitor/svmonitor_en.qm");
        QCoreApplication::installTranslator(&translator);
#endif
        mainScrPanel_ = mainScrPanel;

        setParent(parent);

        ui.setupUi(this);

        connect(ui.btnSave, &QPushButton::clicked, this, [this] {

            saveScript();
        });

        connect(ui.btnActive, &QPushButton::clicked, this, [this] {

            if (SV_Script::isActiveScript(mainScrPanel_, sFileName_)){

                SV_Script::deactiveScript(mainScrPanel_, sFileName_);
                
                ui.btnActive->setText(tr("Включить"));
            }
            else{

                SV_Script::activeScript(mainScrPanel_, sFileName_);

                ui.btnActive->setText(tr("Отключить"));
            }
        });

        connect(ui.txtScript, &QTextEdit::textChanged, [this](){
            ui.lbChange->setText("*");
        });
             
    }
    ~signScriptPanel() = default;

    void showSignScript(const QString& signal, const QString& module, SV_Base::ValueType type){

        sFileName_ = signal + module + ".lua";

        this->setWindowTitle(tr("Скрипт ") + sFileName_);
                       
        QFile file(QApplication::applicationDirPath() + "/scripts/" + sFileName_);

        if (file.exists()){

            QTextStream txtStream(&file);
            txtStream.setCodec("utf8");

            file.open(QIODevice::ReadOnly);

            ui.txtScript->setText(txtStream.readAll());

            file.close();
        }
        else{
          
            QString stext = "ctm = getTimeValue(\"" + module + "\", \"" + signal + "\"); \n";

            if (type == SV_Base::ValueType::BOOL)     stext += "cval = getBoolValue(";
            else if (type == SV_Base::ValueType::INT) stext += "cval = getIntValue(";
            else                                      stext += "cval = getFloatValue(";
            
            stext += "\"" + module + "\", \"" + signal + "\");";

            stext += "\n\n\n";

            stext += "valOffs = 0;\n";
            stext += "tmOffs = 0;\n";                     

            stext += "\n\n";

            if (type == SV_Base::ValueType::BOOL)     stext += "setBoolValue(";
            else if (type == SV_Base::ValueType::INT) stext += "setIntValue(";
            else                                      stext += "setFloatValue(";

            stext += "\"" + signal + "Virt" + "\", cval + valOffs, ctm + tmOffs);";
          

            ui.txtScript->setText(stext);

            QTextStream txtStream(&file);
            txtStream.setCodec("utf8");

            file.open(QIODevice::WriteOnly);

            txtStream << stext;

            file.close();
        }

        ui.lbChange->setText("");
        
        if (SV_Script::isActiveScript(mainScrPanel_, sFileName_))                       
            ui.btnActive->setText(tr("Отключить"));
        else
            ui.btnActive->setText(tr("Включить"));

        show();        
    }

private:

    Ui::signScriptPanelClass ui;
        
    QString sFileName_;

    QDialog* mainScrPanel_ = nullptr;
    
    void closeEvent(QCloseEvent* e){

        if (!ui.lbChange->text().isEmpty()){
            QMessageBox msgBox;
            msgBox.setText(tr("Скрипт ") + sFileName_ + tr(" изменен."));
            msgBox.setInformativeText(tr("Сохранить изменения?"));
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Save);

            if (msgBox.exec() == QMessageBox::Save){

                saveScript();                
            }
        }
    }

    void saveScript(){

        if (ui.lbChange->text().isEmpty()) return;

        bool isActiveScript = SV_Script::isActiveScript(mainScrPanel_, sFileName_);

        if (isActiveScript)
            SV_Script::deactiveScript(mainScrPanel_, sFileName_);

        QFile file(QApplication::applicationDirPath() + "/scripts/" + sFileName_);

        QTextStream txtStream(&file);
        txtStream.setCodec("utf8");

        file.open(QIODevice::WriteOnly);

        txtStream << ui.txtScript->toPlainText();

        file.close();

        ui.lbChange->setText("");

        SV_Script::refreshScript(mainScrPanel_, sFileName_);

        if (isActiveScript)
            SV_Script::activeScript(mainScrPanel_, sFileName_);

    }
};



