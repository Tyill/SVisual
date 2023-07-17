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

#include "SVBase/base.h"

#include <QString>
#include <QMap>
#include <QColor>

class QDialog;
class QWidget;

namespace SV_Script {

  enum class ModeGr {
    Viewer = 0,
    Player = 1,
  };

  struct Config {

    int cycleRecMs;
    int packetSz;

    Config(int cycleRecMs_ = 100, int packetSz_ = 10) :
      cycleRecMs(cycleRecMs_),
      packetSz(packetSz_) {}
  };

  QDialog* getScriptDialog(QWidget* parent, Config, ModeGr);

  void startUpdateThread(QDialog* stPanel);

  using getCopySignalRefCBack = std::function<QMap<QString, SV_Base::SignalData*>()>;
  void setGetCopySignalRef(QDialog* stPanel, getCopySignalRefCBack f);

  using getModuleDataCBack = std::function<SV_Base::ModuleData*(const QString &module)>;
  void setGetModuleData(QDialog* stPanel, getModuleDataCBack f);

  using getSignalDataCBack = std::function<SV_Base::SignalData*(const QString &sign)>;
  void setGetSignalData(QDialog* stPanel, getSignalDataCBack f);

  using addSignalCBack = std::function<bool(SV_Base::SignalData*)>;
  void setAddSignal(QDialog* stPanel, addSignalCBack f);

  using addModuleCBack = std::function<bool(SV_Base::ModuleData*)>;
  void setAddModule(QDialog* stPanel, addModuleCBack f);

  using lockReadSDataCBack = std::function<void()>;
  void setLockReadSData(lockReadSDataCBack f);

  using unlockReadSDataCBack = std::function<void()>;
  void setUnlockReadSData(unlockReadSDataCBack f);
  
  using changeSignColorCBack = std::function<void(const QString &module, const QString &sign, const QColor& color)>;
  void setChangeSignColor(QDialog* stPanel, changeSignColorCBack f);

  bool isActiveScript(QDialog* stPanel, const QString& fname);

  void activeScript(QDialog* stPanel, const QString& fname);

  void deactiveScript(QDialog* stPanel, const QString& fname);

  void refreshScript(QDialog* stPanel, const QString& fname);

  void restartScript(QDialog* stPanel);

  using isLoadSignalDataCBack = std::function<bool(const QString &sign)>;
  void setLoadSignalData(QDialog* stPanel, isLoadSignalDataCBack f);

  // обновление данных callBack
  using updateSignalsCBack = std::function<void()>;
  void setUpdateSignalsCBack(QDialog* stPanel, updateSignalsCBack);

  // добавление сигнала callBack
  using addSignalsCBack = std::function<void()>;
  void setAddSignalsCBack(QDialog* stPanel, addSignalsCBack);

  // модуль подключен
  using moduleConnectCBack = std::function<void(const QString& module)>;
  void setModuleConnectCBack(QDialog* stPanel, moduleConnectCBack);
}