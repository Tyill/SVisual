
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
	connect(ui.cmbComPort, SIGNAL(currentIndexChanged(int)), this, SLOT(paramChange()));
	connect(ui.rbtnConnectByCom, &QRadioButton::clicked, this, [this] (){
		
		bool isSel = ui.rbtnConnectByCom->isChecked();

		ui.txtComSpeed->setEnabled(isSel);
		ui.cmbComPort->setEnabled(isSel);

		ui.txtAddr->setEnabled(!isSel);
		ui.txtPort->setEnabled(!isSel);

		selParamLoad_ = true;		
		paramChange();
	});
	connect(ui.rbtnConnectByEthernet, &QRadioButton::clicked, this, [this](){

		bool isSel = ui.rbtnConnectByEthernet->isChecked();

		ui.txtAddr->setEnabled(isSel);
		ui.txtPort->setEnabled(isSel);

		ui.txtComSpeed->setEnabled(!isSel);
		ui.cmbComPort->setEnabled(!isSel);

		selParamLoad_ = true;
		paramChange();
	});
	connect(ui.txtComSpeed, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	connect(ui.rbtnCopyEna, &QRadioButton::clicked, this, [this](){

		bool isSel = ui.rbtnCopyEna->isChecked();

		ui.txtCopyPath->setEnabled(isSel);
		paramChange();
	});

	connect(ui.txtAddr, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	connect(ui.txtPort, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	connect(ui.txtCopyPath, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
	
}

settingsPanel::~settingsPanel(){}

void settingsPanel::showEvent(QShowEvent * event){

	MainWin::config cng = mainWin_->getConfig();

	ui.txtAddr->setText(cng.tcp_addr);
	ui.txtPort->setText(QString::number(cng.tcp_port));

	ui.rbtnConnectByCom->setChecked(cng.com_ena);
	ui.rbtnConnectByEthernet->setChecked(!cng.com_ena);
	ui.cmbComPort->setCurrentText(cng.com_name);
	ui.txtComSpeed->setText(QString::number(cng.com_speed));

	ui.rbtnCopyEna->setChecked(cng.outArchiveEna);
	ui.txtCopyPath->setText(cng.outArchivePath);
		
	bool isSel = ui.rbtnConnectByEthernet->isChecked();

	ui.txtAddr->setEnabled(isSel);
	ui.txtPort->setEnabled(isSel);

	ui.txtComSpeed->setEnabled(!isSel);
	ui.cmbComPort->setEnabled(!isSel);

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

	cng.tcp_addr = ui.txtAddr->text();
	cng.tcp_port = ui.txtPort->text().toInt();

	cng.outArchiveEna = ui.rbtnCopyEna->isChecked();
	cng.outArchivePath = ui.txtCopyPath->text();

	cng.com_ena = ui.rbtnConnectByCom->isChecked();
	cng.com_name = ui.cmbComPort->currentText();
	cng.com_speed = ui.txtComSpeed->text().toInt();

	mainWin_->updateConfig(cng);

	if (selParamLoad_)
		ui.lbChange->setText(tr("Изменения вступят в силу после перезагрузки"));
	else
		ui.lbChange->setText("");
		
}

void settingsPanel::paramChange(){

	QString name = sender()->objectName();
	if ((name == "txtComSpeed") || (name == "txtAddr") || (name == "txtPort") || (name == "cmbComPort"))
		selParamLoad_ = true;
	ui.lbChange->setText("*");

}