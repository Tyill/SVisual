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

#include "GeneratedFiles/ui_main_win.h"
#include "SVMisc/logger.h"
#include "SVGraphPanel/graph_panel.h"
#include "SVMonitor/src/structurs.h"

#include <QMainWindow>

class SettingsDialog;
class GraphSettingDialog;
class EventTableDialog;
class QTimer;
class QNetworkAccessManager;
class QSystemTrayIcon;
class DbProvider;
class SerialPortReader;

class MainWin : public QMainWindow
{
  Q_OBJECT
    
public:

  struct Config{

    QString dirPath;
    QString initPath;
    QString selOpenDir;

    bool outArchiveEna;        ///< запись архива активна
    QString outArchivePath;    ///< запись архива путь
    QString outArchiveName;    ///< запись архива имя файла
    int outArchiveHourCnt;     ///< запись архива размер файла, час

    bool outDataBaseEna;       ///< запись в БД активна
    QString outDataBaseName;
    QString outDataBaseAddr;

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

    // zabbix
    bool zabbix_ena;
    QString zabbix_addr;       ///< agent ip
    int zabbix_port;           ///< agent port

    int toutLoadWinStateSec;

    SV_Graph::GraphSetting graphSett;
  };
 
  MainWin(QWidget *parent = 0);
  ~MainWin();

  void statusMess(const QString& mess);

  void updateConfig(const Config&);
  Config getConfig();
  
  void updateGraphSetting(const SV_Graph::GraphSetting&);

private:
  
  Ui::MainWin ui;
  bool isSlowMode_ = false;

  QVector<SerialPortReader*> comReaders_;

  Config cng;

  SV_Misc::Logger lg_;

  QMap<QObject*, QWidget*> graphPanels_;
  QDialog* exportDialog_ = nullptr;
  QDialog* scriptDialog_ = nullptr;
  QDialog* triggerDialog_ = nullptr;
  EventTableDialog* eventTableDialog_ = nullptr;
  SettingsDialog* settingsDialog_ = nullptr;
  GraphSettingDialog* graphSettDialog_ = nullptr;
  QSystemTrayIcon* trayIcon_ = nullptr;

  DbProvider* db_ = nullptr;

  QNetworkAccessManager* netManager_ = nullptr;

  QSet<QString> signExist_;

  QMap<QString, SignalAttr> signAttr_;
  QTimer* tmWinSetts_;

  bool eventFilter(QObject *target, QEvent *event);

  bool writeSettings(QString pathIni);
  bool init(QString initPath);
  void connects();
  void load();

  void initTrayIcon();

  void sortSignalByModule();
  void contextMenuEvent(QContextMenuEvent * event);
  QDialog* addNewWindow(const QRect& pos);

public slots:
  void slowMode();
  void contextMenuClick(QAction*);
  void updateTblSignal();
  void updateSignals();
  void moduleConnect(QString module);
  void moduleDisconnect(QString module);
  void onTrigger(QString trigger);
  void changeSignColor(QString module, QString signal, QColor color);
};
