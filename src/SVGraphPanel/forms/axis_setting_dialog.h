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

#include "GeneratedFiles/ui_axis_setting_dialog.h"
#include "SVGraphPanel/graph_panel.h"
#include "SVBase/sv_limits.h"

#include <QTranslator>

class AxisSettingDialog : public QDialog
{
  Q_OBJECT

public:
  AxisSettingDialog(QWidget* parent, const SV_Graph::AxisAttributes& attr):
      QDialog(parent)
  {
#ifdef SV_EN
    QTranslator translator;
    translator.load(":/SVGp/svgraphpanel_en.qm");
    QCoreApplication::installTranslator(&translator);
#endif

    ui.setupUi(this);

    ui.chbAuto->setChecked(attr.isAuto);

    ui.txtMaxValue->setEnabled(!attr.isAuto);
    ui.txtMinValue->setEnabled(!attr.isAuto);
    ui.txtStepValue->setEnabled(!attr.isAuto);

    ui.txtMaxValue->setText(QString::number(attr.max, 'f', 1));
    ui.txtMinValue->setText(QString::number(attr.min, 'f', 1));
    ui.txtStepValue->setText(QString::number(attr.step, 'f', 1));

    connect(ui.chbAuto, SIGNAL(stateChanged(int)), this, SLOT(paramChange()));
    connect(ui.txtMaxValue, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
    connect(ui.txtMinValue, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
    connect(ui.txtStepValue, SIGNAL(textEdited(QString)), this, SLOT(paramChange()));
  }
  ~AxisSettingDialog() = default;

  Ui::AxisSettingDialog ui;

private slots:
  void paramChange() {
    SV_Graph::AxisAttributes attr;

    attr.isAuto = ui.chbAuto->isChecked();

    ui.txtMaxValue->setEnabled(!attr.isAuto);
    ui.txtMinValue->setEnabled(!attr.isAuto);
    ui.txtStepValue->setEnabled(!attr.isAuto);


    attr.max = ui.txtMaxValue->text().toDouble();
    attr.min = ui.txtMinValue->text().toDouble();
    attr.step = ui.txtStepValue->text().toDouble();

    emit req_settChange(attr);
  }

signals:
  void req_settChange(SV_Graph::AxisAttributes);
};


