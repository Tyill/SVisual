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

#include "SVGraphPanel/graph_panel.h"
#include "SVGraphPanel/forms/graph_panel_widget.h"

namespace SV_Graph {

  lockReadSDataCBack pfLockReadSData = nullptr;
  unlockReadSDataCBack pfUnlockReadSData = nullptr;

  QWidget* createGraphPanel(QWidget* parent, const SV_Graph::Config& cng) {

    return new GraphPanelWidget(parent, cng);
  }

  void setGraphSetting(QWidget* gp, const GraphSetting& gs) {
      if (gp) {
          static_cast<GraphPanelWidget*>(gp)->setGraphSetting(gs);
      }
  }

  void setSignalAttr(QWidget* gp, const QString& sign, const SignalAttributes& att) {
      if (gp) {
          static_cast<GraphPanelWidget*>(gp)->setSignalAttr(sign, att);
      }
  }

  void setAxisAttr(QWidget* gp, const QVector<AxisAttributes>& attr) {
      if (gp) {
          static_cast<GraphPanelWidget*>(gp)->setAxisAttr(attr);
      }
  }

  QVector<AxisAttributes> getAxisAttr(QWidget* gp) {
      if (gp) {
          return static_cast<GraphPanelWidget*>(gp)->getAxisAttr();
      }else {
          return QVector<AxisAttributes>();
      }
  }

  void setGetCopySignalRef(QWidget* gp, getCopySignalRefCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfGetCopySignalRef = f;
      }
  }

  void setGetSignalData(QWidget* gp, getSignalDataCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfGetSignalData = f;
      }
  }

  void setGetSignalAttr(QWidget* gp, getSignalAttrCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfGetSignalAttr = f;
      }
  }

  void setLoadSignalData(QWidget* gp, isLoadSignalDataCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfLoadSignalData = f;
      }
  }

  void setLockReadSData(lockReadSDataCBack f) {
      if (f) {
          pfLockReadSData = f;
      }
  }

  void setUnlockReadSData(unlockReadSDataCBack f) {
      if (f) {
          pfUnlockReadSData = f;
      }
  }

  void addSignal(QWidget* gp, QString sname, int section) {
      if (gp) {
          static_cast<GraphPanelWidget*>(gp)->addSignalOnGraph(sname, section);
      }
  }

  void update(QWidget* gp) {
      if (gp) {
          static_cast<GraphPanelWidget*>(gp)->updateSignals();
      }
  }

  QPair<qint64, qint64> getTimeInterval(QWidget* gp) {
      if (gp) {
          return static_cast<GraphPanelWidget*>(gp)->getTimeInterval();
      }else {
          return QPair<qint64, qint64>();
      }
  }

  void setTimeInterval(QWidget* gp, qint64 stTime, qint64 enTime) {
    if (gp) {
      static_cast<GraphPanelWidget*>(gp)->setTimeInterval(stTime, enTime);
      static_cast<GraphPanelWidget*>(gp)->ui.axisTime->update();
      static_cast<GraphPanelWidget*>(gp)->resizeByValue();
    }
  }

  QVector<QVector<QString>> getLocateSignals(QWidget* gp) {
      if (gp) {
          return static_cast<GraphPanelWidget*>(gp)->getLocateSignals();
      }else {
          return QVector<QVector<QString>>();
      }
  }

LockerReadSDataGraph::LockerReadSDataGraph(){
    if (SV_Graph::pfLockReadSData) SV_Graph::pfLockReadSData();
}
LockerReadSDataGraph::~LockerReadSDataGraph() {
    if (SV_Graph::pfUnlockReadSData) SV_Graph::pfUnlockReadSData();
}
}
