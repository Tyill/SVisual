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

#include "SVMonitor/forms/settings_dialog.h"
#include "SVMonitor/forms/main_win.h"

#include <QComboBox>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent):
    QDialog(parent)
{
  mainWin_ = (MainWin*)parent;

  ui.setupUi(this);

  connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(saveChange()));
  connect(ui.btnArchPath, SIGNAL(clicked()), this, SLOT(selDirArch()));
  connect(ui.rbtnConnectByCom, &QRadioButton::toggled, this, [this](){

    bool isSel = ui.rbtnConnectByCom->isChecked();

    ui.btnAddCOM->setEnabled(isSel);
    ui.btnDelCOM->setEnabled(isSel);

    for (auto& com : comPorts_){
      com.first->setEnabled(isSel);
      com.second->setEnabled(isSel);
    }

    selParamLoad_ = true;
    paramChange();
  });
  connect(ui.rbtnConnectByEthernet, &QRadioButton::toggled, this, [this](){

    bool isSel = ui.rbtnConnectByEthernet->isChecked();

    ui.txtIPAddr->setEnabled(isSel);
    ui.txtTCPPort->setEnabled(isSel);

    selParamLoad_ = true;
    paramChange();
  });
  connect(ui.chbArchEna, &QCheckBox::toggled, this, [this](){

    bool isSel = ui.chbArchEna->isChecked();

    ui.txtArchPath->setEnabled(isSel);
    ui.btnArchPath->setEnabled(isSel);
    paramChange();
  });
  connect(ui.chbSaveToChEna, &QCheckBox::toggled, this, [this](){

    bool isSel = ui.chbSaveToChEna->isChecked();

    ui.txtChName->setEnabled(isSel);
    ui.txtChAddr->setEnabled(isSel);
    paramChange();
  });
  connect(ui.chbWebActive, SIGNAL(stateChanged(int)), this, SLOT(paramChange()));
  connect(ui.chbZabbixActive, SIGNAL(stateChanged(int)), this, SLOT(paramChange()));
  connect(ui.btnAddCOM, SIGNAL(clicked()), this, SLOT(addCOM()));
  connect(ui.btnDelCOM, SIGNAL(clicked()), this, SLOT(delCOM()));

  connect(ui.txtIPAddr, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtTCPPort, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtWebIPAddr, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtWebPort, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtZabbixIPAddr, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtZabbixPort, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtArchPath, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtChName, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.txtChAddr, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  connect(ui.spinCycleRecMs, SIGNAL(valueChanged(QString)), this, SLOT(paramChange()));
  connect(ui.spinPacketSz, SIGNAL(valueChanged(QString)), this, SLOT(paramChange()));
  connect(ui.spinTimeOffsetSec, SIGNAL(valueChanged(QString)), this, SLOT(paramChange()));

  MainWin::Config cng = mainWin_->getConfig();

  for (int i = 0; i < cng.com_ports.size(); ++i)
    addCOM(cng.com_ports[i].first, cng.com_ports[i].second, i + 1);

}

SettingsDialog::~SettingsDialog(){}

void SettingsDialog::showEvent(QShowEvent * event){

  MainWin::Config cng = mainWin_->getConfig();

  ui.txtIPAddr->setText(cng.tcp_addr);
  ui.txtTCPPort->setText(QString::number(cng.tcp_port));

  ui.chbWebActive->setChecked(cng.web_ena);
  ui.txtWebIPAddr->setText(cng.web_addr);
  ui.txtWebPort->setText(QString::number(cng.web_port));

  ui.chbZabbixActive->setChecked(cng.zabbix_ena);
  ui.txtZabbixIPAddr->setText(cng.zabbix_addr);
  ui.txtZabbixPort->setText(QString::number(cng.zabbix_port));

  ui.rbtnConnectByEthernet->setChecked(!cng.com_ena);

  ui.chbArchEna->setChecked(cng.outArchiveEna);
  ui.txtArchPath->setText(cng.outArchivePath);

  ui.chbSaveToChEna->setChecked(cng.outDataBaseEna);
  ui.txtChName->setText(cng.outDataBaseName);
  ui.txtChAddr->setText(cng.outDataBaseAddr);

  ui.spinCycleRecMs->setValue(cng.cycleRecMs);
  ui.spinPacketSz->setValue(cng.packetSz);
  ui.spinTimeOffsetSec->setValue(cng.offsetMs / 1000);

  selParamLoad_ = false;
  ui.lbChange->setText("");
}

void SettingsDialog::addCOM(QString port, QString speed, int setRow){

  int row = ui.gridLayout->rowCount() + 1;

  if (setRow >= 0) row = setRow;

  bool isEna = ui.rbtnConnectByCom->isChecked();

  auto label = new QLabel(ui.groupBox);
  label->setText(tr("порт"));
  label->setMinimumSize(QSize(50, 0));

  ui.gridLayout->addWidget(label, row, 2, 1, 1);

  auto txtComPort = new QLineEdit(ui.groupBox);
  txtComPort->setMinimumSize(QSize(100, 0));
  txtComPort->setMaximumSize(QSize(100, 16777215));
  txtComPort->setText(port);
  connect(txtComPort, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  txtComPort->setEnabled(isEna);

  ui.gridLayout->addWidget(txtComPort, row, 3, 1, 1);

  label = new QLabel(ui.groupBox);
  label->setText(tr("скорость"));
  label->setMinimumSize(QSize(55, 0));

  ui.gridLayout->addWidget(label, row, 5, 1, 1);

  auto cbxComSpeed = new QComboBox(ui.groupBox);
  cbxComSpeed->clear();
  cbxComSpeed->insertItems(0, QStringList()
    << "1200"
    << "2400"
    << "4800"
    << "9600"
    << "19200"
    << "38400"
    << "57600"
    << "115200"
    );
  cbxComSpeed->setCurrentText(speed);
  connect(cbxComSpeed, SIGNAL(currentIndexChanged(int)), this, SLOT(paramChange()));
  cbxComSpeed->setEnabled(isEna);

  ui.gridLayout->addWidget(cbxComSpeed, row, 6, 1, 1);

  comPorts_.push_back(qMakePair(txtComPort, cbxComSpeed));

  selParamLoad_ = true;
  ui.lbChange->setText("*");

  auto geom = this->geometry();
  this->setGeometry(QRect(geom.x(), geom.y(), geom.width(), geom.height() + 34));
}

void SettingsDialog::delCOM(){

  int i = comPorts_.size() - 1;
  while (i >= 0){

    if (!comPorts_[i].first->text().isEmpty()){
      comPorts_[i].first->clear();
      break;
    }
    --i;
  }

  selParamLoad_ = true;
  ui.lbChange->setText("*");
}

void SettingsDialog::selDirArch(){

  QString fl = QFileDialog::getExistingDirectory(this,
    tr("Выбор пути сохранения файлов записи"), selDirArch_);

  if (fl.isEmpty()) return;

  ui.txtArchPath->setText(fl + "/");

  selDirArch_ = fl;
}

void SettingsDialog::saveChange(){

  MainWin::Config cng = mainWin_->getConfig();

  cng.tcp_addr = ui.txtIPAddr->text();
  cng.tcp_port = ui.txtTCPPort->text().toInt();

  cng.web_ena = ui.chbWebActive->isChecked();
  cng.web_addr = ui.txtWebIPAddr->text();
  cng.web_port = ui.txtWebPort->text().toInt();

  cng.zabbix_ena = ui.chbZabbixActive->isChecked();
  cng.zabbix_addr = ui.txtZabbixIPAddr->text();
  cng.zabbix_port = ui.txtZabbixPort->text().toInt();

  cng.outArchiveEna = ui.chbArchEna->isChecked();
  cng.outArchivePath = ui.txtArchPath->text();
  cng.outArchivePath.replace("\\", "/");

  cng.outDataBaseEna = ui.chbSaveToChEna->isChecked();
  cng.outDataBaseName = ui.txtChName->text();
  cng.outDataBaseAddr = ui.txtChAddr->text();

  cng.com_ena = ui.rbtnConnectByCom->isChecked();

  cng.com_ports.clear();
  for (auto& port : comPorts_){
    cng.com_ports.push_back(qMakePair(port.first->text(), port.second->currentText()));
  }

  cng.cycleRecMs = ui.spinCycleRecMs->value();
  cng.packetSz = ui.spinPacketSz->value();
  cng.offsetMs = ui.spinTimeOffsetSec->value() * 1000;

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
