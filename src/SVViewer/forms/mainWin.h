#ifndef MAINWIN_H
#define MAINWIN_H

#include "src/stdafx.h"
#include "mainWin.h"
#include "ui_mainWin.h"
#include "src/treeWidgetExt.h"
#include "src/thrLoadData.h"
#include "SVConfig/SVConfigData.h"
#include "src/structurs.h"

class MainWin : public QMainWindow
{
	Q_OBJECT


    friend QMap<QString, SV_Cng::signalData*> getCopySignalRef();

    friend SV_Cng::signalData* getSignalData(const QString& sign);
	
    friend bool loadSignalData(const QString& sign);

public:
	MainWin(QWidget *parent = 0);
	~MainWin();

	Ui::MainWin ui;

	struct config{

		bool sortByMod;

		int cycleRecMs;
		int packetSz;

        QString initPath;
        QString selOpenDir;
	};

    bool loadModuleVals(QString path);

private:

    config cng;

    QWidget* graphPanel_ = nullptr;
	QDialog* statPanel_ = nullptr;

	thrLoadData* thrLoadData_ = nullptr;

	QMutex mtx_;

    QMap<QString, SV_Cng::moduleData*> moduleRef_;   // ключ - имя модуля
    QMap<QString, SV_Cng::groupData*> groupRef_;     // ключ - имя группы
    QMap<QString, SV_Cng::signalData*> signalRef_;   // ключ - название сигнала
    QMap<QString, fileData*> fileRef_;

    bool init(QString initPath);
    bool writeSettings(QString pathIni);
    bool readSignals(QString path);
    bool writeSignals(QString path);

    void updateGroup(QString group, QString sign);

	void Connect();
	void load();	

	void sortSignalByGroupOrModule(bool byModule);

public slots:
	void actionOpenData();
	void actionOpenStat();
	void selSignalClick(QTreeWidgetItem*, int);
	void selSignalDClick(QTreeWidgetItem * item, int column);
	void selSignalChange(QTreeWidgetItem * item, int column);
    bool loadData(QStringList files);
    void loadDataFinished(bool ok);
};



#endif // MAINWIN_H
