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

#include "stdafx.h"

class wdgAxisTime : public QWidget
{
  Q_OBJECT
private:
    
  int cng_dashHeight_ = 3;
  
  int curDashStep_ = 100;
  int curOffsPos_ = 0;
  int fontMetr_ = 0;
  int curIntervSec_ = 0;

  QPair<qint64, qint64> tmInterval_;
    
  double tmScale_ = 1.0;

  int mousePrevPosX_ = 0;

  
  void resizeEvent(QResizeEvent * event);

  void drawDashLines(QPainter& painter);
  void drawTimeMark(QPainter& painter);

  QString getTimeMark(int offs);


public:
  wdgAxisTime(QWidget *parent = 0);
  ~wdgAxisTime();

  void setTimeInterval(qint64, qint64);

  QPair<qint64, qint64> getTimeInterval();

  double getTimeScale();

  QVector<int> getAxisMark();

  void mouseMoveEvent(QMouseEvent * event);
  void mousePressEvent(QMouseEvent * event);
  void wheelEvent(QWheelEvent * event);
  void scale(int delta);

protected:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
  void req_axisChange();

};