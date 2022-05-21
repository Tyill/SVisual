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

#include "GeneratedFiles/ui_main_win.h"
#include "SVBase/base.h"
#include "SVGraphPanel/graph_panel.h"
#include "SVViewer/src/structurs.h"

class GraphSettingDialog;
class ThrLoadData;

class MainWin : public QMainWindow
{
  Q_OBJECT

  friend QMap<QString, SV_Base::SignalData*> getCopySignalRef();
  friend QMap<QString, SV_Base::ModuleData*> getCopyModuleRef();
  friend SV_Base::SignalData* getSignalData(const QString& sign);
  friend SV_Base::ModuleData* getModuleData(const QString& md);
  friend bool loadSignalData(const QString& sign);
  friend bool addSignal(SV_Base::SignalData* sd);
  friend bool addModule(SV_Base::ModuleData* md);
  friend QVector<QString> getModuleSignals(const QString& md);

public:
  MainWin(QWidget *parent = 0);
  ~MainWin();

  Ui::MainWin ui;

  struct Config {

    bool sortByMod;

    int cycleRecMs;
    int packetSz;

    QString initPath;
    QString selOpenDir;

    SV_Graph::GraphSetting graphSett;
  };

  bool loadModuleVals(QString path);

  void updateGraphSetting(const SV_Graph::GraphSetting&);

private:

  Config cng;

  QDialog* exportPanel_ = nullptr;
  QMap<QObject*, QWidget*> graphPanels_;
  QDialog* statPanel_ = nullptr;
  GraphSettingDialog* graphSettPanel_ = nullptr;
  QDialog* scriptPanel_ = nullptr;

  ThrLoadData* thrLoadData_ = nullptr;

  QMutex mtx_;

  QMap<QString, SignalAttr> signAttr_;

  QMap<QString, SV_Base::ModuleData*> moduleRef_;   // ключ - имя модуля
  QMap<QString, SV_Base::GroupData*> groupRef_;     // ключ - имя группы
  QMap<QString, SV_Base::SignalData*> signalRef_;   // ключ - название сигнала
  QMap<QString, FileData*> fileRef_;

  bool init(QString initPath);
  bool writeSettings(QString pathIni);
  bool readSignals(QString path);
  bool writeSignals(QString path);
  QDialog* addNewWindow(const QRect& pos);

  bool eventFilter(QObject *target, QEvent *event);

  void updateGroup(QString group, QString sign);

  void Connect();
  void load();

  void sortSignalByGroupOrModule(bool byModule);
  void contextMenuEvent(QContextMenuEvent * event);

  public slots:
  void updateTblSignal();
  void updateSignals();
  void actionOpenData();
  void actionOpenStat();
  bool loadData(QStringList files);
  void loadDataFinished(bool ok);
  void contextMenuClick(QAction*);
  void changeSignColor(QString module, QString signal, QColor color);
};



#endif // MAINWIN_H
