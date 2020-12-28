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

#include <map>
#include <string>
#include <QtCore>
#include "SVConfig/SVConfigData.h"

#ifdef _WIN32
#ifdef SVSCRIPTPANEL_EXPORTS
#define SVSCRIPTPANEL_API __declspec(dllexport)
#else
#define SVSCRIPTPANEL_API __declspec(dllimport)
#endif
#else
#define SVSCRIPTPANEL_API
#endif

namespace SV_Script {

    enum class modeGr{
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

    SVSCRIPTPANEL_API QDialog* createScriptPanel(QWidget* parent, config, modeGr);

    SVSCRIPTPANEL_API void startUpdateThread(QDialog* stPanel);
      
    typedef QMap<QString, SV_Base::SignalData*>(*pf_getCopySignalRef)();
  SVSCRIPTPANEL_API void setGetCopySignalRef(QDialog* stPanel, pf_getCopySignalRef f);

    typedef SV_Base::ModuleData *(*pf_getModuleData)(const QString &module);
    SVSCRIPTPANEL_API void setGetModuleData(QDialog* stPanel, pf_getModuleData f);

    typedef SV_Base::SignalData *(*pf_getSignalData)(const QString &sign);
  SVSCRIPTPANEL_API void setGetSignalData(QDialog* stPanel, pf_getSignalData f);

    typedef bool(*pf_addSignal)(SV_Base::SignalData*);
    SVSCRIPTPANEL_API void setAddSignal(QDialog* stPanel, pf_addSignal f);

    typedef bool(*pf_addModule)(SV_Base::ModuleData*);
    SVSCRIPTPANEL_API void setAddModule(QDialog* stPanel, pf_addModule f);

    typedef void(*pf_changeSignColor)(const QString &module, const QString &sign, const QColor& color);
    SVSCRIPTPANEL_API void setChangeSignColor(QDialog* stPanel, pf_changeSignColor f);

    SVSCRIPTPANEL_API bool isActiveScript(QDialog* stPanel, const QString& fname);

    SVSCRIPTPANEL_API void activeScript(QDialog* stPanel, const QString& fname);
    
    SVSCRIPTPANEL_API void deactiveScript(QDialog* stPanel, const QString& fname);

    SVSCRIPTPANEL_API void refreshScript(QDialog* stPanel, const QString& fname);

    typedef bool(*pf_loadSignalData)(const QString& sign);
  SVSCRIPTPANEL_API void setLoadSignalData(QDialog* stPanel, pf_loadSignalData f);

    // обновление данных callBack
    typedef void(*pf_updateSignalsCBack)();
    SVSCRIPTPANEL_API void setUpdateSignalsCBack(QDialog* stPanel, pf_updateSignalsCBack);

    // добавление сигнала callBack
    typedef void(*pf_addSignalsCBack)();
    SVSCRIPTPANEL_API void setAddSignalsCBack(QDialog* stPanel, pf_addSignalsCBack);

    // модуль подключен
    typedef void(*pf_moduleConnectCBack)(const QString& module);
    SVSCRIPTPANEL_API void setModuleConnectCBack(QDialog* stPanel, pf_moduleConnectCBack);
}