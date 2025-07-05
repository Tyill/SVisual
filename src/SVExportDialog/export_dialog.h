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
#include <functional>

class QDialog;
class QWidget;

namespace SV_Exp {

  struct Config {

    int cycleRecMs;
    int packetSz;

    Config(int cycleRecMs_ = 100, int packetSz_ = 10) :
      cycleRecMs(cycleRecMs_),
      packetSz(packetSz_) {}
  };

  QDialog* createExportDialog(QWidget* parent, Config);

  using lockReadSDataCBack = std::function<void()>;
  void setLockReadSData(lockReadSDataCBack f);

  using unlockReadSDataCBack = std::function<void()>;
  void setUnlockReadSData(unlockReadSDataCBack f);

  using getCopySignalRefCBack = std::function<QMap<QString, SV_Base::SignalData*>()>;
  void setGetCopySignalRef(QDialog* expDialog, getCopySignalRefCBack f);

  using getCopyModuleRefCBack = std::function<QMap<QString, SV_Base::ModuleData*>()>;
  void setGetCopyModuleRef(QDialog* expDialog, getCopyModuleRefCBack f);

  using getSignalDataCBack = std::function<SV_Base::SignalData*(const QString &sign)>;
  void setGetSignalData(QDialog* expDialog, getSignalDataCBack f);

  using isLoadSignalDataCBack = std::function<bool(const QString &sign)>;
  void setLoadSignalData(QDialog* expDialog, isLoadSignalDataCBack f);

  using getTimeIntervalCBack = std::function< QPair<qint64, qint64>()>;
  void setGetTimeInterval(QDialog* expDialog, getTimeIntervalCBack f);

  using setTimeIntervalCBack = std::function<void(qint64, qint64)>;  
  void setSetTimeInterval(QDialog* expDialog, setTimeIntervalCBack f);

}
