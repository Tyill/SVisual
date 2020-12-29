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

#include "SVConfig/config_data.h"

#include <QString>
#include <QColor>

class QDialog;
class QWidget;

namespace SV_Script {

  enum class modeGr {
    viewer = 0,
    player = 1,
  };

  struct config {

    int cycleRecMs;
    int packetSz;

    config(int cycleRecMs_ = 100, int packetSz_ = 10) :
      cycleRecMs(cycleRecMs_),
      packetSz(packetSz_) {}
  };

  QDialog* createScriptDialog(QWidget* parent, config, modeGr);

  void startUpdateThread(QDialog* stPanel);

  typedef QMap<QString, SV_Base::SignalData*>(*pf_getCopySignalRef)();
  void setGetCopySignalRef(QDialog* stPanel, pf_getCopySignalRef f);

  typedef SV_Base::ModuleData *(*pf_getModuleData)(const QString &module);
  void setGetModuleData(QDialog* stPanel, pf_getModuleData f);

  typedef SV_Base::SignalData *(*pf_getSignalData)(const QString &sign);
  void setGetSignalData(QDialog* stPanel, pf_getSignalData f);

  typedef bool(*pf_addSignal)(SV_Base::SignalData*);
  void setAddSignal(QDialog* stPanel, pf_addSignal f);

  typedef bool(*pf_addModule)(SV_Base::ModuleData*);
  void setAddModule(QDialog* stPanel, pf_addModule f);

  typedef void(*pf_changeSignColor)(const QString &module, const QString &sign, const QColor& color);
  void setChangeSignColor(QDialog* stPanel, pf_changeSignColor f);

  bool isActiveScript(QDialog* stPanel, const QString& fname);

  void activeScript(QDialog* stPanel, const QString& fname);

  void deactiveScript(QDialog* stPanel, const QString& fname);

  void refreshScript(QDialog* stPanel, const QString& fname);

  typedef bool(*pf_loadSignalData)(const QString& sign);
  void setLoadSignalData(QDialog* stPanel, pf_loadSignalData f);

  // обновление данных callBack
  typedef void(*pf_updateSignalsCBack)();
  void setUpdateSignalsCBack(QDialog* stPanel, pf_updateSignalsCBack);

  // добавление сигнала callBack
  typedef void(*pf_addSignalsCBack)();
  void setAddSignalsCBack(QDialog* stPanel, pf_addSignalsCBack);

  // модуль подключен
  typedef void(*pf_moduleConnectCBack)(const QString& module);
  void setModuleConnectCBack(QDialog* stPanel, pf_moduleConnectCBack);
}