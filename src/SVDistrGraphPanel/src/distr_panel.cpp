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

#include "SVDistrGraphPanel/distr_panel.h"
#include "forms/graph_panel_widget.h"

namespace SV_Distr {

  lockReadSDataCBack pfLockReadSData = nullptr;
  unlockReadSDataCBack pfUnlockReadSData = nullptr;

  QWidget* createDistrGraphPanel(QWidget* parent, const SV_Graph::Config& cng) {

    return new GraphPanelWidget(parent, cng);
  }

  void setGraphSetting(QWidget* gp, const SV_Graph::GraphSetting& gs) {
      if (gp) {
          static_cast<GraphPanelWidget*>(gp)->setGraphSetting(gs);
      }
  }

  void setGetCopySignalRefCBack(QWidget* gp, getCopySignalRefCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfGetCopySignalRef = f;
      }
  }

  void setGetSignalDataCBack(QWidget* gp, getSignalDataCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfGetSignalData = f;
      }
  }

  void setGetSignalAttrCBack(QWidget* gp, getSignalAttrCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfGetSignalAttr = f;
      }
  }

  void setLoadSignalDataCBack(QWidget* gp, isLoadSignalDataCBack f) {
      if (gp && f) {
          static_cast<GraphPanelWidget*>(gp)->pfLoadSignalData = f;
      }
  }

  void setLockReadSDataCBack(lockReadSDataCBack f) {
      if (f) {
          pfLockReadSData = f;
      }
  }

  void setUnlockReadSDataCBack(unlockReadSDataCBack f) {
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

LockerReadSDataGraph::LockerReadSDataGraph(){
    if (SV_Distr::pfLockReadSData) SV_Distr::pfLockReadSData();
}
LockerReadSDataGraph::~LockerReadSDataGraph() {
    if (SV_Distr::pfUnlockReadSData) SV_Distr::pfUnlockReadSData();
}
}
