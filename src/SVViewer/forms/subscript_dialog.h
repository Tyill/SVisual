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

#include "SVScriptDialog/script_dialog.h"
#include "SVBase/sv_limits.h"
#include "GeneratedFiles/ui_subscript_dialog.h"


class SubScriptDialog : public QDialog
{
  Q_OBJECT

public:
  SubScriptDialog(QDialog* mainScrPanel, QWidget* parent);
  ~SubScriptDialog() = default;

  void showSignScript(const QString& signal, const QString& module, SV_Base::ValueType type);

private:

  Ui::SubScriptDialog ui;

  QString sFileName_;

  QDialog* mainScrPanel_ = nullptr;

  void closeEvent(QCloseEvent* e);

  void saveScript();
};



