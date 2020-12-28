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
#include "stdafx.h"
#include "forms/scriptPanel.h"

namespace SV_Script {

    QDialog *createScriptPanel(QWidget *parent, config cng, modeGr mode) {

        return new scriptPanel(parent, cng, mode);
  }

    void startUpdateThread(QDialog* stp){

        if (stp)
            ((scriptPanel *)stp)->startUpdateThread();
    }
  
  void setGetCopySignalRef(QDialog *stp, pf_getCopySignalRef f) {

    if (stp)
            ((scriptPanel *)stp)->pfGetCopySignalRef = f;
  }

    void setGetModuleData(QDialog* stp, pf_getModuleData f){

        if (stp)
            ((scriptPanel *)stp)->pfGetModuleData = f;
    }

  void setGetSignalData(QDialog *stp, pf_getSignalData f) {

    if (stp)
            ((scriptPanel *)stp)->pfGetSignalData = f;
  }

    void setAddSignal(QDialog *stp, pf_addSignal f) {

        if (stp)
            ((scriptPanel *)stp)->pfAddSignal = f;
    }

    void setAddModule(QDialog *stp, pf_addModule f) {

        if (stp)
            ((scriptPanel *)stp)->pfAddModule = f;
    }

    void setChangeSignColor(QDialog* stp, pf_changeSignColor f){

        if (stp)
            ((scriptPanel *)stp)->pfChangeSignColor = f;
    }
    
    bool isActiveScript(QDialog* stp, const QString& fname){

        if (stp)
            return ((scriptPanel *)stp)->isActiveScript(fname);
        else
            return false;
    }

    void activeScript(QDialog* stp, const QString& fname){

        if (stp)
            ((scriptPanel *)stp)->activeScript(fname);
    }

    void deactiveScript(QDialog* stp, const QString& fname){

        if (stp)
            ((scriptPanel *)stp)->deactiveScript(fname);
    }

    void refreshScript(QDialog* stp, const QString& fname){

        if (stp)
            ((scriptPanel *)stp)->refreshScript(fname);
    }

  void setLoadSignalData(QDialog *stp, pf_loadSignalData f) {

    if (stp)
            ((scriptPanel *)stp)->pfLoadSignalData = f;
  }
    
    // обновление данных callBack
    void setUpdateSignalsCBack(QDialog *stp, pf_updateSignalsCBack f){

        if (stp)
            ((scriptPanel *)stp)->pfUpdateSignalsCBack = f;
    }

    // добавление сигнала callBack
    void setAddSignalsCBack(QDialog *stp, pf_addSignalsCBack f){
        
        if (stp)
            ((scriptPanel *)stp)->pfAddSignalsCBack = f;
    }

    // модуль подключен
    void setModuleConnectCBack(QDialog* stp, pf_moduleConnectCBack f){

        if (stp)
            ((scriptPanel *)stp)->pfModuleConnectCBack = f;
    }
}