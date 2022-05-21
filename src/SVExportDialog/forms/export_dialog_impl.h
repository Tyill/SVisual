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

#include "GeneratedFiles/ui_export_dialog.h"
#include "SVBase/base.h"
#include "SVExportDialog/export_dialog.h"

class ExportDialog : public QDialog
{
  Q_OBJECT

public:

  SV_Exp::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
  SV_Exp::pf_getCopyModuleRef pfGetCopyModuleRef = nullptr;
  SV_Exp::pf_getSignalData pfGetSignalData = nullptr;
  SV_Exp::pf_loadSignalData pfLoadSignalData = nullptr;
  SV_Exp::pf_setTimeInterval pfSetTimeInterval = nullptr;
  SV_Exp::pf_getTimeInterval pfGetTimeInterval = nullptr;

  ExportDialog(QWidget *parent, SV_Exp::Config);
  ~ExportDialog();

private:
  Ui::ExportDialog ui;
  QString selModule_, selSignal_, selDirMem_;

private:
  void showEvent(QShowEvent * event);

  SV_Exp::Config cng;

  QSet<QString> expSignals_;

  void updateTableSignal();
  void updateTableExport();
#ifdef USE_QtXlsxWriter
  void exportToXLSX(QString fileName);
#endif
  void exportToTXT(QString fileName);
  void exportToJSON(QString fileName);

  private slots:
  void selModule(QListWidgetItem* item);
  void addSignalOnExport();
  void delSignalFromExport();
};


