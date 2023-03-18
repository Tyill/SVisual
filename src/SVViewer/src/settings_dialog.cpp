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

#include "SVViewer/forms/settings_dialog.h"
#include "SVViewer/forms/main_win.h"

#include <QComboBox>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent){

  setParent(parent);

  mainWin_ = (MainWin*)parent;

  ui.setupUi(this);

  connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(saveChange()));
  connect(ui.rbtnChEna, &QCheckBox::toggled, this, [this](){

    bool isSel = ui.rbtnChEna->isChecked();

    ui.txtChAddr->setEnabled(isSel);
    ui.txtChName->setEnabled(isSel);
    paramChange();
  });

  connect(ui.txtChName, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtChAddr, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.spinCycleRecMs, SIGNAL(valueChanged(QString)), this, SLOT(paramChange()));
  connect(ui.spinPacketSz, SIGNAL(valueChanged(QString)), this, SLOT(paramChange()));

  MainWin::Config cng = mainWin_->getConfig();  
}

SettingsDialog::~SettingsDialog(){}

void SettingsDialog::showEvent(QShowEvent * event){

  MainWin::Config cng = mainWin_->getConfig();

  ui.rbtnChEna->setChecked(cng.inputDataBaseEna);
  ui.txtChName->setText(cng.inputDataBaseName);
  ui.txtChAddr->setText(cng.inputDataBaseAddr);

  ui.spinCycleRecMs->setValue(cng.cycleRecMs);
  ui.spinPacketSz->setValue(cng.packetSz);

  selParamLoad_ = false;
  ui.lbChange->setText("");
}

void SettingsDialog::saveChange(){

  MainWin::Config cng = mainWin_->getConfig();

  cng.inputDataBaseEna = ui.rbtnChEna->isChecked();
  cng.inputDataBaseName = ui.txtChName->text();
  cng.inputDataBaseAddr = ui.txtChAddr->text();

  cng.cycleRecMs = ui.spinCycleRecMs->value();
  cng.packetSz = ui.spinPacketSz->value();

  mainWin_->updateConfig(cng);

  if (selParamLoad_)
    ui.lbChange->setText(tr("Изменения вступят в силу после перезагрузки"));
  else
    ui.lbChange->setText("");

}

void SettingsDialog::paramChange(){

  selParamLoad_ = true;
  ui.lbChange->setText("*");
}
