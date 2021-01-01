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

#include "SVStatDialog/forms/stat_dialog_impl.h"
#include "SVConfig/config_limits.h"

#include <QtGui>
#include <QTreeWidget>

using namespace SV_Base;

StatDialog::StatDialog(QWidget *parent, SV_Stat::Config cng_) {

  setParent(parent);

#ifdef SV_EN
  QTranslator translator;
  translator.load(":/SVStat/stat_dialog_en.qm");
  QCoreApplication::installTranslator(&translator);
#endif

  setAcceptDrops(true);

  cng = cng_;

  ui.setupUi(this);

  ui.txtDiapMin->setEnabled(false);
  ui.txtDiapMax->setEnabled(false);

  connect(ui.chbDiapEna, &QCheckBox::stateChanged, [this]() {
    if (ui.chbDiapEna->isChecked()) {
      diapEna_ = true;
      ui.txtDiapMin->setEnabled(true);
      ui.txtDiapMax->setEnabled(true);
      selectSignalChange();
    }
    else {
      diapEna_ = false;
      ui.txtDiapMin->setEnabled(false);
      ui.txtDiapMax->setEnabled(false);
      selectSignalChange();
    }
  });

  connect(ui.chbWinDiap, SIGNAL(stateChanged(int)), SLOT(selectSignalChange()));
  connect(ui.chbFront, SIGNAL(stateChanged(int)), SLOT(selectSignalChange()));

  connect(ui.hist, SIGNAL(selValueChange()), this, SLOT(graphValueChange()));
  connect(ui.cmbSignals, SIGNAL(currentIndexChanged(int)), this, SLOT(selectSignalChange()));
  connect(ui.listSignTimeChange, SIGNAL(currentRowChanged(int)), this, SLOT(selectSignalTime(int)));
  connect(ui.txtDiapMin, SIGNAL(textChanged(QString)), this, SLOT(selectSignalChange()));
  connect(ui.txtDiapMax, SIGNAL(textChanged(QString)), this, SLOT(selectSignalChange()));
  connect(ui.txtValue, SIGNAL(textChanged(QString)), ui.hist, SLOT(setTargPos(QString)));

}

StatDialog::~StatDialog() {


}

void StatDialog::selectSignalChange() {

  QString sign = ui.cmbSignals->currentData().toString();

  if (sign.isEmpty()) return;

  sign_[sign].hist = calcHist(sign);

  ui.hist->setGraphPnt(sign_[sign].hist);

  graphValueChange();
}

QVector<RecData> StatDialog::getSignData(QString sname) {

  SignalData* sdata = pfGetSignalData(sname);

  if (!ui.chbWinDiap->isChecked()) {

    int sz = (int)sdata->buffData.size();
    QVector<RecData> res(sz);
    for (int i = 0; i < sz; ++i)
      res[i] = sdata->buffData[i];

    return res;
  }

  QPair<qint64, qint64> tmInterval = pfGetTimeInterval ? pfGetTimeInterval() : QPair<qint64, qint64>();

  size_t znSz = sdata->buffData.size();

  uint64_t tmZnBegin = sdata->buffMinTime,
    tmZnEnd = sdata->buffMaxTime,
    tmMinInterval = tmInterval.first,
    tmMaxInterval = tmInterval.second;

  if ((tmZnBegin >= tmMaxInterval) || (tmZnEnd <= tmMinInterval)) return QVector<RecData>();


  QVector<RecData> res;

  tmZnEnd = sdata->buffData[0].beginTime + SV_CYCLESAVE_MS;
  uint64_t tmZnEndPrev = sdata->buffData[0].beginTime;

  size_t z = 0;
  while (tmZnBegin < tmMaxInterval) {

    if (tmZnEnd > tmMinInterval) {
      res.append(sdata->buffData[z]);
    }

    ++z;
    if (z < znSz) {
      tmZnBegin = sdata->buffData[z].beginTime;
      tmZnEnd = sdata->buffData[z].beginTime + SV_CYCLESAVE_MS;
    }
    else break;
  }

  return res;
}

void StatDialog::dragEnterEvent(QDragEnterEvent *event)
{
  if (qobject_cast<QTreeWidget *>(event->source()) ||
    qobject_cast<QLabel *>(event->source())) {

    event->accept();

  }
}

void StatDialog::dragMoveEvent(QDragMoveEvent *event) {

  if (qobject_cast<QTreeWidget *>(event->source()) ||
    qobject_cast<QLabel *>(event->source())) {

    event->accept();

  }
}

void StatDialog::dropEvent(QDropEvent *event)
{
  QLabel* lb = qobject_cast<QLabel*>(event->source());

  if (qobject_cast<QTreeWidget *>(event->source()) || lb) {

    QString sname = event->mimeData()->text();

    if (!sname.isEmpty()) {

      pfLoadSignalData(sname);

      SignalData* sdata = pfGetSignalData(sname);

      if (!sign_.contains(sname)) {

        sign_.insert(sname, graphSignData{ sname, sdata->name.c_str() });
      }

      ui.cmbSignals->addItem(sdata->name.c_str());

      ui.cmbSignals->setItemData(ui.cmbSignals->count() - 1, sname);

      ui.cmbSignals->setCurrentIndex(ui.cmbSignals->count() - 1);

      if (ui.cmbSignals->count() == 1) selectSignalChange();
    }

    event->accept();
  }
}

QVector<QPair<int, int>> StatDialog::calcHist(QString sname) {

  vars_ = getSignData(sname);

  QMap<int, int> hist;
  int bDuration = 0, prevVal = 0;

  if (diapEna_) {

    int minVal = ui.txtDiapMin->text().toInt();
    int maxVal = ui.txtDiapMax->text().toInt();
    int sz = vars_.size();
    bool isOnlyFront = ui.chbFront->isChecked();

    SignalData* sdata = pfGetSignalData(sname);

    switch (sdata->type)
    {
    case ValueType::BOOL: {
      bool prev = false; hist.insert(0, 0); sign_[sname].valData.insert(0, valSData{ 0, 0 });
      for (int i = 0; i < sz; ++i) {

        for (int j = 0; j < SV_PACKETSZ; ++j) {

          bool val = vars_[i].vals[j].vBool;

          if (val) {
            if (!isOnlyFront || (val != prev)) hist[0]++;
            if (val != prev) sign_[sname].valData[0].changeCnt++;

            if (val) sign_[sname].valData[0].duration++;
          }
          prev = val;
        }
      }
    }
                          break;

    case ValueType::INT:

      for (int i = 0; i < sz; ++i) {

        for (int j = 0; j < SV_PACKETSZ; ++j) {

          int val = vars_[i].vals[j].vInt;

          if ((minVal <= val) && (val <= maxVal)) {

            if (!hist.contains(val)) {
              hist.insert(val, 1);
              sign_[sname].valData.insert(val, valSData{ 1, 1 });
            }
            else {
              if (!isOnlyFront || (val != prevVal)) hist[val]++;
              if (val != prevVal) sign_[sname].valData[val].changeCnt++;
              sign_[sname].valData[val].duration++;
            }

            prevVal = val;
          }
        }
      }

      break;
    case ValueType::FLOAT:

      for (int i = 0; i < sz; ++i) {

        for (int j = 0; j < SV_PACKETSZ; ++j) {

          int val = vars_[i].vals[j].vFloat > 0 ? vars_[i].vals[j].vFloat + 0.5 : vars_[i].vals[j].vFloat - 0.5;

          if ((minVal <= val) && (val <= maxVal)) {

            if (!hist.contains(val)) {
              hist.insert(val, 1);
              sign_[sname].valData.insert(val, valSData{ 1, 1 });
            }
            else {
              if (!isOnlyFront || (val != prevVal)) hist[val]++;
              if (val != prevVal) sign_[sname].valData[val].changeCnt++;
              sign_[sname].valData[val].duration++;
            }

            prevVal = val;
          }
        }
      }
      break;
    default:
      break;
    }

  }
  else {

    int sz = vars_.size();
    bool isOnlyFront = ui.chbFront->isChecked();
    int prevVal = 0;
    SignalData* sdata = pfGetSignalData(sname);

    switch (sdata->type)
    {
    case ValueType::BOOL: {
      bool prev = false; hist.insert(0, 0); sign_[sname].valData.insert(0, valSData{ 0, 0 });
      for (int i = 0; i < sz; ++i) {

        for (int j = 0; j < SV_PACKETSZ; ++j) {

          bool val = vars_[i].vals[j].vBool;

          if (val) {
            if (!isOnlyFront || (val != prev)) hist[0]++;
            if (val != prev) sign_[sname].valData[0].changeCnt++;

            if (val) sign_[sname].valData[0].duration++;
          }
          prev = val;
        }
      }
    }
                          break;

    case ValueType::INT:
      for (int i = 0; i < sz; ++i) {

        for (int j = 0; j < SV_PACKETSZ; ++j) {

          int val = vars_[i].vals[j].vInt;

          if (!hist.contains(val)) {
            hist.insert(val, 1);
            sign_[sname].valData.insert(val, valSData{ 1, 1 });
          }
          else {
            if (!isOnlyFront || (val != prevVal)) hist[val]++;
            if (val != prevVal) sign_[sname].valData[val].changeCnt++;
            sign_[sname].valData[val].duration++;
          }

          prevVal = val;
        }
      }
      break;
    case ValueType::FLOAT:

      for (int i = 0; i < sz; ++i) {

        for (int j = 0; j < SV_PACKETSZ; ++j) {

          int val = vars_[i].vals[j].vFloat > 0 ? vars_[i].vals[j].vFloat + 0.5 : vars_[i].vals[j].vFloat - 0.5;

          if (!hist.contains(val)) {
            hist.insert(val, 1);
            sign_[sname].valData.insert(val, valSData{ 1, 1 });
          }
          else {
            if (!isOnlyFront || (val != prevVal)) hist[val]++;
            if (val != prevVal) sign_[sname].valData[val].changeCnt++;
            sign_[sname].valData[val].duration++;
          }

          prevVal = val;
        }
      }
      break;
    default:
      break;
    }
  }

  QVector<QPair<int, int>> res;
  QList<int> keys = hist.keys(); int sz = keys.size();
  for (int i = 0; i < sz; ++i) {

    res.append(QPair<int, int>(keys[i], hist[keys[i]]));

    sign_[sname].valData[keys[i]].duration *= SV_CYCLEREC_MS;

    sign_[sname].valData[keys[i]].durationMean = (double)sign_[sname].valData[keys[i]].duration / sign_[sname].valData[keys[i]].changeCnt + 0.5;
  }

  return res;
}

void StatDialog::listTimeUpdate(QString sname, int selVal) {

  ui.listSignTimeChange->clear();

  SignalData* sdata = pfGetSignalData(sname);

  if (sign_[sname].valData.contains(selVal)) {
    int duration = sign_[sname].valData[selVal].duration / 1000.0 + 0.5;
    ui.txtDurationFull->setText(QString::number(duration));
    ui.txtDurationMean->setText(QString::number(sign_[sname].valData[selVal].durationMean));

  }
  if (sdata->type == ValueType::BOOL)
    ui.txtDurationMean->setText(QString::number(sign_[sname].valData[0].durationMean));

  if (sname.isEmpty() || (!ui.chbFront->isChecked() && (sdata->type != ValueType::BOOL))) return;

  int sz = vars_.size(); QStringList itTimes; int prev = 0;
  switch (sdata->type)
  {
  case ValueType::BOOL:

    for (int i = 0; i < sz; ++i) {

      for (int j = 0; j < SV_PACKETSZ; ++j) {

        int val = vars_[i].vals[j].vBool;

        if (val && (val != prev)) {
          itTimes.append(QDateTime::fromMSecsSinceEpoch(vars_[i].beginTime + j * SV_CYCLEREC_MS).toString("yyyy.MM.dd hh:mm:ss:zzz"));
        }
        prev = val;
      }
    }
    break;

  case ValueType::INT:

    for (int i = 0; i < sz; ++i) {

      for (int j = 0; j < SV_PACKETSZ; ++j) {

        int val = vars_[i].vals[j].vInt;

        if ((val != prev) && (val == selVal)) {
          itTimes.append(QDateTime::fromMSecsSinceEpoch(vars_[i].beginTime + j * SV_CYCLEREC_MS).toString("yyyy.MM.dd hh:mm:ss:zzz"));
        }
        prev = val;
      }
    }
    break;
  case ValueType::FLOAT:

    for (int i = 0; i < sz; ++i) {

      for (int j = 0; j < SV_PACKETSZ; ++j) {

        int val = vars_[i].vals[j].vFloat;

        if ((val != prev) && (val == selVal)) {
          itTimes.append(QDateTime::fromMSecsSinceEpoch(vars_[i].beginTime + j * SV_CYCLEREC_MS).toString("yyyy.MM.dd hh:mm:ss:zzz"));
        }
        prev = val;
      }
    }
    break;
  default:
    break;
  }
  ui.listSignTimeChange->addItems(itTimes);

  ui.txtHistValue->setText(QString::number(itTimes.size()));
}

void StatDialog::graphValueChange() {

  ui.txtCurrArea->setText(QString::number(ui.hist->getAreaByPos()));

  QPoint vp = ui.hist->getTargPos();

  ui.txtValue->setText(QString::number(vp.x()));

  ui.txtHistValue->setText(QString::number(vp.y()));

  QString sname = ui.cmbSignals->currentData().toString();

  int mx = Mx(sign_[sname].hist);
  ui.txtMx->setText(QString::number(mx));

  listTimeUpdate(sname, vp.x());
}

void StatDialog::selectSignalTime(int row) {

  QListWidgetItem* item = ui.listSignTimeChange->currentItem();

  if (!item) return;

  QDateTime dt = QDateTime::fromString(item->text(), "yyyy.MM.dd hh:mm:ss:zzz");

  if (pfSetTimeInterval)
    pfSetTimeInterval(dt.addSecs(-1).toMSecsSinceEpoch(), dt.addSecs(1).toMSecsSinceEpoch());

}

// Mx Hist
int StatDialog::Mx(QVector<QPair<int, int>>& hist)
{
  int64_t Square = 0;
  int64_t Mx = 0;

  int sz = hist.size();
  for (int i = 0; i < sz; i++)
  {
    Mx += hist[i].first * hist[i].second;
    Square += hist[i].second;
  }

  if (Square > 0) return (double)Mx / Square + 0.5;

  return 0;
}

