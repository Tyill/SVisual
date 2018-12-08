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
#include <QPrinter>
#include <QPrintDialog>
#include "forms/mainWin.h"
#include "SVGraphPanel/SVGraphPanel.h"
#include "SVStatPanel/SVStatPanel.h"
#include "SVExportPanel/SVExportPanel.h" 
#include "SVConfig/SVConfigLimits.h"
#include "SVConfig/SVConfigData.h"

const QString VERSION = "1.0.6";

MainWin* mainWin = nullptr;

using namespace SV_Cng;


bool loadSignalData(const QString& sign);

QMap<QString, signalData*> getCopySignalRef(){
    
    auto sref = mainWin->signalRef_;

    return sref;
}

QMap<QString, moduleData*> getCopyModuleRef(){

    auto mref = mainWin->moduleRef_;

    return mref;
}


signalData* getSignalData(const QString& sign){

    signalData* sd = mainWin->signalRef_.contains(sign) ? mainWin->signalRef_[sign] : nullptr;

    return sd;
}

bool MainWin::writeSettings(QString pathIni){

    QFile file(pathIni);

    QTextStream txtStream(&file);	QStringList sList;

    file.open(QIODevice::WriteOnly);
    txtStream << "[Param]" << endl;
    txtStream << endl;
    txtStream << "selOpenDir = " << cng.selOpenDir << endl;
    txtStream << endl;
    txtStream << "cycleRecMs = " << cng.cycleRecMs << endl;
    txtStream << "packetSz = " << cng.packetSz << endl;
    txtStream << endl;
    txtStream << "sortByMod = " << (cng.sortByMod ? 1 : 0) << endl;
    txtStream << endl;
	file.close();

    return true;
}

bool MainWin::readSignals(QString path){

    QFile file(path);

    QTextStream txtStream(&file);

    if (file.open(QIODevice::ReadOnly)){

        while (!txtStream.atEnd()){

            QStringList lst = txtStream.readLine().split("\t");

            if (lst.size() >= 4){

				std::string module = qPrintable(lst[0]);
				std::string sname = qPrintable(lst[1]);
                std::string sign = sname + module;

                if (signalRef_.contains(sign.c_str())) continue;

                if (!moduleRef_.contains(module.c_str()))
                    moduleRef_[module.c_str()] = new moduleData(module);

                moduleRef_[module.c_str()]->signls.push_back(sign);
                moduleRef_[module.c_str()]->isActive = false;

				std::string group = (lst.size() == 5) ? qPrintable(lst[4]) : "";
				std::string comment = qPrintable(lst[3]);
				std::string stype = qPrintable(lst[2]);


                if (!groupRef_.contains(group.c_str()))
                    groupRef_[group.c_str()] = new groupData(group.c_str());

                groupRef_[group.c_str()]->signls.push_back(sign);
                groupRef_[group.c_str()]->isActive = false;

                auto sd = new signalData();
                sd->name = sname;
                sd->module = module;
                sd->group = group;
                sd->comment = comment;
                sd->type = getSVType(stype);

                signalRef_[sign.c_str()] = sd;
            }
        }

        file.close();

    }

    return true;
}

bool MainWin::writeSignals(QString path){

    QFile file(path);

    QTextStream txtStream(&file);

    file.open(QIODevice::WriteOnly);

    for (auto s : signalRef_){

        txtStream << s->module.c_str() << '\t'
                  << QString::fromLocal8Bit(s->name.c_str()) << '\t'
                  << getSVTypeStr(s->type).c_str() << '\t'
                  << QString::fromLocal8Bit(s->comment.c_str()) << '\t'
                  << QString::fromLocal8Bit(s->group.c_str()) << endl;
    }

    file.close();

    return true;
}

void MainWin::updateGroup(QString group, QString sign){


	if (!groupRef_.contains(group))
		groupRef_[group] = new groupData(group.toUtf8().data());

	std::string sname = sign.toUtf8().data();

	bool isSingExist = false;
	for (auto& s : groupRef_[group]->signls){
	    if (s == sname){
            isSingExist = true;
            break;
        }
	}

	if (!isSingExist)
		groupRef_[group]->signls.push_back(sname);

}

void MainWin::load(){

	ui.treeSignals->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.treeSignals->setIconSize(QSize(40, 20));
   
	auto gp = SV_Graph::createGraphPanel(this, SV_Graph::config(cng.cycleRecMs,cng.packetSz, SV_Graph::modeGr::viewer));
    SV_Graph::setGetCopySignalRef(gp, getCopySignalRef);
    SV_Graph::setGetSignalData(gp, getSignalData);
    SV_Graph::setLoadSignalData(gp, loadSignalData);
    graphPanels_[this] = gp;

    exportPanel_ = SV_Exp::createExpPanel(this, SV_Exp::config(cng.cycleRecMs, cng.packetSz));
    exportPanel_->setWindowFlags(Qt::Window);
    SV_Exp::setLoadSignalData(exportPanel_, loadSignalData);
    SV_Exp::setGetCopySignalRef(exportPanel_, getCopySignalRef);
    SV_Exp::setGetCopyModuleRef(exportPanel_, getCopyModuleRef);
    SV_Exp::setGetSignalData(exportPanel_, getSignalData);

	statPanel_ = SV_Stat::createStatPanel(this, SV_Stat::config(cng.cycleRecMs,cng.packetSz));
	statPanel_->setWindowFlags(Qt::Window);
	SV_Stat::setGetCopySignalRef(statPanel_, getCopySignalRef);
	SV_Stat::setGetSignalData(statPanel_, getSignalData);
	SV_Stat::setLoadSignalData(statPanel_, loadSignalData);
	SV_Stat::setSetTimeInterval(statPanel_, [](qint64 st, qint64 en){
        SV_Graph::setTimeInterval(mainWin->graphPanels_[mainWin], st, en);
	});
	SV_Stat::setGetTimeInterval(statPanel_, [](){
        return SV_Graph::getTimeInterval(mainWin->graphPanels_[mainWin]);
	});

	ui.splitter->addWidget(gp);
	ui.progressBar->setVisible(false);

	ui.btnSortByModule->setChecked(cng.sortByMod);
	ui.btnSortByGroup->setChecked(!cng.sortByMod);

	readSignals(QApplication::applicationDirPath() + "/svsignals.txt");
}

void MainWin::Connect(){

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(actionOpenData()));
	connect(ui.actionStat, SIGNAL(triggered()), this, SLOT(actionOpenStat()));

	connect(ui.treeSignals, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(selSignalClick(QTreeWidgetItem*, int)));
	connect(ui.treeSignals, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(selSignalDClick(QTreeWidgetItem*, int)));
	connect(ui.treeSignals, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(selSignalChange(QTreeWidgetItem*, int)));

	connect(ui.actionExit, &QAction::triggered, [this]() { 
		this->close();
	});

	connect(ui.actionExport, &QAction::triggered, [this]() {
        if (exportPanel_) exportPanel_->show();
	});

    connect(ui.actionNewWin, &QAction::triggered, [this]() {

        addNewWindow(QRect());
    });
		
	connect(ui.btnSortByGroup, &QPushButton::clicked, [this]() {
		this->ui.btnSortByGroup->setChecked(true);
		this->ui.btnSortByModule->setChecked(false);
		cng.sortByMod = false;
		sortSignalByGroupOrModule(false);
	});

	connect(ui.btnSortByModule, &QPushButton::clicked, [this]() {
		this->ui.btnSortByModule->setChecked(true);
		this->ui.btnSortByGroup->setChecked(false);
		cng.sortByMod = true;
		sortSignalByGroupOrModule(true);
	});

	connect(ui.actionPrint, &QAction::triggered, [this]() {

		QPrinter printer(QPrinter::HighResolution);
		printer.setPageMargins(12, 16, 12, 20, QPrinter::Millimeter);
		printer.setFullPage(false);

		QPrintDialog printDialog(&printer, this);
		if (printDialog.exec() == QDialog::Accepted) {

			QPainter painter(&printer);

			double xscale = printer.pageRect().width() / double(graphPanels_[this]->width());
            double yscale = printer.pageRect().height() / double(graphPanels_[this]->height());
			double scale = qMin(xscale, yscale);
			painter.translate(printer.paperRect().x(), printer.paperRect().y());
			painter.scale(scale, scale);

            graphPanels_[this]->render(&painter);
		}
	});

    connect(ui.actionSaveWinState, &QAction::triggered, [this]() {

        QString fname = QFileDialog::getSaveFileName(this,
            tr("Сохранение состояния окон"), cng.selOpenDir,
            "ini files (*.ini)");

        if (fname.isEmpty()) return;
        cng.selOpenDir = fname;

        QFile file(fname);

        QTextStream txtStream(&file);

        auto wins = graphPanels_.keys();

        int cnt = 0;
        for (auto w : wins){

            file.open(QIODevice::WriteOnly);
            txtStream << "[graphWin" << cnt << "]" << endl;

            if (w == this)
                txtStream << "locate = 0" << endl;
            else{
                auto geom = ((QDialog*)w)->geometry();
                txtStream << "locate = " << geom.x() << " " << geom.y() << " " << geom.width() << " " << geom.height() << endl;
            }
                        
            QVector<QVector<QString>> signs = SV_Graph::getLocateSignals(graphPanels_[w]);
            for (int i = 0; i < signs.size(); ++i){

                txtStream << "section" << i << " = ";
                for (int j = signs[i].size() - 1; j >= 0; --j)
                    txtStream << signs[i][j] << " ";

                txtStream << endl;
            }

            txtStream << endl;
            ++cnt;
        }

        file.close();

    });

    connect(ui.actionLoadWinState, &QAction::triggered, [this]() {

        QString fname = QFileDialog::getOpenFileName(this,
            tr("Загрузка состояния окон"), cng.selOpenDir,
            "ini files (*.ini)");

        if (fname.isEmpty()) return;
        cng.selOpenDir = fname;

        QSettings settings(fname, QSettings::IniFormat);

        auto grps = settings.childGroups();
        for (auto& g : grps){
            settings.beginGroup(g);

            QString locate = settings.value("locate").toString();
            QObject* win = this;
            if (locate != "0"){

                auto lt = locate.split(' ');

                win = addNewWindow(QRect(lt[0].toInt(), lt[1].toInt(), lt[2].toInt(), lt[3].toInt()));
            }

            int sect = 0;
            while (true){

                QString str = settings.value("section" + QString::number(sect), "").toString();
                if (str.isEmpty()) break;

                QStringList signs = str.split(' ');
                for (auto& s : signs)
                    SV_Graph::addSignal(graphPanels_[win], s, sect);

                ++sect;
            }
          
            settings.endGroup();
        }

    });



	connect(ui.actionProgram, &QAction::triggered, [this]() {
	QMessageBox::about(this, tr("About SVisual"),
			tr("<h2>SVViewer </h2>"
			"<p>Программное обеспечение предназначенное"
			"<p>для анализа сигналов с устройст."
            "<p>2017"));
	});
}

bool MainWin::init(QString initPath){


	QSettings settings(initPath, QSettings::IniFormat);
	settings.beginGroup("Param");

	cng.cycleRecMs =  settings.value("cycleRecMs", 100).toInt();
	cng.cycleRecMs = qMax(cng.cycleRecMs, 10);
	cng.packetSz = settings.value("packetSz", 10).toInt();
	cng.packetSz = qMax(cng.packetSz, 1);

	cng.selOpenDir = settings.value("selOpenDir", "").toString();
	cng.sortByMod = settings.value("sortByMod", 1).toInt() == 1;
		
	if (!QFile(initPath).exists())
		writeSettings(initPath);

	return true;

}

MainWin::MainWin(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	mainWin = this;

	this->setWindowTitle(QString("SVViewer ") + VERSION);

	QStringList args = QApplication::arguments();
	cng.initPath = QApplication::applicationDirPath(); if (args.size() == 2) cng.initPath = args[1];
	init(cng.initPath + "/sviewer.ini");

	Connect();

	load();
}

MainWin::~MainWin()
{	
	writeSettings(cng.initPath + "/sviewer.ini");
	writeSignals(cng.initPath + "/svsignals.txt");
}

bool MainWin::eventFilter(QObject *target, QEvent *event){

    if ((event->type() == QEvent::Close) && (target->objectName() == "graphWin")){

        graphPanels_.remove(target);
        target->deleteLater();
    }

    return QMainWindow::eventFilter(target, event);
}

void MainWin::sortSignalByGroupOrModule(bool byModule){

	ui.treeSignals->clear();
	auto sref = getCopySignalRef();

	QIcon iconImpuls(":/SVViewer/images/iconImpuls.png");
	QIcon iconSin(":/SVViewer/images/iconSin.png");

	int scnt = 0;
	if (byModule){

		ui.treeSignals->headerItem()->setText(2, tr("Группа"));
		
		for (auto itMod : moduleRef_){

			if (!itMod->isActive) continue;

			QTreeWidgetItem* root = new QTreeWidgetItem(ui.treeSignals);
			
			root->setText(0, itMod->module.c_str());
			for (auto& s : itMod->signls){

				QString sname = s.c_str();

				if (!sref[sname]->isActive) continue;
				++scnt;
				
				QTreeWidgetItem* item = new QTreeWidgetItem(root);
				item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
				item->setText(0, sref[sname]->name.c_str());
				item->setText(1, SV_Cng::getSVTypeStr(sref[sname]->type).c_str());
				item->setText(2, sref[sname]->group.c_str());
				item->setText(3, sref[sname]->comment.c_str());
				item->setText(4, sname);

				if (sref[sname]->type == valueType::tBool)
					item->setIcon(0, iconImpuls);
				else
					item->setIcon(0, iconSin);
			}
		}
	}
	else {

		ui.treeSignals->headerItem()->setText(2, tr("Модуль"));

		for (auto itGrp : groupRef_){

			if (!itGrp->isActive) continue;

			if (itGrp->signls.empty()) continue;

			QTreeWidgetItem* root = new QTreeWidgetItem(ui.treeSignals);

			root->setText(0, itGrp->group.c_str());
					
			for (auto& s : itGrp->signls){

				QString sname = s.c_str();

				if (!sref[sname]->isActive) continue;
				++scnt;

				QTreeWidgetItem* item = new QTreeWidgetItem(root);
				item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
				item->setText(0, sref[sname]->name.c_str());
				item->setText(1, SV_Cng::getSVTypeStr(sref[sname]->type).c_str());
				item->setText(2, sref[sname]->module.c_str());
				item->setText(3, sref[sname]->comment.c_str());
				item->setText(4, sname);

				if (sref[sname]->type == valueType::tBool)
					item->setIcon(0, iconImpuls);
				else
					item->setIcon(0, iconSin);
			}
		}
	}

	ui.treeSignals->sortByColumn(1);

	ui.lbAllSignCnt->setText(QString::number(scnt));
	
}

void MainWin::loadDataFinished(bool ok){

	if (ok){
		ui.lbStatusMess->setText(cng.selOpenDir);
				
		sortSignalByGroupOrModule(ui.btnSortByModule->isChecked());
	}
	else
		ui.lbStatusMess->setText(tr("Файл не удалось прочитать"));
			
	ui.progressBar->setVisible(false);

	thrLoadData_->deleteLater();
	
}

void MainWin::actionOpenData(){
	
	QStringList files = QFileDialog::getOpenFileNames(this,
		tr("Добавление файлов данных"), cng.selOpenDir,
		"dat files (*.dat)");

	if (files.isEmpty()) return;

	ui.progressBar->setVisible(true);

	thrLoadData_ = new thrLoadData(files);
	connect(thrLoadData_, SIGNAL(finished(bool)), this, SLOT(loadDataFinished(bool)));
}

void MainWin::actionOpenStat(){
		
	statPanel_->show();
}

void MainWin::selSignalClick(QTreeWidgetItem* item, int column){

	if (moduleRef_.contains(item->text(0))){
				
		auto sref = getCopySignalRef();
		std::string module = item->text(0).toUtf8().data();
		int scnt = 0;
		for (auto s : sref)						
			if ((s->module == module) && s->isActive) ++scnt;
		
		ui.lbSignCnt->setText(QString::number(scnt));
	}
	
}

void MainWin::selSignalDClick(QTreeWidgetItem * item, int column){
	
	if (moduleRef_.contains(item->text(0))) return;

	if ((column > 1) && (cng.sortByMod || (column != 2)))
		ui.treeSignals->editItem(item, column);
	else
        SV_Graph::addSignal(graphPanels_[this], item->text(4));
}

void MainWin::selSignalChange(QTreeWidgetItem * item, int column){

	QString sign = item->text(4);
	signalData* sd = getSignalData(sign); if (!sd) return;

	switch (column){
		case 2:
			if (cng.sortByMod){
			   sd->group = item->text(2).toUtf8().data();
			   updateGroup(item->text(2), sign);
		    }
		    break;
		case 3: sd->comment = item->text(3).toUtf8().data(); break;
	}
		
}


QDialog* MainWin::addNewWindow(const QRect& pos){

    QDialog* graphWin = new QDialog(this, Qt::Window);
    graphWin->setObjectName("graphWin");
    graphWin->installEventFilter(this);

    QVBoxLayout* vertLayout = new QVBoxLayout(graphWin);
    vertLayout->setSpacing(0);
    vertLayout->setContentsMargins(5, 5, 5, 5);

    SV_Graph::config config(cng.cycleRecMs, cng.packetSz, SV_Graph::modeGr::viewer);
    config.isShowTable = false;

    auto gp = SV_Graph::createGraphPanel(graphWin, config);
    SV_Graph::setGetCopySignalRef(gp, getCopySignalRef);
    SV_Graph::setGetSignalData(gp, getSignalData);
    SV_Graph::setLoadSignalData(gp, loadSignalData);

    graphPanels_[graphWin] = gp;
    vertLayout->addWidget(gp);

    graphWin->show();

    if (!pos.isNull()){
        graphWin->setGeometry(pos);
        graphWin->resize(QSize(pos.width(), pos.height()));
    }

    return graphWin;
}
