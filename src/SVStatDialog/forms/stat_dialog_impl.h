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

#include "ui_stat_dialog.h"
#include "SVConfig/config_data.h"
#include "SVStatDialog/stat_dialog.h"

class StatDialog : public QDialog
{
  Q_OBJECT

public:

  SV_Stat::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
  SV_Stat::pf_getSignalData pfGetSignalData = nullptr;
  SV_Stat::pf_loadSignalData pfLoadSignalData = nullptr;
  SV_Stat::pf_setTimeInterval pfSetTimeInterval = nullptr;
  SV_Stat::pf_getTimeInterval pfGetTimeInterval = nullptr;

  StatDialog(QWidget *parent, SV_Stat::Config);
  ~StatDialog();

private:
  Ui::StatDialog ui;

  SV_Stat::Config cng;

  struct valSData {
    int changeCnt;
    int duration;
    int durationMean;
  };

  struct graphSignData {
    QString sign;
    QString name;
    QMap<int, valSData> valData;
    QVector<QPair<int, int>> hist;
  };


  bool diapEna_ = false;

  QMap<QString, graphSignData> sign_;
  QVector<SV_Base::RecData> vars_;

  QVector<SV_Base::RecData> getSignData(QString sign);
  void listTimeUpdate(QString sname, int val);
  QVector<QPair<int, int>> calcHist(QString sname);

  // Mx Hist
  int Mx(QVector<QPair<int, int>>& hist);

  private slots:
  void dragEnterEvent(QDragEnterEvent *event);
  void dragMoveEvent(QDragMoveEvent *event);
  void dropEvent(QDropEvent *event);
  void graphValueChange();
  void selectSignalChange();
  void selectSignalTime(int row);

};


