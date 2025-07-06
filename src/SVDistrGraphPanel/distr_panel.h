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

#include <QVector>
#include <QString>
#include <QColor>
#include <functional>

class QWidget;

namespace SV_Graph {
struct Config;
struct GraphSetting;
struct SignalAttributes;
}

namespace SV_Distr {

  QWidget* createDistrGraphPanel(QWidget* parent, const SV_Graph::Config& cng);

  void setGraphSetting(QWidget* gp, const SV_Graph::GraphSetting&);
  
  using getCopySignalRefCBack = std::function<QMap<QString, SV_Base::SignalData*>()>;
  void setGetCopySignalRefCBack(QWidget* gp, getCopySignalRefCBack f);

  using getSignalDataCBack = std::function<SV_Base::SignalData*(const QString& sign)>;
  void setGetSignalDataCBack(QWidget* gp, getSignalDataCBack f);

  using isLoadSignalDataCBack = std::function<bool(const QString& sign)>;
  void setLoadSignalDataCBack(QWidget* gp, isLoadSignalDataCBack f);

  using lockReadSDataCBack = std::function<void()>;
  void setLockReadSDataCBack(lockReadSDataCBack f);

  using unlockReadSDataCBack = std::function<void()>;
  void setUnlockReadSDataCBack(unlockReadSDataCBack f);

  using getSignalAttrCBack = std::function<bool(const QString& sign, SV_Graph::SignalAttributes& out)>;
  void setGetSignalAttrCBack(QWidget* gp, getSignalAttrCBack f);

  void addSignal(QWidget* gp, QString sname, int section = 0);

  void update(QWidget* gp);
}
