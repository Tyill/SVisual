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
#include "forms/settingsPanel.h"
#include "forms/mainwin.h"
#include "SVServer/SVServer.h"

settingsPanel::settingsPanel(QWidget *parent){

	setParent(parent);

	mainWin_ = (MainWin*)parent;

	ui.setupUi(this);
		
	connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(saveChange()));
	connect(ui.btnCopyPath, SIGNAL(clicked()), this, SLOT(selDirCopy()));
	connect(ui.txtComPort, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	connect(ui.rbtnConnectByCom, &QRadioButton::clicked, this, [this] (){
		
		bool isSel = ui.rbtnConnectByCom->isChecked();

		ui.txtComSpeed->setEnabled(isSel);
		ui.txtComPort->setEnabled(isSel);

		ui.txtIPAddr->setEnabled(!isSel);
		ui.txtTCPPort->setEnabled(!isSel);

		selParamLoad_ = true;		
		paramChange();
	});
	connect(ui.rbtnConnectByEthernet, &QRadioButton::clicked, this, [this](){

		bool isSel = ui.rbtnConnectByEthernet->isChecked();

		ui.txtIPAddr->setEnabled(isSel);
		ui.txtTCPPort->setEnabled(isSel);

		ui.txtComSpeed->setEnabled(!isSel);
		ui.txtComPort->setEnabled(!isSel);

		selParamLoad_ = true;
		paramChange();
	});
	connect(ui.txtComSpeed, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	connect(ui.rbtnCopyEna, &QRadioButton::clicked, this, [this](){

		bool isSel = ui.rbtnCopyEna->isChecked();

		ui.txtCopyPath->setEnabled(isSel);
		paramChange();
	});

	connect(ui.txtIPAddr, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	connect(ui.txtTCPPort, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	connect(ui.txtCopyPath, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	
}

settingsPanel::~settingsPanel(){}

void settingsPanel::showEvent(QShowEvent * event){

	MainWin::config cng = mainWin_->getConfig();

	ui.txtIPAddr->setText(cng.tcp_addr);
	ui.txtTCPPort->setText(QString::number(cng.tcp_port));

	ui.rbtnConnectByCom->setChecked(cng.com_ena);
	ui.rbtnConnectByEthernet->setChecked(!cng.com_ena);
	ui.txtComPort->setText(cng.com_name);
	ui.txtComSpeed->setText(QString::number(cng.com_speed));

	ui.rbtnCopyEna->setChecked(cng.outArchiveEna);
	ui.txtCopyPath->setText(cng.outArchivePath);
		
	bool isSel = ui.rbtnConnectByEthernet->isChecked();

	ui.txtIPAddr->setEnabled(isSel);
	ui.txtTCPPort->setEnabled(isSel);

	ui.txtComSpeed->setEnabled(!isSel);
	ui.txtComPort->setEnabled(!isSel);

	isSel = ui.rbtnCopyEna->isChecked();

	ui.txtCopyPath->setEnabled(isSel);
	
	selParamLoad_ = false;
	ui.lbChange->setText("");
}

void settingsPanel::selDirCopy(){
		
	QString fl = QFileDialog::getExistingDirectory(this,
		tr("Выбор пути сохранения файлов записи"), selDirCopy_);

	if (fl.isEmpty()) return;

	ui.txtCopyPath->setText(fl + "/");

	selDirCopy_ = fl;
}

void settingsPanel::saveChange(){

	MainWin::config cng = mainWin_->getConfig();

	cng.tcp_addr = ui.txtIPAddr->text();
	cng.tcp_port = ui.txtTCPPort->text().toInt();

	cng.outArchiveEna = ui.rbtnCopyEna->isChecked();
	cng.outArchivePath = ui.txtCopyPath->text();

	cng.com_ena = ui.rbtnConnectByCom->isChecked();
	cng.com_name = ui.txtComPort->text();
	cng.com_speed = ui.txtComSpeed->text().toInt();

	mainWin_->updateConfig(cng);

	if (selParamLoad_)
		ui.lbChange->setText(tr("Изменения вступят в силу после перезагрузки"));
	else
		ui.lbChange->setText("");
		
}

void settingsPanel::paramChange(){

	QString name = sender()->objectName();
	if ((name == "txtComSpeed") || (name == "txtIPAddr") || (name == "txtTCPPort") || (name == "txtComPort"))
		selParamLoad_ = true;
	ui.lbChange->setText("*");

}