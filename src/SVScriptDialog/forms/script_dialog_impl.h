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

#include <thread>
#include <mutex>
#include <map>

#include "GeneratedFiles/ui_script_dialog.h"
#include "SVBase/base.h"
#include "SVScriptDialog/script_dialog.h"
#include "Lua/lua.hpp"

#include <QDialog>

class ScriptDialog : public QDialog
{
  Q_OBJECT

  friend void printMess(const std::string& mess);
  friend uint64_t getTimeValue(const std::string& module, const std::string& signal);
  friend bool getBoolValue(const std::string& module, const std::string& signal);
  friend int getIntValue(const std::string& module, const std::string& signal);
  friend float getFloatValue(const std::string& module, const std::string& signal);
  friend void setBoolValue(const std::string& signal, bool Value, uint64_t time);
  friend void setIntValue(const std::string& signal, int Value, uint64_t time);
  friend void setFloatValue(const std::string& signal, float Value, uint64_t time);

public:

  SV_Script::pf_updateSignalsCBack pfUpdateSignalsCBack = nullptr;
  SV_Script::pf_addSignalsCBack pfAddSignalsCBack = nullptr;
  SV_Script::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
  SV_Script::pf_getSignalData pfGetSignalData = nullptr;
  SV_Script::pf_getModuleData pfGetModuleData = nullptr;
  SV_Script::pf_addSignal pfAddSignal = nullptr;
  SV_Script::pf_addModule pfAddModule = nullptr;
  SV_Script::pf_moduleConnectCBack pfModuleConnectCBack = nullptr;
  SV_Script::pf_loadSignalData pfLoadSignalData = nullptr;
  SV_Script::pf_changeSignColor pfChangeSignColor = nullptr;

  ScriptDialog(QWidget *parent, SV_Script::Config, SV_Script::ModeGr mode);
  ~ScriptDialog();

  void startUpdateThread();

  bool isActiveScript(const QString& fname);

  void activeScript(const QString& fname);

  void deactiveScript(const QString& fname);

  void refreshScript(const QString& fname);

private:
  Ui::ScriptDialog ui;

  SV_Script::ModeGr mode_;

  SV_Script::Config cng;

  lua_State* luaState_ = nullptr;

  std::thread workThr_;
  std::mutex mtx_;
  volatile bool isStopWork_ = false;

  QMap<QString, SV_Base::SignalData*> signBuff_;

  size_t iterValue_ = 0,
    buffCPos_ = 0,
    buffSz_ = 1;

  struct scriptState {
    bool isChange = true;
    bool isActive = false;
    QString name;
    QString text;
    int tabInx = 0;

    scriptState(QString name_ = "", QString text_ = "", int tabInx_ = -1, bool isChange_ = false) :
      name(name_), text(text_), tabInx(tabInx_), isChange(isChange_) {}
  };

  QVector<scriptState> scrState_;

  QString exlName(QString);

  void updateSign(SV_Base::SignalData* sign, size_t beginPos, size_t valuePos);
  bool updateBuffValue(const QString& module, const QString& signal, SV_Base::ValueType stype);

  void setValue(const QString& signal, SV_Base::Value Value, uint64_t time);
  void contextMenuEvent(QContextMenuEvent * event);
  void workCycle();

  private slots:
  void addScript(QString name = "");
  void saveScript();
  void nameScriptChange(int row, int col);

};


