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
#include "SVConfig/config_limits.h"
#include <QTranslator>

namespace SV_Graph {

  QWidget* createGraphPanel(QWidget* parent, const SV_Graph::Config& cng) {

    return new GraphPanelWidget(parent, cng);
  }

  void setGraphSetting(QWidget* gp, const GraphSetting& gs) {

    if (gp)
      ((GraphPanelWidget*)gp)->setGraphSetting(gs);
  }

  void setSignalAttr(QWidget* gp, const QString& sign, const SignalAttributes& att) {

    if (gp)
      ((GraphPanelWidget*)gp)->setSignalAttr(sign, att);
  }

  void setAxisAttr(QWidget* gp, const QVector<AxisAttributes>& attr) {

    if (gp)
      ((GraphPanelWidget*)gp)->setAxisAttr(attr);
  }

  QVector<AxisAttributes> getAxisAttr(QWidget* gp) {

    if (gp)
      return ((GraphPanelWidget*)gp)->getAxisAttr();
    else
      return QVector<AxisAttributes>();
  }

  void setGetCopySignalRef(QWidget* gp, pf_getCopySignalRef f) {

    if (gp && f)
      ((GraphPanelWidget*)gp)->pfGetCopySignalRef = f;
  }

  void setGetSignalData(QWidget* gp, pf_getSignalData f) {

    if (gp && f)
      ((GraphPanelWidget*)gp)->pfGetSignalData = f;
  }

  void setGetSignalAttr(QWidget* gp, pf_getSignalAttr f) {

    if (gp && f)
      ((GraphPanelWidget*)gp)->pfGetSignalAttr = f;
  }

  void setLoadSignalData(QWidget* gp, pf_loadSignalData f) {

    if (gp && f)
      ((GraphPanelWidget*)gp)->pfLoadSignalData = f;
  }

  void addSignal(QWidget* gp, QString sname, int section) {

    if (gp)
      ((GraphPanelWidget*)gp)->addSignalOnGraph(sname, section);
  }

  void update(QWidget* gp) {

    if (gp)
      ((GraphPanelWidget*)gp)->updateSignals();
  }

  QPair<qint64, qint64> getTimeInterval(QWidget* gp) {

    if (gp)
      return ((GraphPanelWidget*)gp)->getTimeInterval();

    return QPair<qint64, qint64>();
  }

  void setTimeInterval(QWidget* gp, qint64 stTime, qint64 enTime) {

    if (gp) {

      ((GraphPanelWidget*)gp)->setTimeInterval(stTime, enTime);

      ((GraphPanelWidget*)gp)->ui.axisTime->update();

      ((GraphPanelWidget*)gp)->resizeByValue();
    }
  }

  QVector<QVector<QString>> getLocateSignals(QWidget* gp) {

    return gp ? ((GraphPanelWidget*)gp)->getLocateSignals() : QVector<QVector<QString>>();

  }
}