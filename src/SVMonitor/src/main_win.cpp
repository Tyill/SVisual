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

#include "SVMonitor/forms/main_win.h"
#include "SVMonitor/forms/event_table_dialog.h"
#include "SVMonitor/forms/settings_dialog.h"
#include "SVMonitor/forms/subscript_dialog.h"
#include "SVMonitor/forms/graph_setting_dialog.h"
#include "SVMonitor/src/db_provider.h"
#include "SVMisc/tcp_server.h"
#include "SVExportDialog/export_dialog.h"
#include "SVScriptDialog/script_dialog.h"
#include "SVServer/sv_server.h"
#include "SVWebServer/web_server.h"
#include "SVZabbix/zabbix.h"
#include "SVBase/sv_limits.h"
#include "server_api.h"
#include "com_reader.h"

#include <QSettings>
#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTreeWidget>
#include <QColorDialog>
#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QSystemTrayIcon>

const QString VERSION = QStringLiteral("1.2.2");

using namespace SV_Base;

void MainWin::statusMess(const QString& mess){

  QMetaObject::invokeMethod(ui.txtStatusMess, "append", Qt::AutoConnection,
    Q_ARG(QString, QString::fromStdString(SV_Misc::currDateTime()) + " " + mess));

  lg_.writeLine(qPrintable(mess));
}

MainWin::MainWin(QWidget *parent)
  : QMainWindow(parent){

  ui.setupUi(this);

  this->setWindowTitle(QString("SVMonitor ") + VERSION);

  QStringList args = QApplication::arguments();

  cng.dirPath = QApplication::applicationDirPath();
  cng.initPath = cng.dirPath + "/svmonitor.ini"; 
  if (args.size() == 2) cng.initPath = args[1];

  readSettings(cng.initPath);
  statusMess(tr("Инициализация параметров успешно"));

  auto gp = SV_Graph::createGraphPanel(this, SV_Graph::Config(cng.cycleRecMs, cng.packetSz));
  graphPanels_[this] = gp;
  ui.splitter->addWidget(gp);

  connects();

  load();

  sortSignalByModule();

  // запуск получения данных
  SV_Srv::Config srvCng; {
    srvCng.cycleRecMs = cng.cycleRecMs;
    srvCng.packetSz = cng.packetSz;
    srvCng.offsetMs = cng.offsetMs;
    srvCng.outArchiveEna = cng.outArchiveEna;
    srvCng.outArchiveHourCnt = cng.outArchiveHourCnt;
    srvCng.outArchiveName = cng.outArchiveName.toStdString();
    srvCng.outArchivePath = cng.outArchivePath.toStdString();
    srvCng.outDataBaseEna = cng.outDataBaseEna;
    srvCng.outDataBaseAddr = cng.outDataBaseAddr.toStdString();
    srvCng.outDataBaseName = cng.outDataBaseName.toStdString();
  }

  if (cng.com_ena){
    for (auto& port : cng.com_ports){

      if (port.first.isEmpty() || port.second.isEmpty()) continue;

      SerialPortReader::Config ccng(port.first, port.second.toInt(), cng.cycleRecMs, cng.packetSz);
      auto comReader = new SerialPortReader(ccng, this);
      if (comReader->start()) {
        statusMess(QString(tr("Прослушивание %1 порта запущено")).arg(port.first)); 
        SV_Srv::startServer(srvCng);
        comReader->setDataCBack(SV_Srv::receiveData);
      } else {
        statusMess(QString(tr("%1 порт недоступен")).arg(port.first));
      }
      comReaders_.push_back(comReader);
    }
  }else{
    if (SV_Misc::TCPServer::start(cng.tcp_addr.toStdString(), cng.tcp_port)){
      statusMess(QString(tr("TCP cервер запущен: адрес %1 порт %2").arg(cng.tcp_addr).arg(cng.tcp_port)));
      SV_Srv::startServer(srvCng);
      SV_Misc::TCPServer::setDataCBack(SV_Srv::receiveData);
    }
    else
      statusMess(QString(tr("Не удалось запустить TCP сервер: адрес %1 порт %2").arg(cng.tcp_addr).arg(cng.tcp_port)));
  }

  // web
  if (cng.web_ena){
    if (SV_Web::startServer(cng.web_addr, cng.web_port, SV_Web::Config(SV_CYCLEREC_MS, SV_PACKETSZ))){
      statusMess(QString(tr("WEB cервер запущен: адрес %1 порт %2").arg(cng.web_addr).arg(cng.web_port)));    }
    else
      statusMess(QString(tr("Не удалось запустить WEB сервер: адрес %1 порт %2").arg(cng.web_addr).arg(cng.web_port)));
  }

  // zabbix
  if (cng.zabbix_ena){
    if (SV_Zbx::startAgent(cng.zabbix_addr, cng.zabbix_port, SV_Zbx::Config(SV_CYCLEREC_MS, SV_PACKETSZ))){
      statusMess(QString(tr("Zabbix агент запущен: адрес %1 порт %2").arg(cng.zabbix_addr).arg(cng.zabbix_port)));
    }
    else
      statusMess(QString(tr("Не удалось запустить Zabbix агент: адрес %1 порт %2").arg(cng.zabbix_addr).arg(cng.zabbix_port)));
  }

  SV_Trigger::startUpdateThread(triggerDialog_);
}

MainWin::~MainWin(){

  if (cng.com_ena){
    for (auto comReader : qAsConst(comReaders_)){
      if (comReader) comReader->stop();
    }
  }else{    
    SV_Misc::TCPServer::stop();
  }

  if (cng.web_ena)
    SV_Web::stopServer();

  if (cng.zabbix_ena)
    SV_Zbx::stopAgent();

  SV_Srv::stopServer();

  if (db_){
    if (!db_->saveSignals(SV_Srv::getCopySignalRef()))
      statusMess(tr("Ошибка сохранения сигналов в БД"));
    if (!db_->saveAttrSignals(signAttr_))
      statusMess(tr("Ошибка сохранения атрибутов в БД"));
    if (!db_->saveTriggers(SV_Trigger::getCopyTriggerRef(triggerDialog_)))
      statusMess(tr("Ошибка сохранения триггеров в БД"));
  }

  writeSettings(cng.initPath);
}

bool MainWin::readSettings(QString initPath){

    QString logPath = cng.dirPath + "/log/svmonitor.log";
    lg_.setPathFile(qPrintable(logPath));

    QSettings settings(initPath, QSettings::IniFormat);
    settings.beginGroup("Param");

    cng.cycleRecMs = settings.value("cycleRecMs", 100).toInt();
    cng.cycleRecMs = qMax(cng.cycleRecMs, 1);
    cng.packetSz = settings.value("packetSz", 10).toInt();
    cng.packetSz = qMax(cng.packetSz, 1);
    cng.offsetMs = settings.value("offsetMs", 0).toInt();
    cng.offsetMs = qMax(cng.offsetMs, 0);
    cng.selOpenDir = settings.value("selOpenDir", "").toString();

    QFont ft = QApplication::font();
    int fsz = settings.value("fontSz", ft.pointSize()).toInt();
    ft.setPointSize(fsz);
    QApplication::setFont(ft);

    // связь по usb
    cng.com_ena = settings.value("com_ena", 0).toInt() == 1;
    QString com_name = settings.value("com0_name", "COM4").toString();
    QString com_speed = settings.value("com0_speed", "9600").toString();

    cng.com_ports.push_back(qMakePair(com_name, com_speed));

    // доп порты
    int comCnt = 1;
    while (true){

        QString com_name = settings.value("com" + QString::number(comCnt) + "_name", "").toString();
        QString com_speed = settings.value("com" + QString::number(comCnt) + "_speed", "").toString();

        if (com_name.isEmpty() || com_speed.isEmpty())
            break;

        cng.com_ports.push_back(qMakePair(com_name, com_speed));
        ++comCnt;
    }

    cng.dbPath = settings.value("dbPath", "").toString();
    if (cng.dbPath.isEmpty()) {
        cng.dbPath = cng.dirPath + "/svm.db";
    }
    // связь по TCP
    cng.tcp_addr = settings.value("tcp_addr", "127.0.0.1").toString();
    cng.tcp_port = settings.value("tcp_port", "2144").toInt();

    // web
    cng.web_ena = settings.value("web_ena", "0").toInt() == 1;
    cng.web_addr = settings.value("web_addr", "127.0.0.1").toString();
    cng.web_port = settings.value("web_port", "2145").toInt();

    // zabbix
    cng.zabbix_ena = settings.value("zabbix_ena", "0").toInt() == 1;
    cng.zabbix_addr = settings.value("zabbix_addr", "127.0.0.1").toString();
    cng.zabbix_port = settings.value("zabbix_port", "2146").toInt();

    // копир на диск
    cng.outArchiveEna = settings.value("outArchiveEna", "1").toInt() == 1;
    cng.outArchivePath = settings.value("outArchivePath", "").toString();
    if (cng.outArchivePath.isEmpty()) cng.outArchivePath = cng.dirPath + "/";
    cng.outArchivePath.replace("\\", "/");
    cng.outArchiveName = settings.value("outFileName", "svrec").toString();;
    cng.outArchiveHourCnt = settings.value("outFileHourCnt", 2).toInt();
    cng.outArchiveHourCnt = qBound(1, cng.outArchiveHourCnt, 12);

    // запись в БД
    cng.outDataBaseEna = settings.value("outDataBaseEna", "1").toInt() == 1;
    cng.outDataBaseName = settings.value("outDataBaseName", "svdb").toString();
    cng.outDataBaseAddr = settings.value("outDataBaseAddr", "localhost:9000").toString();

    cng.graphSett.lineWidth = settings.value("lineWidth", "2").toInt();
    cng.graphSett.transparent = settings.value("transparent", "100").toInt();
    cng.graphSett.darkTheme = settings.value("darkTheme", "0").toInt() == 1;
    cng.graphSett.signBoolOnTop = settings.value("signBoolOnTop", "0").toInt() == 1;
    cng.graphSett.gapTolerance = settings.value("gapTolerance", "1").toInt();

    cng.toutLoadWinStateSec = settings.value("toutLoadWinStateSec", "10").toInt();

    settings.endGroup();

    tmWinSetts_ = new QTimer(this);
    connect(tmWinSetts_, &QTimer::timeout, this, [this, initPath]() {

        QSettings settings(initPath, QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
        auto grps = settings.childGroups();
        for (auto& g : grps){

            if (!g.startsWith("graphWin"))
                continue;

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
                for (auto& s : signs){
                    SV_Graph::addSignal(graphPanels_[win], s, sect);
                }
                ++sect;
            }

            QVector< SV_Graph::AxisAttributes> axisAttrs;
            int axisInx = 0;
            while (true){

                QString str = settings.value("axisAttr" + QString::number(axisInx), "").toString();
                if (str.isEmpty()) break;

                QStringList attr = str.split(' ');

                SV_Graph::AxisAttributes axAttr;
                axAttr.isAuto = attr[0] == "1";
                axAttr.min = attr[1].toDouble();
                axAttr.max = attr[2].toDouble();
                axAttr.step = attr[3].toDouble();

                axisAttrs.push_back(axAttr);

                ++axisInx;
            }

            if (!axisAttrs.empty())
                SV_Graph::setAxisAttr(graphPanels_[win], axisAttrs);

            QString tmDiap = settings.value("tmDiap", "10000").toString();

            auto ctmIntl = SV_Graph::getTimeInterval(graphPanels_[win]);
            ctmIntl.second = ctmIntl.first + tmDiap.toLongLong();

            SV_Graph::setTimeInterval(graphPanels_[win], ctmIntl.first, ctmIntl.second);

            settings.endGroup();
        }
        tmWinSetts_->stop();
    });
    tmWinSetts_->start(cng.toutLoadWinStateSec * 1000);

    if (!QFile(initPath).exists())
        writeSettings(initPath);

    return true;
}

bool MainWin::writeSettings(QString pathIni){

    QFile file(pathIni);

    QTextStream txtStream(&file);
    txtStream.setCodec(QTextCodec::codecForName("UTF-8"));

    // запись новых данных
    file.open(QIODevice::WriteOnly);
    txtStream << "[Param]" << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "tcp_addr = " << cng.tcp_addr << Qt::endl;
    txtStream << "tcp_port = " << cng.tcp_port << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "web_ena = " << (cng.web_ena ? "1" : "0") << Qt::endl;
    txtStream << "web_addr = " << cng.web_addr << Qt::endl;
    txtStream << "web_port = " << cng.web_port << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "zabbix_ena = " << (cng.zabbix_ena ? "1" : "0") << Qt::endl;
    txtStream << "zabbix_addr = " << cng.zabbix_addr << Qt::endl;
    txtStream << "zabbix_port = " << cng.zabbix_port << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "com_ena = " << (cng.com_ena ? "1" : "0") << Qt::endl;

    for (int i = 0; i < cng.com_ports.size(); ++i){
        txtStream << "com" << i << "_name = " << cng.com_ports[i].first << Qt::endl;
        txtStream << "com" << i << "_speed = " << cng.com_ports[i].second << Qt::endl;
    }

    txtStream << Qt::endl;
    txtStream << "dbPath = " << cng.dbPath << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "cycleRecMs = " << cng.cycleRecMs << Qt::endl;
    txtStream << "packetSz = " << cng.packetSz << Qt::endl;
    txtStream << "offsetMs = " << cng.offsetMs << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "; save on disk" << Qt::endl;
    txtStream << "outArchiveEna = " << (cng.outArchiveEna ? "1" : "0") << Qt::endl;
    txtStream << "outArchivePath = " << cng.outArchivePath << Qt::endl;
    txtStream << "outArchiveName = " << cng.outArchiveName << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "; save into DB" << Qt::endl;
    txtStream << "outDataBaseEna = " << (cng.outDataBaseEna ? "1" : "0") << Qt::endl;
    txtStream << "outDataBaseName = " << cng.outDataBaseName << Qt::endl;
    txtStream << "outDataBaseAddr = " << cng.outDataBaseAddr << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "selOpenDir = " << cng.selOpenDir << Qt::endl;
    txtStream << "fontSz = " << this->font().pointSize() << Qt::endl;
    txtStream << "transparent = " << cng.graphSett.transparent << Qt::endl;
    txtStream << "lineWidth = " << cng.graphSett.lineWidth << Qt::endl;
    txtStream << "darkTheme = " << (cng.graphSett.darkTheme ? "1" : "0") << Qt::endl;
    txtStream << "signBoolOnTop = " << (cng.graphSett.signBoolOnTop ? "1" : "0") << Qt::endl;
    txtStream << "gapTolerance = " << cng.graphSett.gapTolerance << Qt::endl;
    txtStream << Qt::endl;
    txtStream << "toutLoadWinStateSec = " << cng.toutLoadWinStateSec << Qt::endl;
    txtStream << Qt::endl;

    // состояние окон
    auto wins = graphPanels_.keys();
    int cnt = 0;
    for (auto w : wins){

        txtStream << "[graphWin" << cnt << "]" << Qt::endl;

        if (w == this)
            txtStream << "locate = 0" << Qt::endl;
        else{
            auto geom = ((QDialog*)w)->geometry();
            txtStream << "locate = " << geom.x() << " " << geom.y() << " " << geom.width() << " " << geom.height() << Qt::endl;
        }

        auto tmIntl = SV_Graph::getTimeInterval(graphPanels_[w]);
        txtStream << "tmDiap = " << (tmIntl.second - tmIntl.first) << Qt::endl;

        QVector<QVector<QString>> signs = SV_Graph::getLocateSignals(graphPanels_[w]);
        for (int i = 0; i < signs.size(); ++i){

            txtStream << "section" << i << " = ";
            for (int j = signs[i].size() - 1; j >= 0; --j)
                txtStream << signs[i][j] << " ";

            txtStream << Qt::endl;
        }

        QVector<SV_Graph::AxisAttributes> axisAttr = SV_Graph::getAxisAttr(graphPanels_[w]);
        for (int i = 0; i < axisAttr.size(); ++i){

            txtStream << "axisAttr" << i << " = ";
            txtStream << (axisAttr[i].isAuto ? "1" : "0") << " ";
            txtStream << axisAttr[i].min << " ";
            txtStream << axisAttr[i].max << " ";
            txtStream << axisAttr[i].step << " ";

            txtStream << Qt::endl;
        }

        txtStream << Qt::endl;
        ++cnt;
    }

    file.close();

    return true;
}

void MainWin::connects(){

    connect(ui.treeSignals, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int){
        auto mref = SV_Srv::getCopyModuleRef();

        if (mref.find(item->text(0).toStdString()) != mref.end()){

            ui.lbSignCnt->setText(QString::number(mref[item->text(0).toStdString()]->signls.size()));
        }
    });
    connect(ui.treeSignals, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem* item, int column){
        auto mref = SV_Srv::getCopyModuleRef();

        if (mref.find(item->text(0).toStdString()) != mref.end())
            return;

        auto sign = item->text(5);
        if (column == 0){
            if (tmWinSetts_->isActive()) tmWinSetts_->stop();
            SV_Graph::addSignal(graphPanels_[this], sign);
        }
        else {
            if (column == 2){

                auto clr = QColorDialog::getColor();

                item->setBackgroundColor(2, clr);

                auto sd = getSignalDataSrv(sign);

                if (!sd) return;

                signAttr_[sign] = SignalAttr{ QString::fromStdString(sd->name),
                    QString::fromStdString(sd->module),
                    clr };
                for (auto gp : qAsConst(graphPanels_))
                    SV_Graph::setSignalAttr(gp, sign, SV_Graph::SignalAttributes{ clr });
            }
            else
                ui.treeSignals->editItem(item, column);
        }
    });
    connect(ui.treeSignals, &QTreeWidget::itemChanged, [this](QTreeWidgetItem* item, int column){
        std::string sign = item->text(5).toStdString();
        SV_Base::SignalData* sd = SV_Srv::getSignalData(sign);

        if (!sd) return;

        switch (column){
        case 3: sd->comment = item->text(3).toStdString(); break;
        case 4: sd->group = item->text(4).toStdString(); break;
        }
    });

    connect(ui.actionExit, &QAction::triggered, this, [this]() {
        this->close();
    });
    connect(ui.actionPrint, &QAction::triggered, this, [this]() {
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
    connect(ui.actionTrgPanel, &QAction::triggered, this, [this]() {
        if (triggerDialog_) triggerDialog_->showNormal();
    });
    connect(ui.actionEventOrder, &QAction::triggered, this, [this]() {
        if (eventTableDialog_) eventTableDialog_->showNormal();
    });
    connect(ui.actionExport, &QAction::triggered, this, [this]() {
        if (exportDialog_) exportDialog_->showNormal();
    });
    connect(ui.actionNewWin, &QAction::triggered, this, [this]() {
        addNewWindow(QRect());
    });
    connect(ui.actionScript, &QAction::triggered, this, [this]() {
        SV_Script::startUpdateThread(scriptDialog_);
        scriptDialog_->showNormal();
    });
    connect(ui.actionSettings, &QAction::triggered, this, [this]() {
        if (settingsDialog_) settingsDialog_->showNormal();
    });
    connect(ui.actionGraphSett, &QAction::triggered, this, [this]() {
        if (graphSettDialog_) graphSettDialog_->showNormal();
    });
    connect(ui.actionUpFont, &QAction::triggered, this, [this]() {

        QFont ft = QApplication::font();

        ft.setPointSize(ft.pointSize() + 1);

        QApplication::setFont(ft);
    });
    connect(ui.actionDnFont, &QAction::triggered, this, [this]() {

        QFont ft = QApplication::font();

        ft.setPointSize(ft.pointSize() - 1);

        QApplication::setFont(ft);
    });
    connect(ui.actionCheckUpdate, &QAction::triggered, this, [this]() {

        if (!netManager_){
            netManager_ = new QNetworkAccessManager(this);
            connect(netManager_, &QNetworkAccessManager::finished, this, [](QNetworkReply* reply){

                QByteArray response_data = reply->readAll();
                QJsonDocument jsDoc = QJsonDocument::fromJson(response_data);

                if (jsDoc.isObject()){
                    QJsonObject  jsObj = jsDoc.object();

                    QMessageBox msgBox;
                    msgBox.setTextFormat(Qt::RichText);
                    msgBox.setStandardButtons(QMessageBox::Ok);

                    QString lastVer = jsObj["tag_name"].toString();

                    if (VERSION != lastVer)
                        msgBox.setText(tr("Доступна новая версия: ") + "<a href='" + jsObj["html_url"].toString() + "'>" + lastVer + "</a>");
                    else
                        msgBox.setText(tr("У вас самая новая версия"));

                    msgBox.exec();
                }
            });
        }

        QNetworkRequest request(QUrl("https://api.github.com/repos/Tyill/SVisual/releases/latest"));

        netManager_->get(request);
    });
    connect(ui.actionSaveWinState, &QAction::triggered, this, [this]() {

        QString fname = QFileDialog::getSaveFileName(this,
            tr("Сохранение состояния окон"), cng.selOpenDir,
            "ini files (*.ini)");

        if (fname.isEmpty()) return;
        cng.selOpenDir = fname;

        QFile file(fname);

        QTextStream txtStream(&file);
        txtStream.setCodec(QTextCodec::codecForName("UTF-8"));

        auto wins = graphPanels_.keys();

        int cnt = 0;
        for (auto w : wins){

            file.open(QIODevice::WriteOnly);
            txtStream << "[graphWin" << cnt << "]" << Qt::endl;

            if (w == this)
                txtStream << "locate = 0" << Qt::endl;
            else{
                auto geom = ((QDialog*)w)->geometry();
                txtStream << "locate = " << geom.x() << " " << geom.y() << " " << geom.width() << " " << geom.height() << Qt::endl;
            }

            auto tmIntl = SV_Graph::getTimeInterval(graphPanels_[w]);
            txtStream << "tmDiap = " << (tmIntl.second - tmIntl.first) << Qt::endl;

            QVector<QVector<QString>> signs = SV_Graph::getLocateSignals(graphPanels_[w]);
            for (int i = 0; i < signs.size(); ++i){

                txtStream << "section" << i << " = ";
                for (int j = signs[i].size() - 1; j >= 0; --j)
                    txtStream << signs[i][j] << " ";

                txtStream << Qt::endl;
            }

            QVector<SV_Graph::AxisAttributes> axisAttr = SV_Graph::getAxisAttr(graphPanels_[w]);
            for (int i = 0; i < axisAttr.size(); ++i){

                txtStream << "axisAttr" << i << " = ";
                txtStream << (axisAttr[i].isAuto ? "1" : "0") << " ";
                txtStream << axisAttr[i].min << " ";
                txtStream << axisAttr[i].max << " ";
                txtStream << axisAttr[i].step << " ";

                txtStream << Qt::endl;
            }

            txtStream << Qt::endl;
            ++cnt;
        }

        file.close();

        statusMess(tr("Состояние успешно сохранено"));
    });
    connect(ui.actionLoadWinState, &QAction::triggered, this, [this]() {

        QString fname = QFileDialog::getOpenFileName(this,
            tr("Загрузка состояния окон"), cng.selOpenDir,
            "ini files (*.ini)");

        if (fname.isEmpty()) return;
        cng.selOpenDir = fname;

        QSettings settings(fname, QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

        if (tmWinSetts_->isActive()) tmWinSetts_->stop();

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


            QVector< SV_Graph::AxisAttributes> axisAttrs;
            int axisInx = 0;
            while (true){

                QString str = settings.value("axisAttr" + QString::number(axisInx), "").toString();
                if (str.isEmpty()) break;

                QStringList attr = str.split(' ');

                SV_Graph::AxisAttributes axAttr;
                axAttr.isAuto = attr[0] == "1";
                axAttr.min = attr[1].toDouble();
                axAttr.max = attr[2].toDouble();
                axAttr.step = attr[3].toDouble();

                axisAttrs.push_back(axAttr);

                ++axisInx;
            }

            if (!axisAttrs.empty())
                SV_Graph::setAxisAttr(graphPanels_[win], axisAttrs);

            QString tmDiap = settings.value("tmDiap", "10000").toString();

            auto ctmIntl = SV_Graph::getTimeInterval(graphPanels_[win]);
            ctmIntl.second = ctmIntl.first + tmDiap.toLongLong();

            SV_Graph::setTimeInterval(graphPanels_[win], ctmIntl.first, ctmIntl.second);

            settings.endGroup();
        }

        statusMess(tr("Состояние успешно загружено"));
    });
    connect(ui.actionManual, &QAction::triggered, this, [this]() {

#ifdef SV_EN
        QDesktopServices::openUrl(QUrl::fromLocalFile(cng.dirPath + "/SVManualEN.pdf"));
#else
        QDesktopServices::openUrl(QUrl::fromLocalFile(cng.dirPath + "/SVManualRU.pdf"));
#endif
    });
    connect(ui.actionProgram, &QAction::triggered, this, [this]() {

        QString mess = "<h2>SVMonitor </h2>"
            "<p>Программное обеспечение предназначенное"
            "<p>для анализа сигналов с устройст."
            "<p>2017";

        QMessageBox::about(this, tr("About SVisual"), mess);
    });
    connect(ui.btnSlowPlay, &QPushButton::clicked, this, [this]() {
        slowMode();
    });
}

void MainWin::load(){

    ui.treeSignals->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.treeSignals->setIconSize(QSize(40, 20));

    settingsDialog_ = new SettingsDialog(this); settingsDialog_->setWindowFlags(Qt::Window);
    graphSettDialog_ = new GraphSettingDialog(cng.graphSett, this); graphSettDialog_->setWindowFlags(Qt::Window);

    SV_Graph::setLockReadSDataCBack(lockReadSDataSrv);
    SV_Graph::setUnlockReadSDataCBack(unlockReadSDataSrv);
    SV_Graph::setLoadSignalDataCBack(graphPanels_[this], loadSignalDataSrv);
    SV_Graph::setGetCopySignalRefCBack(graphPanels_[this], getCopySignalRefSrv);
    SV_Graph::setGetSignalDataCBack(graphPanels_[this], getSignalDataSrv);
    SV_Graph::setGetSignalAttrCBack(graphPanels_[this], [this](const QString& sname, SV_Graph::SignalAttributes& out){
        if (signAttr_.contains(sname)){
            out.color = signAttr_[sname].color;
            return true;
        }
        return false;
    });
    SV_Graph::setGraphSetting(graphPanels_[this], cng.graphSett);

    triggerDialog_ = SV_Trigger::getTriggerDialog(this, SV_Trigger::Config(cng.cycleRecMs, cng.packetSz));
    triggerDialog_->setWindowFlags(Qt::Window);
    SV_Trigger::setLockReadSDataCBack(lockReadSDataSrv);
    SV_Trigger::setUnlockReadSDataCBack(unlockReadSDataSrv);
    SV_Trigger::setGetCopySignalRefCBack(triggerDialog_, getCopySignalRefSrv);
    SV_Trigger::setGetSignalDataCBack(triggerDialog_, getSignalDataSrv);
    SV_Trigger::setGetCopyModuleRefCBack(triggerDialog_, getCopyModuleRefSrv);
    SV_Trigger::setGetModuleDataCBack(triggerDialog_, getModuleDataSrv);
    SV_Trigger::setOnTriggerCBack(triggerDialog_, [this](const QString& name){
        onTrigger(name);
    });

    scriptDialog_ = SV_Script::getScriptDialog(this, SV_Script::Config(cng.cycleRecMs, cng.packetSz), SV_Script::ModeGr::Player);
    scriptDialog_->setWindowFlags(Qt::Window);
    SV_Script::setLockReadSDataCBack(lockReadSDataSrv);
    SV_Script::setUnlockReadSDataCBack(unlockReadSDataSrv);
    SV_Script::setLoadSignalDataCBack(scriptDialog_, loadSignalDataSrv);
    SV_Script::setGetCopySignalRefCBack(scriptDialog_, getCopySignalRefSrv);
    SV_Script::setGetSignalDataCBack(scriptDialog_, getSignalDataSrv);
    SV_Script::setGetModuleDataCBack(scriptDialog_, getModuleDataSrv);
    SV_Script::setAddSignalCBack(scriptDialog_, addSignalSrv);
    SV_Script::setAddModuleCBack(scriptDialog_, addModuleSrv);
    SV_Script::setAddSignalsCBack(scriptDialog_, [this](){
        QMetaObject::invokeMethod(this, "updateTblSignal", Qt::AutoConnection);
    });
    SV_Script::setUpdateSignalsCBack(scriptDialog_, [this](){
        QMetaObject::invokeMethod(this, "updateSignals", Qt::AutoConnection);
    });
    SV_Script::setModuleConnectCBack(scriptDialog_, [this](const QString& module){
        QMetaObject::invokeMethod(this, "moduleConnect", Qt::AutoConnection, Q_ARG(QString, module));
    });
    SV_Script::setChangeSignColorCBack(scriptDialog_, [this](const QString& module, const QString& name, const QColor& clr){
        QMetaObject::invokeMethod(this, "changeSignColor", Qt::AutoConnection, Q_ARG(QString, module), Q_ARG(QString, name), Q_ARG(QColor, clr));
    });

    exportDialog_ = SV_Exp::createExportDialog(this, SV_Exp::Config(cng.cycleRecMs, cng.packetSz));
    exportDialog_->setWindowFlags(Qt::Window);
    SV_Exp::setLockReadSDataCBack(lockReadSDataSrv);
    SV_Exp::setUnlockReadSDataCBack(unlockReadSDataSrv);
    SV_Exp::setLoadSignalDataCBack(exportDialog_, loadSignalDataSrv);
    SV_Exp::setGetCopySignalRefCBack(exportDialog_, getCopySignalRefSrv);
    SV_Exp::setGetCopyModuleRefCBack(exportDialog_, getCopyModuleRefSrv);
    SV_Exp::setGetSignalDataCBack(exportDialog_, getSignalDataSrv);

    SV_Srv::setStatusCBack([this](const std::string& mess){
        statusMess(QString::fromLocal8Bit(mess.c_str()));
    });
    SV_Srv::setOnUpdateSignalsCBack([this](){
        QMetaObject::invokeMethod(this, "updateSignals", Qt::AutoConnection);
    });
    SV_Srv::setOnAddSignalsCBack([this](){
        QMetaObject::invokeMethod(this, "updateTblSignal", Qt::AutoConnection);
    });
    SV_Srv::setOnModuleConnectCBack([this](const std::string& module){
        QMetaObject::invokeMethod(this, "moduleConnect", Qt::AutoConnection, Q_ARG(QString, QString::fromStdString(module)));
    });
    SV_Srv::setOnModuleDisconnectCBack([this](const std::string& module){
        QMetaObject::invokeMethod(this, "moduleDisconnect", Qt::AutoConnection, Q_ARG(QString, QString::fromStdString(module)));
    });

    SV_Web::setLockReadSDataCBack(lockReadSDataSrv);
    SV_Web::setUnlockReadSDataCBack(unlockReadSDataSrv);
    SV_Web::setGetCopySignalRefCBack(getCopySignalRefSrv);
    SV_Web::setGetSignalDataCBack(getSignalDataSrv);
    SV_Web::setGetCopyModuleRefCBack(getCopyModuleRefSrv);

    SV_Zbx::setLockReadSData(lockReadSDataSrv);
    SV_Zbx::setUnlockReadSData(unlockReadSDataSrv);
    SV_Zbx::setGetSignalData(getSignalDataSrv);

    db_ = new DbProvider(qUtf8Printable(cng.dbPath), this);
    if (db_->isConnect()){
        statusMess(tr("Подключение БД успешно"));
    }
    else{
        statusMess(tr("Подключение БД ошибка: ") + cng.dbPath);
        delete db_;
        db_ = nullptr;
    }
    eventTableDialog_ = new EventTableDialog(db_, this);
    eventTableDialog_->setWindowFlags(Qt::Window);

    /////////////////////
    initTrayIcon();

}

void MainWin::updateGraphSetting(const SV_Graph::GraphSetting& gs){

  cng.graphSett = gs;

  for (auto o : qAsConst(graphPanels_))
    SV_Graph::setGraphSetting(o, gs);
}

bool MainWin::eventFilter(QObject *target, QEvent *event){

  if ((event->type() == QEvent::Close) && (target->objectName() == "graphWin")){

    graphPanels_.remove(target);
    target->deleteLater();
  }

  return QMainWindow::eventFilter(target, event);
}

void MainWin::sortSignalByModule(){

  int itsz = ui.treeSignals->topLevelItemCount();
  QMap<QString, bool> isExpanded;
  for (int i = 0; i < itsz; ++i)
    isExpanded[ui.treeSignals->topLevelItem(i)->text(0)] = ui.treeSignals->topLevelItem(i)->isExpanded();

  ui.treeSignals->clear();

  auto mref = SV_Srv::getCopyModuleRef();

  for (auto& it : mref){

    auto md = it.second;

    if (md->isDelete) continue;

    QTreeWidgetItem* root = new QTreeWidgetItem(ui.treeSignals);

    if (isExpanded.contains(md->module.c_str()))
      root->setExpanded(isExpanded[md->module.c_str()]);
    root->setFlags(root->flags() | Qt::ItemFlag::ItemIsEditable);
    root->setText(0, md->module.c_str());

    md->isEnable ? root->setIcon(0, QIcon(":/SVMonitor/images/trafficlight-green.png")) :
      root->setIcon(0, QIcon(":/SVMonitor/images/trafficlight-yel.png"));

    if (!md->isActive) root->setIcon(0, QIcon(":/SVMonitor/images/trafficlight-red.png"));

    auto msigns = getModuleSignalsSrv(md->module.c_str());
    for (auto& sign : msigns){
      SV_Base::SignalData* sd = SV_Srv::getSignalData(sign.toStdString());

      if (!sd || sd->isDelete) continue;

      QTreeWidgetItem* item = new QTreeWidgetItem(root);
      item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
      item->setText(0, sd->name.c_str());
      item->setText(1, SV_Base::getSVTypeStr(sd->type).c_str());

      if (signAttr_.contains(sign))
        item->setBackgroundColor(2, signAttr_[sign].color);
      else
        item->setBackgroundColor(2, QColor(255, 255, 255));

      item->setText(3, sd->comment.c_str());
      item->setText(4, sd->group.c_str());

      item->setText(5, sign);
    }
  }

  ui.treeSignals->sortByColumn(1);

  ui.lbAllSignCnt->setText(QString::number(SV_Srv::getCopySignalRef().size()));

}

void MainWin::contextMenuEvent(QContextMenuEvent * event){

  QString root = ui.treeSignals->currentItem() ? ui.treeSignals->currentItem()->text(0) : "";

  if (root.isEmpty() || !qobject_cast<TreeWidgetExt*>(focusWidget())) return;

  QMenu* menu = new QMenu(this);

  auto mref = getCopyModuleRefSrv();

  if (mref.contains(root)){

    if (mref[root]->isEnable && mref[root]->isActive){
      menu->addAction(tr("Показать все"));
      menu->addAction(tr("Отключить"));
    }
    else{
      menu->addAction(tr("Включить"));
      menu->addAction(tr("Удалить"));
    }
  }
  else{
    QString module = ui.treeSignals->currentItem()->parent()->text(0);
    if (module != "Virtual"){
      menu->addAction(tr("Скрипт"));
    }
    menu->addAction(tr("Сбросить цвет"));
    menu->addAction(tr("Удалить"));
  }

  connect(menu,
    SIGNAL(triggered(QAction*)),
    this,
    SLOT(contextMenuClick(QAction*))
    );

  menu->exec(event->globalPos());
}

void MainWin::contextMenuClick(QAction* act){

  QString root = ui.treeSignals->currentItem() ? ui.treeSignals->currentItem()->text(0) : "";

  if (root.isEmpty()) return;

  auto sref = getCopySignalRefSrv();
  auto mref = getCopyModuleRefSrv();

  // signal
  if (mref.find(root) == mref.end()){

    if (act->text() == tr("Скрипт")){

      SV_Script::startUpdateThread(scriptDialog_);

      SubScriptDialog* scr = new SubScriptDialog(scriptDialog_, this);
      scr->setWindowFlags(Qt::Window);

      QString module = ui.treeSignals->currentItem()->parent()->text(0);

      auto sd = getSignalDataSrv(root + module);

      scr->showSignScript(root, module, sd->type);
    }
    else if (act->text() == tr("Сбросить цвет")){

      QString module = ui.treeSignals->currentItem()->parent()->text(0);
      QString sign = root + module;

      if (signAttr_.contains(sign)){
        signAttr_.remove(sign);
        for (auto gp : qAsConst(graphPanels_)){
          SV_Graph::update(gp);
        }
        db_->delAttrSignal(root, module);
        sortSignalByModule();
      }
    }
    else if (act->text() == tr("Удалить")){

      QString module = ui.treeSignals->currentItem()->parent()->text(0);
      QString sign = root + module;
      if (sref.contains(sign)){
        sref[sign]->isDelete = true;

        statusMess(tr("Сигнал удален ") + module + ":" + root);

        sortSignalByModule();
      }
    }
  }
  // module
  else{
    if (act->text() == tr("Показать все")){
      if (tmWinSetts_->isActive()) tmWinSetts_->stop();
      for (auto& s : mref[root]->signls){
        if (sref.contains(s.c_str()) && sref[s.c_str()]->isActive){
          SV_Graph::addSignal(graphPanels_[this], QString::fromStdString(s));
        }
      }
    }
    else if (act->text() == tr("Включить")){

      mref[root]->isEnable = true;
      sortSignalByModule();
    }
    else if (act->text() == tr("Отключить")){

      mref[root]->isEnable = false;
      sortSignalByModule();
    }
    else if (act->text() == tr("Удалить")){

      QMessageBox mb;
      mb.setText(tr("Удалить модуль со всеми сигналами?"));
      mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      mb.setDefaultButton(QMessageBox::No);

      switch (mb.exec()) {
      case QMessageBox::Yes:
      {
        statusMess(tr("Модуль удален ") + root);

        mref[root]->isDelete = true;

        for (auto s : qAsConst(sref)){
          if (QString::fromStdString(s->module) == root){
            s->isDelete = true;
          }
        }

        sortSignalByModule();
      }
      break;
      case QMessageBox::No:
        break;
      }
    }
  }

}

void MainWin::updateTblSignal(){

  auto sref = SV_Srv::getCopySignalRef();

  if (sref.size() > SV_VALUE_MAX_CNT)
    statusMess(tr("Превышен лимит количества сигналов: %1. Стабильная работа не гарантирована.").
    arg(SV_VALUE_MAX_CNT));

  // посмотрим в БД что есть
  if (db_){

    for (auto& s : sref){

      // только тех, которые еще не видел
      if (!signExist_.contains(s.first.c_str())){

        SignalData sd = db_->getSignal(s.second->name.c_str(), s.second->module.c_str());
        if (!sd.name.empty()){
          s.second->group = sd.group;
          s.second->comment = sd.comment;
        }

        SignalAttr as = db_->getAttrSignal(s.second->name.c_str(), s.second->module.c_str());
        if (as.color.isValid()){
          signAttr_[QString::fromStdString(s.first)] = as;
        }

        auto trg = db_->getTrigger(s.second->name.c_str(), s.second->module.c_str());
        int sz = trg.size();
        for (int i = 0; i < sz; ++i)
          SV_Trigger::addTrigger(triggerDialog_, trg[i]->name, trg[i]);

        signExist_.insert(s.first.c_str());
      }
    }
  }

  if (!isSlowMode_)
    sortSignalByModule();
}

void MainWin::updateSignals(){

  for (auto gp : qAsConst(graphPanels_))
    SV_Graph::update(gp);
}

void MainWin::moduleConnect(QString module){

  statusMess(tr("Подключен модуль: ") + module);

  auto mref = SV_Srv::getCopyModuleRef();

  // только тех, которые еще не видел
  if (!signExist_.contains(module)){

    auto trgOn = db_ ? db_->getTrigger(module + "On") : nullptr;
    if (!trgOn)  {
      trgOn = new SV_Trigger::TriggerData();
      trgOn->name = module + "On";
      trgOn->signal = "";
      trgOn->module = module;
      trgOn->condType = SV_Trigger::EventType::CONNECT_MODULE;
      trgOn->isActive = false;
      trgOn->condValue = 0;
      trgOn->condTOut = 0;
    }
    SV_Trigger::addTrigger(triggerDialog_, module + "On", trgOn);

    auto trgOff = db_ ? db_->getTrigger(module + "Off") : nullptr;
    if (!trgOff){
      trgOff = new  SV_Trigger::TriggerData();
      trgOff->name = module + "Off";
      trgOff->signal = "";
      trgOff->module = module;
      trgOff->condType = SV_Trigger::EventType::DISCONNECT_MODULE;
      trgOff->isActive = false;
      trgOff->condValue = 0;
      trgOff->condTOut = 0;
    }
    SV_Trigger::addTrigger(triggerDialog_, module + "Off", trgOff);

    signExist_.insert(module);
  }

  sortSignalByModule();

  auto tr = SV_Trigger::getTriggerData(triggerDialog_, module + "On");
  if (tr && tr->isActive){
    tr->condValue = 1;
  }
  tr = SV_Trigger::getTriggerData(triggerDialog_, module + "Off");
  if (tr && tr->isActive){
    tr->condValue = 0;
  }
}

void MainWin::moduleDisconnect(QString module){

  statusMess(tr("Отключен модуль: ") + module);

  sortSignalByModule();

  auto tr = SV_Trigger::getTriggerData(triggerDialog_, module + "On");
  if (tr && tr->isActive)
    tr->condValue = 0;

  tr = SV_Trigger::getTriggerData(triggerDialog_, module + "Off");
  if (tr && tr->isActive)
    tr->condValue = 1;
}

void MainWin::onTrigger(QString trigger){

  auto* td = SV_Trigger::getTriggerData(triggerDialog_, trigger);
  if (!td){
      return;
  }

  const auto name = QString("%1:%2:%3").arg(td->module, td->signal, td->name);

  statusMess(QObject::tr("Событие: ") + name);

  if (db_){
    db_->saveEvent(trigger, QDateTime::currentDateTime());
  }

  if (!td->userProcPath.isEmpty()){
    QFile f(td->userProcPath);
    if (f.exists()){

      QStringList args = td->userProcArgs.split('\t');
      for (auto& ar : args) ar = ar.trimmed();

      QProcess::startDetached(td->userProcPath, args);

      statusMess(name + QObject::tr(" Процесс запущен: ") + td->userProcPath + " args: " + td->userProcArgs);
    }
    else
      statusMess(name + QObject::tr(" Путь не найден: ") + td->userProcPath);
  }
}

void MainWin::initTrayIcon(){

  trayIcon_ = new QSystemTrayIcon(this);
  QIcon trayImage(":/SVMonitor/images/logo.png");
  trayIcon_->setIcon(trayImage);

  // Setting system tray's icon menu...
  QMenu* trayIconMenu = new QMenu(this);

  QAction* restoreAction = new QAction("Восстановить", trayIcon_);
  QAction* quitAction = new QAction("Выход", trayIcon_);

  connect(restoreAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addAction(quitAction);

  trayIcon_->setContextMenu(trayIconMenu);

  connect(trayIcon_, &QSystemTrayIcon::activated, this, [this](){
    trayIcon_->hide();
    this->showMaximized();
    isSlowMode_ = false;
    sortSignalByModule();
  });
}

void MainWin::slowMode(){

  isSlowMode_ = true;

  ui.treeSignals->clear();

  this->trayIcon_->show();
  this->hide();

}

MainWin::Config MainWin::getConfig(){

  return cng;
}

void MainWin::updateConfig(const MainWin::Config& newCng){
    
  SV_Srv::Config srvCng; {
    srvCng.cycleRecMs = cng.cycleRecMs;
    srvCng.packetSz = cng.packetSz;
    srvCng.outArchiveEna = newCng.outArchiveEna;
    srvCng.outArchiveHourCnt = newCng.outArchiveHourCnt;
    srvCng.outArchiveName = newCng.outArchiveName.toStdString();
    srvCng.outArchivePath = newCng.outArchivePath.toStdString();
    srvCng.outDataBaseEna = newCng.outDataBaseEna;
    srvCng.outDataBaseName = newCng.outDataBaseName.toStdString();
    srvCng.outDataBaseAddr = newCng.outDataBaseAddr.toStdString();
  }
  SV_Srv::setConfig(srvCng);

  cng = newCng;
}

QDialog* MainWin::addNewWindow(const QRect& pos){

  QDialog* graphWin = new QDialog(this, Qt::Window | Qt::WindowStaysOnTopHint);
  graphWin->setObjectName("graphWin");
  graphWin->installEventFilter(this);

  QVBoxLayout* vertLayout = new QVBoxLayout(graphWin);
  vertLayout->setSpacing(0);
  vertLayout->setContentsMargins(5, 5, 5, 5);

  SV_Graph::Config Config(cng.cycleRecMs, cng.packetSz);
  Config.isShowTable = false;

  auto gp = SV_Graph::createGraphPanel(graphWin, Config);
  SV_Graph::setLoadSignalDataCBack(gp, [](const QString& sign){
    return SV_Srv::signalBufferEna(sign.toUtf8().data());
  });
  SV_Graph::setGetCopySignalRefCBack(gp, getCopySignalRefSrv);
  SV_Graph::setGetSignalDataCBack(gp, getSignalDataSrv);
  SV_Graph::setGetSignalAttrCBack(gp, [this](const QString& sname, SV_Graph::SignalAttributes& out){
    if (signAttr_.contains(sname)){
      out.color = signAttr_[sname].color;
      return true;
    }
    return false;
  });
  SV_Graph::setGraphSetting(gp, cng.graphSett);

  graphPanels_[graphWin] = gp;
  vertLayout->addWidget(gp);

  graphWin->show();

  if (!pos.isNull()){
    graphWin->setGeometry(pos);
    graphWin->resize(QSize(pos.width(), pos.height()));
  }

  return graphWin;
}

void MainWin::changeSignColor(QString module, QString name, QColor clr){
  for (auto gp : graphPanels_){
    SV_Graph::setSignalAttr(gp, name + module, SV_Graph::SignalAttributes{ clr });
  }
}
