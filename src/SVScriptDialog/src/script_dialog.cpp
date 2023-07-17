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

#include "SVScriptDialog/forms/script_dialog_impl.h"

ScriptDialog* scrDialogRef = nullptr;

namespace SV_Script {

  lockReadSDataCBack pfLockReadSData = nullptr;
  unlockReadSDataCBack pfUnlockReadSData = nullptr;
    
  QDialog* getScriptDialog(QWidget *parent, Config cng, ModeGr mode) {
      if (!scrDialogRef) {
          scrDialogRef = new ScriptDialog(parent, cng, mode);
      }
      return scrDialogRef;
  }
  void startUpdateThread(QDialog* stp) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->startUpdateThread();
      }
  }
  void setGetCopySignalRef(QDialog *stp, getCopySignalRefCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfGetCopySignalRef = f;
      }
  }
  void setGetModuleData(QDialog* stp, getModuleDataCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfGetModuleData = f;
      }
  }
  void setGetSignalData(QDialog *stp, getSignalDataCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfGetSignalData = f;
      }
  }
  void setAddSignal(QDialog *stp, addSignalCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfAddSignal = f;
      }
  }
  void setAddModule(QDialog *stp, addModuleCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfAddModule = f;
      }
  }
  void setChangeSignColor(QDialog* stp, changeSignColorCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfChangeSignColor = f;
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
  bool isActiveScript(QDialog* stp, const QString& fname) {
      if (stp) {
          return static_cast<ScriptDialog*>(stp)->isActiveScript(fname);
      }else {
          return false;
      }
  }
  void activeScript(QDialog* stp, const QString& fname) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->activeScript(fname);
      }
  }
  void deactiveScript(QDialog* stp, const QString& fname) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->deactiveScript(fname);
      }
  }
  void refreshScript(QDialog* stp, const QString& fname) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->refreshScript(fname);
      }
  }
  void restartScript(QDialog* stp) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->restartScript();
      }
  }
  void setLoadSignalData(QDialog *stp, isLoadSignalDataCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfLoadSignalData = f;
      }
  }
  void setUpdateSignalsCBack(QDialog* stp, updateSignalsCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfUpdateSignalsCBack = f;
      }
  }
  void setAddSignalsCBack(QDialog *stp, addSignalsCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfAddSignalsCBack = f;
      }
  }
  void setModuleConnectCBack(QDialog* stp, moduleConnectCBack f) {
      if (stp) {
          static_cast<ScriptDialog*>(stp)->pfModuleConnectCBack = f;
      }
  }
}

LockerReadSDataScript::LockerReadSDataScript() {
    if (SV_Script::pfLockReadSData) SV_Script::pfLockReadSData();
}
LockerReadSDataScript::~LockerReadSDataScript() {
    if (SV_Script::pfUnlockReadSData) SV_Script::pfUnlockReadSData();
}