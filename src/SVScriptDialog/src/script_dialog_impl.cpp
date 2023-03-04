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

#include <thread>

#include "SVScriptDialog/forms/script_dialog_impl.h"
#include "SVBase/limits.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "SVMisc/timer_delay.h"
#include "SVMisc/front.h"
#include "SVMisc/misc.h"

#include <QtGui>
#include <QMessageBox>
#include <QMenu>

using namespace SV_Base;
namespace lub = luabridge;

extern ScriptDialog* scrDialogRef;

void printMess(const std::string& mess) {

  if ((scrDialogRef->buffCPos_ == 0) && (scrDialogRef->iterValue_ == 0)) {
    QString qmess = QString::fromStdString(SV_Misc::currDateTime()) + " " + mess.c_str();
    QMetaObject::invokeMethod(scrDialogRef->ui.txtStatusMess, "append", Qt::AutoConnection, Q_ARG(QString, qmess));
  }
}

uint64_t getCTimeMS() {

  return SV_Misc::currDateTimeSinceEpochMs();
}

void changeSignColor(const std::string& module, const std::string& signal, const std::string& color) {

  QString md = qUtf8Printable(module.c_str()),
    sn = qUtf8Printable(signal.c_str());

  if (scrDialogRef->pfChangeSignColor) {
    QColor clr;
    clr.setNamedColor(QString::fromStdString(color));
    scrDialogRef->pfChangeSignColor(md, sn, clr);
  }
}

uint64_t getTimeValue(const std::string& module, const std::string& signal) {

  QString md = qUtf8Printable(module.c_str()),
    sn = qUtf8Printable(signal.c_str()),
    sign = sn + md;
  if (scrDialogRef->updateBuffValue(md, sn, SV_Base::ValueType::BOOL)) {

    if (scrDialogRef->mode_ == SV_Script::ModeGr::player)
      return scrDialogRef->signBuff_[sign]->lastData.beginTime;
    else {
      if ((scrDialogRef->buffCPos_ == 0) && (scrDialogRef->iterValue_ == 0))
        scrDialogRef->buffSz_ = qMax(scrDialogRef->buffSz_, scrDialogRef->signBuff_[sign]->buffData.size());

      size_t inx = qMin(scrDialogRef->buffCPos_, scrDialogRef->signBuff_[sign]->buffData.size() - 1);

      return scrDialogRef->signBuff_[sign]->buffData[inx].beginTime;
    }
  }
  else
    return 0;
}

bool getBoolValue(const std::string& module, const std::string& signal) {

  QString md = qUtf8Printable(module.c_str()),
    sn = qUtf8Printable(signal.c_str()),
    sign = sn + md;
  if (scrDialogRef->updateBuffValue(md, sn, SV_Base::ValueType::BOOL)) {

    if (scrDialogRef->mode_ == SV_Script::ModeGr::player)
      return scrDialogRef->signBuff_[sign]->lastData.vals[scrDialogRef->iterValue_].vBool;
    else {
      if ((scrDialogRef->buffCPos_ == 0) && (scrDialogRef->iterValue_ == 0))
        scrDialogRef->buffSz_ = qMax(scrDialogRef->buffSz_, scrDialogRef->signBuff_[sign]->buffData.size());

      size_t inx = qMin(scrDialogRef->buffCPos_, scrDialogRef->signBuff_[sign]->buffData.size() - 1);

      return scrDialogRef->signBuff_[sign]->buffData[inx].vals[scrDialogRef->iterValue_].vBool;
    }
  }
  else
    return false;
}

int getIntValue(const std::string& module, const std::string& signal) {

  QString md = qUtf8Printable(module.c_str()),
    sn = qUtf8Printable(signal.c_str()),
    sign = sn + md;
  if (scrDialogRef->updateBuffValue(md, sn, SV_Base::ValueType::INT)) {

    if (scrDialogRef->mode_ == SV_Script::ModeGr::player)
      return scrDialogRef->signBuff_[sign]->lastData.vals[scrDialogRef->iterValue_].vInt;
    else {
      if ((scrDialogRef->buffCPos_ == 0) && (scrDialogRef->iterValue_ == 0))
        scrDialogRef->buffSz_ = qMax(scrDialogRef->buffSz_, scrDialogRef->signBuff_[sign]->buffData.size());

      size_t inx = qMin(scrDialogRef->buffCPos_, scrDialogRef->signBuff_[sign]->buffData.size() - 1);

      return scrDialogRef->signBuff_[sign]->buffData[inx].vals[scrDialogRef->iterValue_].vInt;
    }
  }
  else
    return 0;
}

float getFloatValue(const std::string& module, const std::string& signal) {

  QString md = qUtf8Printable(module.c_str()),
    sn = qUtf8Printable(signal.c_str()),
    sign = sn + md;
  if (scrDialogRef->updateBuffValue(md, sn, SV_Base::ValueType::FLOAT)) {

    if (scrDialogRef->mode_ == SV_Script::ModeGr::player)
      return scrDialogRef->signBuff_[sign]->lastData.vals[scrDialogRef->iterValue_].vFloat;
    else {
      if ((scrDialogRef->buffCPos_ == 0) && (scrDialogRef->iterValue_ == 0))
        scrDialogRef->buffSz_ = qMax(scrDialogRef->buffSz_, scrDialogRef->signBuff_[sign]->buffData.size());

      size_t inx = qMin(scrDialogRef->buffCPos_, scrDialogRef->signBuff_[sign]->buffData.size() - 1);

      return scrDialogRef->signBuff_[sign]->buffData[inx].vals[scrDialogRef->iterValue_].vFloat;
    }
  }
  else
    return 0.F;
}

void setBoolValue(const std::string& signal, bool bval, uint64_t time) {

  QString md = "Virtual",
    sn = qUtf8Printable(signal.c_str()),
    sign = sn + md;

  SV_Base::Value val;
  val.vBool = bval;

  if (scrDialogRef->updateBuffValue(md, sn, SV_Base::ValueType::BOOL))
    scrDialogRef->setValue(sign, val, time);
}

void setIntValue(const std::string& signal, int ival, uint64_t time) {

  QString md = "Virtual",
    sn = qUtf8Printable(signal.c_str()),
    sign = sn + md;

  SV_Base::Value val;
  val.vInt = ival;

  if (scrDialogRef->updateBuffValue(md, sn, SV_Base::ValueType::INT))
    scrDialogRef->setValue(sign, val, time);
}

void setFloatValue(const std::string& signal, float fval, uint64_t time) {

  QString md = "Virtual",
    sn = qUtf8Printable(signal.c_str()),
    sign = sn + md;

  SV_Base::Value val;
  val.vFloat = fval;

  if (scrDialogRef->updateBuffValue(md, sn, SV_Base::ValueType::FLOAT))
    scrDialogRef->setValue(sign, val, time);
}

void ScriptDialog::setValue(const QString& sign, SV_Base::Value val, uint64_t time) {

  auto sd = signBuff_[sign];

  sd->lastData.vals[iterValue_] = val;

  // заполняем буфер
  size_t vp = (mode_ == SV_Script::ModeGr::player) ? sd->buffValuePos : buffCPos_;

  sd->buffData[vp].vals[iterValue_] = val;

  if (iterValue_ == (SV_PACKETSZ - 1)) {

    sd->lastData.beginTime = time;
    sd->buffData[vp].beginTime = time;

    updateSign(sd, sd->buffBeginPos, vp);

    ++vp;

    if (mode_ == SV_Script::ModeGr::player) {
      size_t buffSz = 2 * 3600000 / SV_CYCLESAVE_MS; // 2 часа жестко

      if (vp == buffSz) vp = 0;

      sd->buffValuePos = vp;

      if (vp == sd->buffBeginPos) {
        ++sd->buffBeginPos;
        if (sd->buffBeginPos == buffSz) sd->buffBeginPos = 0;
      }
    }
    else {

      if (vp == buffSz_)
        vp = buffSz_ - 1;

      sd->buffValuePos = vp;

      size_t csz = sd->buffData.size();
      if (csz < buffSz_) {

        sd->buffData.resize(buffSz_);

        SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * (buffSz_ - csz)];
        for (size_t i = 0; i < (buffSz_ - csz); ++i)
          sd->buffData[i + csz].vals = &buff[i * SV_PACKETSZ];
      }
    }
  }
}

void ScriptDialog::updateSign(SignalData* sign, size_t beginPos, size_t valuePos) {

  sign->buffMinTime = sign->buffData[beginPos].beginTime;
  sign->buffMaxTime = sign->buffData[valuePos].beginTime + SV_CYCLESAVE_MS;

  double minValue = sign->buffMinValue, maxValue = sign->buffMaxValue;

  if (sign->type == ValueType::INT) {

    Value* vl = sign->buffData[valuePos].vals;

    for (int i = 0; i < SV_PACKETSZ; ++i) {

      if (vl[i].vInt > maxValue) maxValue = vl[i].vInt;
      if (vl[i].vInt < minValue) minValue = vl[i].vInt;
    }

  }
  else if (sign->type == ValueType::FLOAT) {

    Value* vl = sign->buffData[valuePos].vals;
    for (int i = 0; i < SV_PACKETSZ; ++i) {

      if (vl[i].vFloat > maxValue) maxValue = vl[i].vFloat;
      if (vl[i].vFloat < minValue) minValue = vl[i].vFloat;
    }
  }

  sign->buffMinValue = minValue;
  sign->buffMaxValue = maxValue;

}

bool ScriptDialog::updateBuffValue(const QString& module, const QString& sname, SV_Base::ValueType stype) {

  QString sign = sname + module;

  if (signBuff_.contains(sign)) {
    return true;
  }

  if (module == "Virtual") {

    if (!pfAddSignal || !pfAddModule || !pfGetModuleData || !pfLoadSignalData)
      return false;

    SignalData* sd = new SignalData();
    signBuff_[sign] = sd;

    sd->isActive = true;
    sd->isBuffEnable = false;
    sd->isDelete = false;

    sd->name = qUtf8Printable(sname);
    sd->module = "Virtual";
    sd->type = stype;

    sd->lastData.vals = new SV_Base::Value[SV_PACKETSZ];
    sd->lastData.beginTime = SV_Misc::currDateTimeSinceEpochMs();
    memset(sd->lastData.vals, 0, sizeof(SV_Base::Value) * SV_PACKETSZ);

    sd->buffMinTime = sd->lastData.beginTime - 5000;
    sd->buffMaxTime = sd->lastData.beginTime + 5000;
    sd->buffMaxValue = 1;
    sd->buffMinValue = 0;

    auto md = pfGetModuleData("Virtual");
    if (!md) {
      md = new SV_Base::ModuleData("Virtual");
      md->isActive = true;
      md->isDelete = false;
      md->isEnable = true;
      pfAddModule(md);

      if (pfModuleConnectCBack)
        pfModuleConnectCBack("Virtual");
    }

    pfAddSignal(sd);

    if (mode_ == SV_Script::ModeGr::player) {
      pfLoadSignalData(sign);
    }
    else {
      sd->buffData.resize(buffSz_);

      SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * buffSz_];
      for (size_t i = 0; i < buffSz_; ++i)
        sd->buffData[i].vals = &buff[i * SV_PACKETSZ];
    }

    if (pfAddSignalsCBack) {
      pfAddSignalsCBack();
    }
    return true;
  }
  else {
    if (isStopWork_) {
      return false;
    }

    auto sdata = pfGetSignalData(sign);

    if (sdata && pfLoadSignalData && pfLoadSignalData(sign)) {
      signBuff_[sign] = sdata;
      return true;
    }
    return false;
  }
}

/////////////////////////////////

ScriptDialog::ScriptDialog(QWidget *parent, SV_Script::Config cng_, SV_Script::ModeGr mode) {

  setParent(parent);

#ifdef SV_EN
  QTranslator translator;
  translator.load(":/SVScript/script_dialog_en.qm");
  QCoreApplication::installTranslator(&translator);
#endif

  mode_ = mode;
  cng = cng_;

  ui.setupUi(this);

  scrDialogRef = this;

  connect(ui.btnNewScript, SIGNAL(clicked()), SLOT(addScript()));
  connect(ui.btnSave, SIGNAL(clicked()), SLOT(saveScript()));
  connect(ui.btnSetActive, &QPushButton::clicked, [this]() {
    std::unique_lock<std::mutex> lck(mtx_);

    auto items = ui.tblScripts->selectedItems();
    for (auto it : items) {
      QString sname = it->text();
      auto sts = std::find_if(scrState_.begin(), scrState_.end(),
        [sname](const scriptState& st) {
        return st.name == sname;
      }
      );

      if (!sts->isActive) {
        int rowCnt = ui.tblActiveScripts->rowCount();
        ui.tblActiveScripts->insertRow(rowCnt);
        auto itm = new QTableWidgetItem(sname);
        itm->setFlags(itm->flags() ^ Qt::ItemFlag::ItemIsEditable);
        ui.tblActiveScripts->setItem(rowCnt, 0, itm);

        sts->isActive = true;
        buffCPos_ = 0;
      }
    }
  });
  connect(ui.btnResetActive, &QPushButton::clicked, [this]() {

    auto items = ui.tblActiveScripts->selectedItems();
    for (auto it : items) {
      QString sname = it->text();
      std::find_if(scrState_.begin(), scrState_.end(),
        [sname](const scriptState& st) {
        return st.name == sname;
      }
      )->isActive = false;

      ui.tblActiveScripts->removeRow(it->row());
    }
  });
  connect(ui.btnClear, &QPushButton::clicked, [this]() {

    ui.txtStatusMess->clear();
  });
  connect(ui.tblScripts, SIGNAL(cellChanged(int, int)), SLOT(nameScriptChange(int, int)));
  connect(ui.tabWidget, &QTabWidget::tabCloseRequested, [this](int inx) {

    QString sname = ui.tabWidget->tabText(inx);
    auto it = std::find_if(scrState_.begin(), scrState_.end(),

      [sname](const scriptState& st) {

      return st.name == sname;
    });

    if (it->isChange) {
      QMessageBox msgBox;
      msgBox.setText(sname + tr(" изменен."));
      msgBox.setInformativeText(tr("Сохранить изменения?"));
      msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
      msgBox.setDefaultButton(QMessageBox::Save);

      if (msgBox.exec() == QMessageBox::Save)
        saveScript();
    }

    it->isChange = false;

    ui.tabWidget->removeTab(inx);

    for (int i = 0; i < scrState_.size(); ++i) {

      if (scrState_[i].tabInx == inx)
        scrState_[i].tabInx = -1;
      else if (scrState_[i].tabInx > inx)
        --scrState_[i].tabInx;
    }

    if ((inx - 1) >= 0) {
      QString sname = ui.tabWidget->tabText(inx - 1);
      auto it = std::find_if(scrState_.begin(), scrState_.end(),

        [sname](const scriptState& st) {

        return st.name == sname;
      });

      ui.lbChange->setText(it->isChange ? "*" : "");
    }
  });
  connect(ui.tabWidget, &QTabWidget::tabBarClicked, [this](int inx) {

    QString sname = ui.tabWidget->tabText(inx);
    auto it = std::find_if(scrState_.begin(), scrState_.end(),

      [sname](const scriptState& st) {

      return st.name == sname;
    });

    ui.lbChange->setText(it->isChange ? "*" : "");
  });
  connect(ui.tblScripts, &QTableWidget::itemDoubleClicked, [this](QTableWidgetItem* item) {

    int row = item->row();
    QString scrName = ui.tblScripts->item(row, 0)->text();

    int sz = ui.tabWidget->count();
    for (int i = 0; i < sz; ++i) {
      if (ui.tabWidget->tabText(i) == scrName)
        return;
    }

    scrState_[row].tabInx = ui.tabWidget->count();

    auto te = new QTextEdit();
    te->setFrameShape(QFrame::NoFrame);

    te->setText(scrState_[row].text);

    connect(te, &QTextEdit::textChanged, [this, row]() {
      scrState_[row].isChange = true;
      ui.lbChange->setText("*");
    });

    ui.tabWidget->addTab(te, scrName);

    ui.tabWidget->setCurrentIndex(scrState_[row].tabInx);

    ui.lbChange->setText(scrState_[row].isChange ? "*" : "");
  });
  connect(ui.tblActiveScripts, &QTableWidget::itemDoubleClicked, [this](QTableWidgetItem* item) {

    int row = item->row();
    QString scrName = ui.tblActiveScripts->item(row, 0)->text();

    int sz = ui.tabWidget->count();
    for (int i = 0; i < sz; ++i) {
      if (ui.tabWidget->tabText(i) == scrName)
        return;
    }

    for (int i = 0; i < scrState_.size(); ++i) {
      if (scrState_[i].name == scrName) {
        row = i;
        break;
      }
    }

    scrState_[row].tabInx = ui.tabWidget->count();

    auto te = new QTextEdit();
    te->setFrameShape(QFrame::NoFrame);

    te->setText(scrState_[row].text);

    connect(te, &QTextEdit::textChanged, [this, row]() {
      scrState_[row].isChange = true;
      ui.lbChange->setText("*");
    });

    ui.tabWidget->addTab(te, scrName);

    ui.tabWidget->setCurrentIndex(scrState_[row].tabInx);

    ui.lbChange->setText(scrState_[row].isChange ? "*" : "");
  });

  QDir dir(QApplication::applicationDirPath() + "/scripts/");

  if (!dir.exists())
    dir.mkdir(dir.absolutePath());

  for (auto& f : dir.entryList(QDir::Files)) {

    QString name = f.right(f.size() - f.lastIndexOf('/') - 1);

    addScript(name);
  }

  QList<int> ss; ss.append(150); ss.append(500);
  ui.splitter->setSizes(ss);

  ss[0] = 300; ss[1] = 100;
  ui.splitter_2->setSizes(ss);

  ui.tblScripts->setColumnWidth(0, 150);
  ui.tblActiveScripts->setColumnWidth(0, 150);

}

ScriptDialog::~ScriptDialog() {

  isStopWork_ = true;
  if (workThr_.joinable()) workThr_.join();
}

void ScriptDialog::contextMenuEvent(QContextMenuEvent * event) {

  if (!qobject_cast<QTableWidget*>(focusWidget()) ||
    (qobject_cast<QTableWidget*>(focusWidget())->objectName() != "tblScripts")) return;

  QTableWidgetItem* item = ui.tblScripts->currentItem();
  if (!item) return;

  QMenu* menu = new QMenu(this);

  menu->addAction(tr("Удалить"));

  connect(menu, &QMenu::triggered, [this, item](QAction*) {
    std::unique_lock<std::mutex> lck(mtx_);

    int row = item->row();
    QString scrName = ui.tblScripts->item(row, 0)->text();

    if (scrState_[row].tabInx >= 0) {
      ui.tabWidget->removeTab(scrState_[row].tabInx);

      for (int i = 0; i < scrState_.size(); ++i)
        if (scrState_[i].tabInx > scrState_[row].tabInx)
          --scrState_[i].tabInx;
    }
    scrState_.remove(row);
    ui.tblScripts->removeRow(row);

    int sz = ui.tblActiveScripts->rowCount();
    for (int i = 0; i < sz; ++i) {
      if (ui.tblActiveScripts->item(i, 0)->text() == scrName) {
        ui.tblActiveScripts->removeRow(i);
        break;
      }
    }

    QFile::remove(QApplication::applicationDirPath() + "/scripts/" + scrName);
  });

  menu->exec(event->globalPos());

}

void ScriptDialog::startUpdateThread() {

  if (workThr_.joinable()) return;

  luaState_ = luaL_newstate();
  luaL_openlibs(luaState_);

  lub::getGlobalNamespace(luaState_)
    .addFunction("printMess", printMess)
    .addFunction("getTimeValue", getTimeValue)
    .addFunction("getBoolValue", getBoolValue)
    .addFunction("getIntValue", getIntValue)
    .addFunction("getFloatValue", getFloatValue)
    .addFunction("setBoolValue", setBoolValue)
    .addFunction("setIntValue", setIntValue)
    .addFunction("setFloatValue", setFloatValue)
    .addFunction("getCTimeMS", getCTimeMS)
    .addFunction("changeSignColor", changeSignColor);

  workThr_ = std::thread([](ScriptDialog* sp) { sp->workCycle(); }, this);
}

void ScriptDialog::addScript(QString name) {

  std::unique_lock<std::mutex> lck(mtx_);

  bool isNew = name.isEmpty();
  if (isNew)
    name = "NewScript.lua";

  name = exlName(name);

  int rowCnt = ui.tblScripts->rowCount();
  ui.tblScripts->insertRow(rowCnt);

  ui.tblScripts->setItem(rowCnt, 0, new QTableWidgetItem(name));

  int nameMaxWidth = 150;
  for (int i = 0; i < (rowCnt + 1); ++i) {

    int nameFontMetr = int(this->fontMetrics().width(ui.tblScripts->item(i, 0)->text()) * 1.5);

    nameMaxWidth = qMax(nameFontMetr, nameMaxWidth);
  }

  ui.tblScripts->setColumnWidth(0, nameMaxWidth);

  if (isNew) {

    auto te = new QTextEdit();
    te->setFrameShape(QFrame::NoFrame);
    int tinx = ui.tabWidget->addTab(te, name);
    ui.tabWidget->setCurrentIndex(tinx);

    connect(te, &QTextEdit::textChanged, [this, rowCnt]() {
      scrState_[rowCnt].isChange = true;
      ui.lbChange->setText("*");
    });

    scrState_.push_back(scriptState(name, "", tinx));

  }
  else {

    QFile file(QApplication::applicationDirPath() + "/scripts/" + name);

    QTextStream txtStream(&file);
    txtStream.setCodec("utf8");

    file.open(QIODevice::ReadOnly);

    QString text = txtStream.readAll();

    file.close();

    scrState_.push_back(scriptState(name, text));
  }
}

bool ScriptDialog::isActiveScript(const QString& sname) {

  auto sts = std::find_if(scrState_.begin(), scrState_.end(),
    [sname](const scriptState& st) {
    return st.name == sname;
  }
  );

  if (sts == scrState_.end()) {

    addScript(sname);

    sts = &scrState_.back();
  }

  return sts->isActive;
}

void ScriptDialog::activeScript(const QString& sname) {

  if (!isActiveScript(sname)) {

    std::unique_lock<std::mutex> lck(mtx_);

    std::find_if(scrState_.begin(), scrState_.end(),
      [sname](const scriptState& st) {
      return st.name == sname;
    }
    )->isActive = true;

    int rowCnt = ui.tblActiveScripts->rowCount();
    ui.tblActiveScripts->insertRow(rowCnt);
    auto itm = new QTableWidgetItem(sname);
    itm->setFlags(itm->flags() ^ Qt::ItemFlag::ItemIsEditable);
    ui.tblActiveScripts->setItem(rowCnt, 0, itm);

    buffCPos_ = 0;
  }
}

void ScriptDialog::deactiveScript(const QString& sname) {

  if (isActiveScript(sname)) {

    int rows = ui.tblActiveScripts->rowCount();
    for (int i = 0; i < rows; ++i) {

      if (sname == ui.tblActiveScripts->item(i, 0)->text()) {

        std::find_if(scrState_.begin(), scrState_.end(),
          [sname](const scriptState& st) {
          return st.name == sname;
        }
        )->isActive = false;

        ui.tblActiveScripts->removeRow(i);
        break;
      }
    }
  }
}

void ScriptDialog::refreshScript(const QString& sname) {

  QFile file(QApplication::applicationDirPath() + "/scripts/" + sname);

  QTextStream txtStream(&file);
  txtStream.setCodec("utf8");

  file.open(QIODevice::ReadOnly);

  auto sts = std::find_if(scrState_.begin(), scrState_.end(),
    [sname](const scriptState& st) {
    return st.name == sname;
  }
  );

  sts->text = txtStream.readAll();

  file.close();

  int sz = ui.tabWidget->count();
  for (int i = 0; i < sz; ++i) {
    if (ui.tabWidget->tabText(i) == sname) {

      ((QTextEdit*)ui.tabWidget->widget(i))->setText(sts->text);

      sts->isChange = false;
      ui.lbChange->setText("");

      break;
    }
  }
}

void ScriptDialog::nameScriptChange(int row, int col) {

  if ((col == 0) && (row < scrState_.size())) {

    QString sname = ui.tblScripts->item(row, 0)->text();

    if (std::find_if(scrState_.begin(), scrState_.end(),

      [sname](const scriptState& st) {

      return st.name == sname;

    }) != scrState_.end())
    {
      ui.txtStatusMess->append(QString::fromStdString(SV_Misc::currDateTime()) + " " + tr("Скрипт с таким именем уже существует"));

      ui.tblScripts->item(row, 0)->setText(scrState_[row].name);
      return;
    }

    if (scrState_[row].tabInx >= 0)
      ui.tabWidget->setTabText(scrState_[row].tabInx, sname);

    int sz = ui.tblActiveScripts->rowCount();
    for (int i = 0; i < sz; ++i) {
      if (ui.tblActiveScripts->item(i, 0)->text() == scrState_[row].name) {
        ui.tblActiveScripts->item(i, 0)->setText(sname);
        break;
      }
    }

    QFile::rename(QApplication::applicationDirPath() + "/scripts/" + scrState_[row].name,
      QApplication::applicationDirPath() + "/scripts/" + sname);

    scrState_[row].name = sname;
  }
}

void ScriptDialog::saveScript() {

  if (ui.tabWidget->currentIndex() < 0)
    return;

  QString sname = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
  auto it = std::find_if(scrState_.begin(), scrState_.end(),

    [sname](const scriptState& st) {

    return st.name == sname;
  }
  );

  if (it->isChange && mtx_.try_lock()) {

    QFile file(QApplication::applicationDirPath() + "/scripts/" + sname);

    QTextStream txtStream(&file);
    txtStream.setCodec("utf8");

    file.open(QIODevice::WriteOnly);

    it->text = ((QTextEdit*)ui.tabWidget->currentWidget())->toPlainText();
    it->isChange = false;

    txtStream << it->text;

    file.close();

    buffCPos_ = 0;

    mtx_.unlock();

    ui.lbChange->setText("");
  }
}

QString ScriptDialog::exlName(QString name) {

  int ptrInx = name.lastIndexOf(".");
  QString begName = name.left(ptrInx),
    endName = ptrInx > 0 ? name.right(name.size() - ptrInx) : "";

  int rowCnt = ui.tblScripts->rowCount(), newScpCnt = 0;
  for (int i = 0; i < rowCnt; ++i) {

    if (ui.tblScripts->item(i, 0)->text() == name) {
      ++newScpCnt;

      name = begName + QString::number(newScpCnt) + endName;
      i = -1;
    }
  }

  return name;
}

void ScriptDialog::workCycle() {

  QString path = QApplication::applicationDirPath() + "/scripts/";

  SV_Misc::Front fp;
  SV_Misc::TimerDelay tmDelay;
  tmDelay.update();

  while (!isStopWork_) {

    tmDelay.update();

    mtx_.lock();

    QString serr;

    // load.lua
    auto loadScr = std::find_if(scrState_.begin(), scrState_.end(),
      [](const scriptState& st) {
      return st.name == "load.lua";
    });

    if ((loadScr != scrState_.end()) && fp.PosFront(loadScr->isActive, 0)) {

      luaL_loadstring(luaState_, qUtf8Printable(loadScr->text));

      lua_pcall(luaState_, 0, 0, 0);

      const char* err = lua_tostring(luaState_, -1);
      if (err) {
        serr = QString(err);
        lua_pop(luaState_, -1);
      }
    }

    bool isActive = false,
      isNewCycle = (buffCPos_ == 0);

    QString allScr;
    for (auto& s : scrState_) {
      if (s.isActive && (s.name != "load.lua")) {
        isActive = true;

        allScr += s.text + '\n';
      }
    }

    // other scripts
    if (isActive && serr.isEmpty()) {

      luaL_loadstring(luaState_, qUtf8Printable(allScr));

      if (mode_ == SV_Script::ModeGr::viewer) {

        bool isNoError = false;
        while (buffCPos_ < buffSz_) {

          for (iterValue_ = 0; iterValue_ < size_t(SV_PACKETSZ); ++iterValue_) {

            lua_pushvalue(luaState_, -1);

            lua_pcall(luaState_, 0, 0, 0);

            if (!isNoError) {
              const char* err = lua_tostring(luaState_, -1);
              if (err) {
                serr = QString(err);
                lua_pop(luaState_, -1);
                break;
              }
              else
                isNoError = true;
            }
          }
          iterValue_ = 0;
          ++buffCPos_;

          if (!isNoError) break;
        }
      }
      else { // SV_Script::ModeGr::player

        bool isNoError = false;
        for (iterValue_ = 0; iterValue_ < size_t(SV_PACKETSZ); ++iterValue_) {

          lua_pushvalue(luaState_, -1);

          lua_pcall(luaState_, 0, 0, 0);

          if (!isNoError) {
            const char* err = lua_tostring(luaState_, -1);
            if (err) {
              serr = QString(err);
              lua_pop(luaState_, -1);
              break;
            }
            else
              isNoError = true;
          }
        }
        iterValue_ = 0;
      }
    }

    mtx_.unlock();

    if (isActive && isNewCycle && pfUpdateSignalsCBack && (mode_ == SV_Script::ModeGr::viewer))
      pfUpdateSignalsCBack();

    if (!serr.isEmpty()) {
      QString qmess = QString::fromStdString(SV_Misc::currDateTime()) + " " + serr;
      QMetaObject::invokeMethod(scrDialogRef->ui.txtStatusMess, "append", Qt::AutoConnection, Q_ARG(QString, qmess));
    }

    int ms = SV_CYCLESAVE_MS - (int)tmDelay.getCTime();
    if (ms > 0)
      SV_Misc::sleepMs(ms);
  }
}
