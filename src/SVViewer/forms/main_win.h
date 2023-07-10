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

#include <QMutex>

class GraphSettingDialog;
class ThrLoadData;
class SettingsDialog;
class DbClickHouseLoader;

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
  friend class FileLoader;
  friend class DbClickHouseLoader;

public:
  MainWin(QWidget *parent = 0);
  ~MainWin();

  Ui::MainWin ui;

  struct Config {

    bool sortByMod{};

    int cycleRecMs{};
    int packetSz{};

    QString initPath;
    QString selOpenDir;

    bool inputDataBaseEna{};
    QString inputDataBaseName;
    QString inputDataBaseAddr;

    int toutLoadWinStateSec = 10;

    SV_Graph::GraphSetting graphSett;
  };

  void updateConfig(const Config&);
  Config getConfig()const;

  void updateGraphSetting(const SV_Graph::GraphSetting&);

  bool loadSDataRequest(QWidget* from, const QString& sname);

public slots:
  void updateTblSignal();
  void updateSignals();
  void actionOpenData();
  void actionOpenStat();
  void contextMenuClick(QAction*);
  void changeSignColor(const QString& module, const QString& signal, const QColor& color);

protected:
  bool eventFilter(QObject *target, QEvent *event) override;
  void contextMenuEvent(QContextMenuEvent * event) override;

private:
  void connects();
  void load();

  bool readSettings(const QString& initPath);
  bool writeSettings(const QString& pathIni);

  bool readSignals(const QString& path);
  bool writeSignals(const QString& path);
  QDialog* addNewWindow(const QRect& pos);

  void updateGroup(const QString& group, const QString& sign);
  void sortSignalByGroupOrModule(bool byModule);

  void showMessageDialog(const QString& mess);

private:
  Config cng;

  QDialog* exportPanel_ = nullptr;
  QMap<QObject*, QWidget*> graphPanels_;
  QDialog* statPanel_ = nullptr;
  GraphSettingDialog* graphSettPanel_ = nullptr;
  QDialog* scriptPanel_ = nullptr;

  SettingsDialog* settingsDialog_ = nullptr;

  QTimer* tmWinSetts_ = nullptr;

  QMutex mtx_;

  QMap<QString, SignalAttr> signAttr_;

  QMap<QString, SV_Base::ModuleData*> moduleRef_;   // ключ - имя модуля
  QMap<QString, SV_Base::GroupData*> groupRef_;     // ключ - имя группы
  QMap<QString, SV_Base::SignalData*> signalRef_;   // ключ - название сигнала
  QMap<QString, FileData*> fileRef_;

  QMap<QObject*, DbClickHouseLoader*> m_chLoaders;
};



#endif // MAINWIN_H
