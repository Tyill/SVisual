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

#include <QNetworkAccessManager>

#include "forms/ui_mainwin.h"
#include "SVConfig/SVConfigData.h"
#include "SVServer/SVServer.h"
#include "SVAuxFunc/mt_log.h"
#include "SVGraphPanel/SVGraphPanel.h" 
#include "src/dbProvider.h"
#include "src/comReader.h"

class settingsPanel;
class graphSettingPanel;
class eventOrderWin;

class MainWin : public QMainWindow
{
    Q_OBJECT

        friend void statusMess(const QString& mess);

public:

	struct config{

		QString dirPath;
		QString initPath;
        QString selOpenDir;

		bool outArchiveEna;        ///< запись архива активна
		QString outArchivePath;    ///< запись архива путь
		QString outArchiveName;    ///< запись архива имя файла
		int outArchiveHourCnt;     ///< запись архива размер файла, час

		int cycleRecMs;            ///< период записи - задает пользователь
		int packetSz;              ///< размер пакета - задает пользователь

		bool com_ena;              ///< запись по com
        QVector<QPair<QString, QString>> com_ports;   ///< COM name, speed
                                   
		QString dbPath;            ///< путь к бд

		// связь по TCP
		QString tcp_addr;          ///< ip
		int tcp_port;              ///< port

        // web
        bool web_ena;              
        QString web_addr;          ///< ip
        int web_port;              ///< port

        
        SV_Graph::graphSetting graphSett;
	};

    SV_Aux::Logger lg;

	MainWin(QWidget *parent = 0);
	~MainWin();

    void updateConfig(config);
    config getConfig();

	QVector<uEvent> getEvents(QDateTime, QDateTime);

    void updateGraphSetting(const SV_Graph::graphSetting&);

private:

   
	Ui::MainWin ui;
	bool isSlowMode_ = false;
    
	QVector<SerialPortReader*> comReaders_;

	config cng;
	SV_Srv::config srvCng;

	bool initOk_ = false;

    QMap<QObject*, QWidget*> graphPanels_;
    QDialog* exportPanel_ = nullptr;
    QDialog* scriptPanel_ = nullptr;
    QDialog* triggerPanel_ = nullptr;
	eventOrderWin* orderWin_ = nullptr;   
	settingsPanel* settPanel_ = nullptr;
    graphSettingPanel* graphSettPanel_ = nullptr;
	QSystemTrayIcon* trayIcon_ = nullptr;

	dbProvider* db_ = nullptr;
    
    QNetworkAccessManager* netManager_ = nullptr;

	QSet<QString> signExist_;
	
    QMap<QString, signalAttr> signAttr_;

    bool eventFilter(QObject *target, QEvent *event);

	bool writeSettings(QString pathIni);
	bool init(QString initPath);
	void Connect();
	void load();

	void initTrayIcon();
	
	void sortSignalByModule();
	void contextMenuEvent(QContextMenuEvent * event);
    QDialog* addNewWindow(const QRect& pos);

public slots:
	void slowMode();
	void selSignalClick(QTreeWidgetItem*, int);
	void selSignalDClick(QTreeWidgetItem * item, int column);
	void selSignalChange(QTreeWidgetItem * item, int column);
	void contextMenuClick(QAction*);
	void updateTblSignal();
	void updateSignals();
	void moduleConnect(QString module);
	void moduleDisconnect(QString module);
	void onTrigger(QString trigger);
    
};
