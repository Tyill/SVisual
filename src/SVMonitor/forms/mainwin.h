#pragma once

#include "forms/ui_mainwin.h"
#include "SVConfig/SVConfigData.h"
#include "SVServer/SVServer.h"
#include "src/sql.h"
#include "src/structurs.h"
#include "src/comReader.h"

class settingsPanel;
class eventOrderWin;

class MainWin : public QMainWindow
{
	Q_OBJECT
			
public:

	struct config{

		QString dirPath;
		QString initPath;

		bool outArchiveEna;          ///< запись архива активна
		QString outArchivePath;      ///< запись архива путь
		QString outArchiveName;      ///< запись архива имя файла
		int outArchiveHourCnt;       ///< запись архива размер файла, час

		int cycleRecMs;           ///< период записи - задает пользователь
		int packetSz;             ///< размер пакета - задает пользователь

		bool com_ena;            ///< запись по com
		QString com_name;        ///< COM0..
		int com_speed;           ///< 9600..

		QString dbPath;         ///< путь к бд

		// связь по TCP
		QString tcp_addr;       ///< ip
		int tcp_port;           ///< port
	};

	MainWin(QWidget *parent = 0);
	~MainWin();

    void updateConfig(config);
    config getConfig();

	void addUserData(userEventData);
	void delUserData(QString);
	userEventData* getUserData(QString);

	QVector<uEvent> getEvents(QDateTime, QDateTime);

private:

	Ui::MainWin ui;
	bool isSlowMode_ = false;

	SerialPortReader* pComReader_ = nullptr;

	config cng;
	SV_Srv::config srvCng;

	bool initOk_ = false;

	QWidget* graphPanel_ = nullptr;
	eventOrderWin* orderWin_ = nullptr;
	QWidget* trgPanel_ = nullptr;
	settingsPanel* settPanel_ = nullptr;
	QSystemTrayIcon* trayIcon_ = nullptr;
	sql* db = nullptr;

	QMutex mtx_;

	QSet<QString> signExist_;
	QMap<QString, userEventData> userEvents_;

	bool writeSettings(QString pathIni);
	bool init(QString initPath);
	void Connect();
	void load();

	void initTrayIcon();
	
	void sortSignalByModule();
	void contextMenuEvent(QContextMenuEvent * event);
	
public slots:
	void slowMode();
	void selSignalClick(QTreeWidgetItem*, int);
	void selSignalDClick(QTreeWidgetItem * item, int column);
	void selSignalChange(QTreeWidgetItem * item, int column);
	void contextMenuClick(QAction*);
	void StatusTxtMess(QString mess);
	void updateTblSignal();
	void updateSignals();
	void moduleConnect(QString module);
	void moduleDisconnect(QString module);
	void onTrigger(QString trigger);
};
