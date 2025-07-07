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

  enum class ModeGr {
    Viewer = 0,
    Player = 1,
    Distr = 2,
  };

  struct Config {
    bool isShowTable = true;
    int cycleRecMs;
    int packetSz;

    ModeGr mode;

    Config(int cycleRecMs_ = 100, int packetSz_ = 10, ModeGr mode_ = ModeGr::Player) :
      cycleRecMs(cycleRecMs_),
      packetSz(packetSz_),
      mode(mode_) {}
  };

  struct GraphSetting {
    bool darkTheme = false;
    bool signBoolOnTop = false;
    int transparent = 0;
    int lineWidth = 1;
    int gapTolerance = 1;
  };

  struct SignalAttributes {
    QColor color;
  };

  struct AxisAttributes {
    bool isAuto = true;
    double min = 0.0, max = 0.0, step = 0.0;
  };

  QWidget* createGraphPanel(QWidget* parent, const Config& cng);

  void setGraphSetting(QWidget* gp, const GraphSetting&);

  void setSignalAttr(QWidget* gp, const QString& sign, const SignalAttributes&);

  void setAxisAttr(QWidget* gp, const QVector<AxisAttributes>&);

  QVector<AxisAttributes> getAxisAttr(QWidget* gp);

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


  using getSignalAttrCBack = std::function<bool(const QString& sign, SignalAttributes& out)>;
  void setGetSignalAttrCBack(QWidget* gp, getSignalAttrCBack f);

  void addSignal(QWidget* gp, const QString& sign, int section = 0);

  void update(QWidget* gp);

  QPair<qint64, qint64> getTimeInterval(QWidget* gp);

  void setTimeInterval(QWidget* gp, qint64 stTime, qint64 enTime);

  QVector<QVector<QString>> getLocateSignals(QWidget *gp);

}
