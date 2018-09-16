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
#ifndef MAINWIN_H
#define MAINWIN_H

#include "src/stdafx.h"
#include "mainWin.h"
#include "ui_mainWin.h"
//#include "exportWin.h"
#include "src/treeWidgetExt.h"
#include "src/thrLoadData.h"
#include "SVConfig/SVConfigData.h"
#include "src/structurs.h"

class MainWin : public QMainWindow
{
	Q_OBJECT


    friend QMap<QString, SV_Cng::signalData*> getCopySignalRef();
    friend QMap<QString, SV_Cng::moduleData*> getCopyModuleRef();
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
   
    QDialog* exportPanel_ = nullptr;
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
