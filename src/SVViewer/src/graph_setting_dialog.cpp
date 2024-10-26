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

#include "SVViewer/forms/graph_setting_dialog.h"
#include "SVViewer/forms/main_win.h"

GraphSettingDialog::GraphSettingDialog(QWidget *parent, const SV_Graph::GraphSetting& gs):
    QDialog(parent)
{

  MainWin* mainWin = (MainWin*)parent;

  ui.setupUi(this);

  ui.slrColorTransparent->setValue(gs.transparent);
  ui.slrPenWidth->setValue(gs.lineWidth);
  ui.chbDarkTheme->setChecked(gs.darkTheme);
  ui.chbSignBoolOnTop->setChecked(gs.signBoolOnTop);
  ui.sbGapTolerance->setValue(gs.gapTolerance);

  connect(ui.slrColorTransparent, &QSlider::sliderMoved, [this, mainWin](int pos) {

    SV_Graph::GraphSetting gs = graphSetting();
    gs.transparent = pos;
   
    mainWin->updateGraphSetting(gs);
  });

  connect(ui.sbGapTolerance, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, mainWin](int pos) {

      SV_Graph::GraphSetting gs = graphSetting();
      gs.gapTolerance = pos;

      mainWin->updateGraphSetting(gs);
  });

  connect(ui.slrPenWidth, &QSlider::sliderMoved, [this, mainWin](int pos) {

    SV_Graph::GraphSetting gs = graphSetting();
    gs.lineWidth = pos;

    mainWin->updateGraphSetting(gs);
  });

  connect(ui.chbDarkTheme, &QCheckBox::stateChanged, [this, mainWin]() {
    mainWin->updateGraphSetting(graphSetting());
  });

  connect(ui.chbSignBoolOnTop, &QCheckBox::stateChanged, [this, mainWin]() {
    mainWin->updateGraphSetting(graphSetting());
  });
}

SV_Graph::GraphSetting GraphSettingDialog::graphSetting(){

  SV_Graph::GraphSetting gs;
  gs.transparent = ui.slrColorTransparent->value();
  gs.lineWidth = ui.slrPenWidth->value();
  gs.darkTheme = ui.chbDarkTheme->isChecked();
  gs.signBoolOnTop = ui.chbSignBoolOnTop->isChecked();

  return gs;
}