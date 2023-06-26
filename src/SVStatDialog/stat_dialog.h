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

#include <QString>
#include <QMap>
#include "SVBase/base.h"

class QDialog;

namespace SV_Stat {

  struct Config {

    int cycleRecMs;
    int packetSz;

    Config(int cycleRecMs_ = 100, int packetSz_ = 10) :
      cycleRecMs(cycleRecMs_),
      packetSz(packetSz_) {}
  };

  QDialog* createStatDialog(QWidget* parent, Config);

  using getCopySignalRefCBack = std::function<QMap<QString, SV_Base::SignalData*>()>;
  void setGetCopySignalRef(QDialog* stPanel, getCopySignalRefCBack f);

  using getSignalDataCBack = std::function<SV_Base::SignalData*(const QString &sign)>;
  void setGetSignalData(QDialog* stPanel, getSignalDataCBack f);

  using isLoadSignalDataCBack = std::function<bool(const QString &sign)>;
  void setLoadSignalData(QDialog* stPanel, isLoadSignalDataCBack f);

  using getTimeIntervalCBack = std::function< QPair<qint64, qint64>()>;
  void setGetTimeInterval(QDialog* stPanel, getTimeIntervalCBack f);

  using setTimeIntervalCBack = std::function<void(qint64, qint64)>;
  void setSetTimeInterval(QDialog* stPanel, setTimeIntervalCBack f);

}