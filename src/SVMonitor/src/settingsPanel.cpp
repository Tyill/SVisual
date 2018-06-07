
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