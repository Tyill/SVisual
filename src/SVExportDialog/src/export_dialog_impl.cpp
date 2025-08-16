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

#include "SVExportDialog/forms/export_dialog_impl.h"
#include "SVBase/sv_limits.h"
#ifdef USE_QtXlsxWriter
#include "xlsx/xlsxdocument.h"
#endif

#include <QFileDialog>
#include <QTextStream>
#include <QTimer>
#include <QTranslator>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

using namespace SV_Base;

ExportDialog::ExportDialog(QWidget *parent, SV_Exp::Config cng_):
    QDialog(parent)
{

#ifdef SV_EN
  QTranslator translator;
  translator.load(":/SVExp/export_dialog_en.qm");
  QCoreApplication::installTranslator(&translator);
#endif

  cng = cng_;

  ui.setupUi(this);

  ui.dTimeBegin->setDateTime(QDateTime::currentDateTime().addSecs(-10));
  ui.dTimeEnd->setDateTime(QDateTime::currentDateTime());

  connect(ui.btnAddSignal, &QPushButton::clicked, [this]() {
    addSignalOnExport();
  });

  connect(ui.btnDelSignal, &QPushButton::clicked, [this]() {
    delSignalFromExport();
  });

  connect(ui.btnExport, &QPushButton::clicked, [this]() {

    QFileDialog dialog(this);

    QString selectedFilter;
    QString fileName = dialog.getSaveFileName(this,
      tr("Создание файла данных"), selDirMem_,
#ifdef USE_QtXlsxWriter
      tr("json file (*.json);; xlsx file (*.xlsx);; csv file (*.csv)"), &selectedFilter);
#else
      tr("json file (*.json);; csv file (*.csv)"), &selectedFilter);
#endif

    if (!fileName.isEmpty()) {
      if (selectedFilter == "json file (*.json)")
        exportToJSON(fileName);
#ifdef USE_QtXlsxWriter
      else if (selectedFilter == "xlsx file (*.xlsx)")
        exportToXLSX(fileName);
#endif
      else
        exportToCSV(fileName);
    }
  });

  connect(ui.tableModule, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selModule(QListWidgetItem*)));
}

ExportDialog::~ExportDialog() {


}

void ExportDialog::showEvent(QShowEvent * event) {

  auto mref = pfGetCopyModuleRef();

  ui.tableModule->clear();
  for (auto mod : mref) {
    if (!mod->isDelete && mod->isActive) ui.tableModule->addItem(mod->module.c_str());
  }
  ui.tableSignal->clearContents();

  if (ui.tableModule->count() > 0)
    selModule(ui.tableModule->item(0));
}

void ExportDialog::selModule(QListWidgetItem* item) {

  selModule_ = item->text();

  updateTableSignal();
}

void ExportDialog::addSignalOnExport() {

  int rows = ui.tableSignal->rowCount();

  for (int i = 0; i < rows; ++i) {

    auto item = ui.tableSignal->item(i, 0);
    if (!item) continue;
    if (item->isSelected()) {

      QString nm = item->text() + selModule_;
      if (!expSignals_.contains(nm)) {
        expSignals_.insert(nm);

        pfLoadSignalData(nm);
      }
    }
  }

  updateTableExport();
}

void ExportDialog::delSignalFromExport() {

  int rows = ui.tableExport->rowCount();

  for (int i = 0; i < rows; ++i) {

    auto item = ui.tableExport->item(i, 1);
    if (!item) continue;
    if (item->isSelected()) {

      QString nm = item->text() + selModule_;
      if (expSignals_.contains(nm))
        expSignals_.remove(nm);
    }
  }

  updateTableExport();
}

void ExportDialog::updateTableSignal() {

  ui.tableSignal->clearContents();
  auto sref = pfGetCopySignalRef();

  int row = 0, rowCnt = ui.tableSignal->rowCount();
  for (auto s : sref) {

    if ((selModule_ == s->module.c_str()) && !s->isDelete) {

      if (row >= rowCnt) {
        ui.tableSignal->insertRow(rowCnt);  ++rowCnt;
      }
      ui.tableSignal->setItem(row, 0, new QTableWidgetItem(s->name.c_str()));
      ui.tableSignal->setItem(row, 1, new QTableWidgetItem(getSVTypeStr(s->type).c_str()));
      ui.tableSignal->setItem(row, 2, new QTableWidgetItem(s->comment.c_str()));

      ++row;
    }
  }
  ui.tableSignal->update();
}

void ExportDialog::updateTableExport() {

  ui.tableExport->clearContents();
  auto sref = pfGetCopySignalRef();

  int row = 0, rowCnt = ui.tableExport->rowCount();
  for (auto s : sref) {

    if (expSignals_.contains((s->name + s->module).c_str())) {

      if (row >= rowCnt) {
        ui.tableExport->insertRow(rowCnt);  ++rowCnt;
      }
      ui.tableExport->setItem(row, 0, new QTableWidgetItem(s->module.c_str()));
      ui.tableExport->setItem(row, 1, new QTableWidgetItem(s->name.c_str()));
      ui.tableExport->setItem(row, 2, new QTableWidgetItem(getSVTypeStr(s->type).c_str()));
      ui.tableExport->setItem(row, 3, new QTableWidgetItem(s->comment.c_str()));

      ++row;
    }
  }
  ui.tableExport->update();
}
#ifdef USE_QtXlsxWriter
void ExportDialog::exportToXLSX(QString fileName) {

  QXlsx::Document xlsx;

  uint64_t beginTime = ui.dTimeBegin->dateTime().toMSecsSinceEpoch();
  uint64_t endTime = ui.dTimeEnd->dateTime().toMSecsSinceEpoch();

  QString module = "";
  int row = 1, col = 1;
  for (auto& s : expSignals_) {

    auto sd = pfGetSignalData(s);

    if (module != sd->module.c_str()) {
      module = sd->module.c_str();
      xlsx.write(1, col, sd->module.c_str()); ++col;
    }

    xlsx.write(1, col, sd->name.c_str()); ++col;

    row = 2;
    size_t bsz, cp, vp;
    {LockerReadSDataExp lock;
        bsz = sd->buffData.size();
        cp = sd->buffBeginPos;
        vp = sd->buffValuePos;
    }
    while (cp != vp) {

      uint64_t dt = sd->buffData[cp].beginTime;
      if (((beginTime < dt) && (dt < endTime)) || ui.chbAllTime->isChecked()) {

        if (ui.rbtnEveryVal->isChecked()) {

          xlsx.write(row, 1, QDateTime::fromMSecsSinceEpoch(dt));

          for (int j = 0; j < SV_PACKETSZ; ++j) {
            switch (sd->type) {
            case ValueType::BOOL:  xlsx.write(row, col - 1, sd->buffData[cp].vals[j].vBool ? 1 : 0); break;
            case ValueType::INT:   xlsx.write(row, col - 1, sd->buffData[cp].vals[j].vInt); break;
            case ValueType::FLOAT: xlsx.write(row, col - 1, sd->buffData[cp].vals[j].vFloat); break;
            }
            ++row;
          }
        }
        else if (ui.rbtnEveryMin->isChecked()) {
          int evr = 60000 / SV_CYCLESAVE_MS;

          if ((cp % evr) == 0) {

            xlsx.write(row, 1, QDateTime::fromMSecsSinceEpoch(dt));

            switch (sd->type) {
            case ValueType::BOOL:  xlsx.write(row, col - 1, sd->buffData[cp].vals[0].vBool ? 1 : 0); break;
            case ValueType::INT:   xlsx.write(row, col - 1, sd->buffData[cp].vals[0].vInt); break;
            case ValueType::FLOAT: xlsx.write(row, col - 1, sd->buffData[cp].vals[0].vFloat); break;
            }
            ++row;
          }
        }
        else if (ui.rbtnEverySec->isChecked()) {
          int evr = 1000 / SV_CYCLESAVE_MS;

          if ((cp % evr) == 0) {

            xlsx.write(row, 1, QDateTime::fromMSecsSinceEpoch(dt));

            switch (sd->type) {
            case ValueType::BOOL:  xlsx.write(row, col - 1, sd->buffData[cp].vals[0].vBool ? 1 : 0); break;
            case ValueType::INT:   xlsx.write(row, col - 1, sd->buffData[cp].vals[0].vInt); break;
            case ValueType::FLOAT: xlsx.write(row, col - 1, sd->buffData[cp].vals[0].vFloat); break;
            }
            ++row;
          }
        }
      }
      ++cp;
      if (cp >= bsz) cp = 0;
    }
  }
  if (!fileName.endsWith(".xlsx")){
    fileName += ".xlsx";
  }
  if (xlsx.saveAs(fileName))
    ui.lbMessage->setText(tr("Успешно сохранен: ") + fileName);
  else{
    ui.lbMessage->setText(tr("Не удалось сохранить: ") + fileName);
  }
  QTimer::singleShot(5000,[this]{
      ui.lbMessage->setText("");
  });
}
#endif
void ExportDialog::exportToCSV(QString fileName) {

  if (!fileName.endsWith(".csv")){
    fileName += ".csv";
  }
  QFile data(fileName);
  QTextStream out(&data);

  if (data.open(QFile::WriteOnly)) {

    uint64_t beginTime = ui.dTimeBegin->dateTime().toMSecsSinceEpoch();
    uint64_t endTime = ui.dTimeEnd->dateTime().toMSecsSinceEpoch();

    for (auto& s : expSignals_) {

      auto sd = pfGetSignalData(s);

      out << sd->module.c_str() << ';';
      out << sd->name.c_str() << ';';
            
      size_t bsz, cp, vp;
      {LockerReadSDataExp lock;
          bsz = sd->buffData.size();
          cp = sd->buffBeginPos;
          vp = sd->buffValuePos;
      }
      bool allTime = ui.chbAllTime->isChecked();
      bool everyValues = ui.rbtnEveryVal->isChecked();
      bool everyMin = ui.rbtnEveryMin->isChecked();
      bool everySec = ui.rbtnEverySec->isChecked();
      while (cp != vp) {
        uint64_t dt = sd->buffData[cp].beginTime;
        if (allTime || (beginTime < dt && dt < endTime)) {
          if (everyValues) {
            out << dt << ';';
            for (int j = 0; j < SV_PACKETSZ; ++j) {
              switch (sd->type) {
              case ValueType::BOOL:  out << sd->buffData[cp].vals[j].vBool << ';'; break;
              case ValueType::INT:   out << sd->buffData[cp].vals[j].vInt << ';'; break;
              case ValueType::FLOAT: out << sd->buffData[cp].vals[j].vFloat << ';'; break;
              }
            }
          }
          else if ((everyMin && cp % (60000 / SV_CYCLESAVE_MS) == 0) ||
                   (everySec && cp % (1000 / SV_CYCLESAVE_MS) == 0)){
              out << dt << ';';
              switch (sd->type) {
              case ValueType::BOOL:  out << sd->buffData[cp].vals[0].vBool << ';'; break;
              case ValueType::INT:   out << sd->buffData[cp].vals[0].vInt << ';'; break;
              case ValueType::FLOAT: out << sd->buffData[cp].vals[0].vFloat << ';'; break;
              }
          }
        }
        ++cp;
        if (cp >= bsz) cp = 0;
      }
      out << '\n';
    }
    data.close();
    ui.lbMessage->setText(tr("Успешно сохранен: ") + fileName);
  }
  else{
    ui.lbMessage->setText(tr("Не удалось создать файл: ") + fileName);
  }
  QTimer::singleShot(5000,[this]{
      ui.lbMessage->setText("");
  });
}

void ExportDialog::exportToJSON(QString fileName) {

  uint64_t beginTime = ui.dTimeBegin->dateTime().toMSecsSinceEpoch();
  uint64_t endTime = ui.dTimeEnd->dateTime().toMSecsSinceEpoch();

  QJsonArray jsSignalsArray;
  for (auto& s : expSignals_) {

    QJsonObject jsSignal;
    auto sd = pfGetSignalData(s);

    jsSignal["module"] = sd->module.c_str();
    jsSignal["signal"] = sd->name.c_str();
        
    size_t bsz, cp, vp;
    {LockerReadSDataExp lock;
        bsz = sd->buffData.size();
        cp = sd->buffBeginPos;
        vp = sd->buffValuePos;
    }
    bool allTime = ui.chbAllTime->isChecked();
    bool everyValues = ui.rbtnEveryVal->isChecked();
    bool everyMin = ui.rbtnEveryMin->isChecked();
    bool everySec = ui.rbtnEverySec->isChecked();
    QJsonArray jsSignalVals;
    while (cp != vp) {
      uint64_t dt = sd->buffData[cp].beginTime;
      if (allTime || (beginTime < dt && dt < endTime)) {
        if (everyValues) {
          QJsonObject jsValue;
          jsValue["time"] = double(dt);

          QJsonArray jsValues;
          for (int j = 0; j < SV_PACKETSZ; ++j) {
            switch (sd->type) {
            case ValueType::BOOL:  jsValues.append(sd->buffData[cp].vals[j].vBool); break;
            case ValueType::INT:   jsValues.append(sd->buffData[cp].vals[j].vInt); break;
            case ValueType::FLOAT: jsValues.append(sd->buffData[cp].vals[j].vFloat); break;
            }
          }
          jsValue["values"] = jsValues;
          jsSignalVals.append(jsValue);
        }
        else if ((everyMin && cp % (60000 / SV_CYCLESAVE_MS) == 0) ||
                 (everySec && cp % (1000 / SV_CYCLESAVE_MS) == 0)){
            QJsonObject jsValue;
            jsValue["time"] = double(dt);

            QJsonArray jsValues;
            switch (sd->type) {
            case ValueType::BOOL:  jsValues.append(sd->buffData[cp].vals[0].vBool); break;
            case ValueType::INT:   jsValues.append(sd->buffData[cp].vals[0].vInt); break;
            case ValueType::FLOAT: jsValues.append(sd->buffData[cp].vals[0].vFloat); break;
            }
            jsValue["values"] = jsValues;
            jsSignalVals.append(jsValue);
        }
      }
      jsSignal["values"] = jsSignalVals;
      ++cp;
      if (cp >= bsz) cp = 0;
    }
    jsSignalsArray.append(jsSignal);
  }

  if (!fileName.endsWith(".json")){
    fileName += ".json";
  }
  QFile data(fileName);
  if (data.open(QFile::WriteOnly)) {
    data.write(QJsonDocument(jsSignalsArray).toJson(QJsonDocument::Compact));
    data.close();
    ui.lbMessage->setText(tr("Успешно сохранен: ") + fileName);
  }
  else{
    ui.lbMessage->setText(tr("Не удалось сохранить: ") + fileName);
  }
  QTimer::singleShot(5000,[this]{
      ui.lbMessage->setText("");
  });
}


