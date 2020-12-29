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

#include <QtCore>
#include "SVConfig/config_data.h"

#ifdef _WIN32
#ifdef SVSTATPANEL_EXPORTS
#define SVSTATPANEL_API __declspec(dllexport)
#else
#define SVSTATPANEL_API __declspec(dllimport)
#endif
#else
#define SVSTATPANEL_API
#endif

namespace SV_Stat {

  struct config {

    int cycleRecMs;
    int packetSz;

    config(int cycleRecMs_ = 100, int packetSz_ = 10) :
      cycleRecMs(cycleRecMs_),
      packetSz(packetSz_) {}
  };

  SVSTATPANEL_API QDialog* createStatDialog(QWidget* parent, config);

  typedef QMap<QString, SV_Base::SignalData*>(*pf_getCopySignalRef)();
  SVSTATPANEL_API void setGetCopySignalRef(QDialog* stPanel, pf_getCopySignalRef f);

  typedef SV_Base::SignalData *(*pf_getSignalData)(const QString &sign);
  SVSTATPANEL_API void setGetSignalData(QDialog* stPanel, pf_getSignalData f);

  typedef bool(*pf_loadSignalData)(const QString& sign);
  SVSTATPANEL_API void setLoadSignalData(QDialog* stPanel, pf_loadSignalData f);

  typedef QPair<qint64, qint64>(*pf_getTimeInterval)();
  SVSTATPANEL_API void setGetTimeInterval(QDialog* stPanel, pf_getTimeInterval f);

  typedef void(*pf_setTimeInterval)(qint64, qint64);
  SVSTATPANEL_API void setSetTimeInterval(QDialog* stPanel, pf_setTimeInterval f);

}