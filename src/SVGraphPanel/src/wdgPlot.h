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
#include "wdgAxisTime.h"
#include "wdgAxisValue.h"

class wdgPlot : public QWidget
{
  Q_OBJECT

private:
  
  QPoint presPnt_;
    
  bool lpm_ = false, rpm_ = false, keyCntr_ = false;
  QTimer* tmrMarkerPos_ = NULL;

  wdgAxisTime* axisTime_ = NULL;
  wdgAxisValue* axisValue_ = NULL;

public:
  wdgPlot(QWidget *parent = 0);

  QRect SelRect;

  void setAxisTime(wdgAxisTime* ax);
  void setAxisValue(wdgAxisValue* ax);
  void scale(int delta);
  
protected:  
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent * event);

signals:
  void req_rctChange();
  void req_moveChange();
  void req_updMarker();
};