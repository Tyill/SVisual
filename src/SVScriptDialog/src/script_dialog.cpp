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

namespace SV_Script {

  QDialog *createScriptDialog(QWidget *parent, Config cng, ModeGr mode) {

    return new ScriptDialog(parent, cng, mode);
  }

  void startUpdateThread(QDialog* stp) {

    if (stp)
      ((ScriptDialog *)stp)->startUpdateThread();
  }

  void setGetCopySignalRef(QDialog *stp, pf_getCopySignalRef f) {

    if (stp)
      ((ScriptDialog *)stp)->pfGetCopySignalRef = f;
  }

  void setGetModuleData(QDialog* stp, pf_getModuleData f) {

    if (stp)
      ((ScriptDialog *)stp)->pfGetModuleData = f;
  }

  void setGetSignalData(QDialog *stp, pf_getSignalData f) {

    if (stp)
      ((ScriptDialog *)stp)->pfGetSignalData = f;
  }

  void setAddSignal(QDialog *stp, pf_addSignal f) {

    if (stp)
      ((ScriptDialog *)stp)->pfAddSignal = f;
  }

  void setAddModule(QDialog *stp, pf_addModule f) {

    if (stp)
      ((ScriptDialog *)stp)->pfAddModule = f;
  }

  void setChangeSignColor(QDialog* stp, pf_changeSignColor f) {

    if (stp)
      ((ScriptDialog *)stp)->pfChangeSignColor = f;
  }

  bool isActiveScript(QDialog* stp, const QString& fname) {

    if (stp)
      return ((ScriptDialog *)stp)->isActiveScript(fname);
    else
      return false;
  }

  void activeScript(QDialog* stp, const QString& fname) {

    if (stp)
      ((ScriptDialog *)stp)->activeScript(fname);
  }

  void deactiveScript(QDialog* stp, const QString& fname) {

    if (stp)
      ((ScriptDialog *)stp)->deactiveScript(fname);
  }

  void refreshScript(QDialog* stp, const QString& fname) {

    if (stp)
      ((ScriptDialog *)stp)->refreshScript(fname);
  }

  void setLoadSignalData(QDialog *stp, pf_loadSignalData f) {

    if (stp)
      ((ScriptDialog *)stp)->pfLoadSignalData = f;
  }

  // обновление данных callBack
  void setUpdateSignalsCBack(QDialog *stp, pf_updateSignalsCBack f) {

    if (stp)
      ((ScriptDialog *)stp)->pfUpdateSignalsCBack = f;
  }

  // добавление сигнала callBack
  void setAddSignalsCBack(QDialog *stp, pf_addSignalsCBack f) {

    if (stp)
      ((ScriptDialog *)stp)->pfAddSignalsCBack = f;
  }

  // модуль подключен
  void setModuleConnectCBack(QDialog* stp, pf_moduleConnectCBack f) {

    if (stp)
      ((ScriptDialog *)stp)->pfModuleConnectCBack = f;
  }
}