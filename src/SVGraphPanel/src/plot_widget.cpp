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

#include "plot_widget.h"
#include "axis_time_widget.h"
#include "axis_value_widget.h"

#include <QtGui>
#include <QApplication>

PlotWidget::PlotWidget(QWidget *parent):
    QWidget(parent)
{

  setAttribute(Qt::WA_NoSystemBackground);

  this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void PlotWidget::mousePressEvent(QMouseEvent *event) {


  presPnt_ = event->pos();

  if (event->button() == Qt::LeftButton) {
    lpm_ = true;

    if (!tmrMarkerPos_) {
      tmrMarkerPos_ = new QTimer(this);
      connect(tmrMarkerPos_, &QTimer::timeout, this, [this]() {

        if (lpm_) {
          keyCntr_ = true;
          emit req_updMarker();
        }

        tmrMarkerPos_->stop();
        tmrMarkerPos_->deleteLater();
        tmrMarkerPos_ = NULL;
      });

      tmrMarkerPos_->setInterval(1000);
      tmrMarkerPos_->start();
    }
  }
  else if ((event->button() == Qt::RightButton) && axisTime_ && axisValue_) {

    rpm_ = true;

    this->setCursor(Qt::ClosedHandCursor);

    axisTime_->mousePressEvent(event);
    axisValue_->mousePressEvent(event);
  }


  QWidget::mousePressEvent(event);
}

void PlotWidget::mouseReleaseEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {

    lpm_ = false;


    if (!keyCntr_) emit req_rctChange();

    keyCntr_ = false;
  }
  else if (event->button() == Qt::RightButton) {

    rpm_ = false;

    this->setCursor(Qt::ArrowCursor);

    emit req_moveChange();
  }

  QWidget::mouseReleaseEvent(event);
}

void PlotWidget::mouseMoveEvent(QMouseEvent *event) {

  int dist = (event->pos() - presPnt_).manhattanLength();
  if (dist < QApplication::startDragDistance()) return;

  if (tmrMarkerPos_) {
    tmrMarkerPos_->stop();
    tmrMarkerPos_->deleteLater();
    tmrMarkerPos_ = NULL;
  }

  if (lpm_ && keyCntr_) {

    emit req_updMarker();

  }
  else if (lpm_) {
    int w = event->pos().x() - presPnt_.x();
    int h = event->pos().y() - presPnt_.y();

    if ((w > 0) && (h > 0))
      SelRect = QRect(presPnt_.x(), presPnt_.y(), w, h);
    else if ((w < 0) && (h > 0))
      SelRect = QRect(event->pos().x(), presPnt_.y(), abs(w), h);
    else if ((w > 0) && (h < 0))
      SelRect = QRect(presPnt_.x(), event->pos().y(), w, abs(h));
    else if ((w < 0) && (h < 0))
      SelRect = QRect(event->pos().x(), event->pos().y(), abs(w), abs(h));

    update();
  }
  else if (rpm_ && axisTime_ && axisValue_) {

    axisTime_->mouseMoveEvent(event);
    axisValue_->mouseMoveEvent(event);
  }

  QWidget::mouseMoveEvent(event);
}

void PlotWidget::mouseDoubleClickEvent(QMouseEvent *event) {

    emit req_fullSize();
    QWidget::mouseDoubleClickEvent(event);
}

void PlotWidget::wheelEvent(QWheelEvent * event) {
  auto keys = event->modifiers();

  if (axisTime_ && axisValue_) {

    if (keys.testFlag(Qt::ControlModifier)) {
      axisValue_->wheelEvent(event);
    }
    else if (keys.testFlag(Qt::ShiftModifier)) {
      axisTime_->wheelEvent(event);
    }
    else {
      axisTime_->wheelEvent(event);
      axisValue_->wheelEvent(event);
    }
  }
}

void PlotWidget::scale(int delta) {

  if (axisTime_ && axisValue_) {

    axisTime_->scale(delta);
    axisValue_->scale(delta);
  }
}

void PlotWidget::setAxisTime(AxisTimeWidget* ax) {

  axisTime_ = ax;

}

void PlotWidget::setAxisValue(AxisValueWidget* ax) {

  axisValue_ = ax;

}

void PlotWidget::paintEvent(QPaintEvent *event) {

  QPainter painter(this);

  QPen pn(Qt::green, Qt::DashLine);  pn.setWidth(2);
  painter.setPen(pn);

  painter.drawRect(SelRect);

  QWidget::paintEvent(event);
}



