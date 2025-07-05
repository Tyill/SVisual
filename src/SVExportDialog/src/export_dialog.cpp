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

namespace SV_Exp {

  lockReadSDataCBack pfLockReadSData = nullptr;
  unlockReadSDataCBack pfUnlockReadSData = nullptr;
  
  QDialog* createExportDialog(QWidget *parent, SV_Exp::Config cng) {

    return new ExportDialog(parent, cng);
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

  void setGetCopySignalRefCBack(QDialog *exp, getCopySignalRefCBack f) {
      if (exp) {
          static_cast<ExportDialog*>(exp)->pfGetCopySignalRef = f;
      }
  }

  void setGetCopyModuleRefCBack(QDialog *exp, getCopyModuleRefCBack f) {
      if (exp) {
          static_cast<ExportDialog*>(exp)->pfGetCopyModuleRef = f;
      }
  }

  void setGetSignalDataCBack(QDialog *exp, getSignalDataCBack f) {
      if (exp) {
          static_cast<ExportDialog*>(exp)->pfGetSignalData = f;
      }
  }

  void setLoadSignalDataCBack(QDialog *exp, isLoadSignalDataCBack f) {
      if (exp) {
          static_cast<ExportDialog*>(exp)->pfLoadSignalData = f;
      }
  }

  void setGetTimeIntervalCBack(QDialog* exp, getTimeIntervalCBack f) {
      if (exp) {
          static_cast<ExportDialog*>(exp)->pfGetTimeInterval = f;
      }
  }

  void setSetTimeIntervalCBack(QDialog* exp, setTimeIntervalCBack f) {
      if (exp) {
          static_cast<ExportDialog*>(exp)->pfSetTimeInterval = f;
      }
  }

LockerReadSDataExp::LockerReadSDataExp() {
    if (SV_Exp::pfLockReadSData) SV_Exp::pfLockReadSData();
}
LockerReadSDataExp::~LockerReadSDataExp() {
    if (SV_Exp::pfUnlockReadSData) SV_Exp::pfUnlockReadSData();
}

}
