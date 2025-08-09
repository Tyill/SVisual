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

#include <QWidget>

class HistWidget : public QWidget
{
  Q_OBJECT
private:

  QVector<QPair<int, int>> grPnts_;
  QColor hColor_ = QColor(127, 0, 127);

  int targPos_ = 0, minXPos_ = 0, minYPos_ = 0;

  double sclX_ = 0, sclY_ = 0;

  void mouseMoveEvent(QMouseEvent * event);
  void mousePressEvent(QMouseEvent * event);

public:
  HistWidget(QWidget *parent = 0);
  ~HistWidget();

  void setGraphPnt(QVector<QPair<int, int>> pnts);
  void setColor(QColor clr);

  int getAreaHist(int leftPos, int rightPos);

  QPoint getTargPos();
  int getAreaByPos();

  public slots:
  void setTargPos(QString pos);

protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
  void selValueChange();

};
