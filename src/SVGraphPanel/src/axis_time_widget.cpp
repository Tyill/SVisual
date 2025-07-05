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

#include "axis_time_widget.h"

#include <QtGui>

AxisTimeWidget::AxisTimeWidget(QWidget *parent):
    QWidget(parent)
{

  tmInterval_.first = QDateTime::currentDateTime().toMSecsSinceEpoch();
  tmInterval_.second = tmInterval_.first + 3600000;

  tmScale_ = (double)(tmInterval_.second - tmInterval_.first) / width();

}

AxisTimeWidget::~AxisTimeWidget() {}

void AxisTimeWidget::mouseMoveEvent(QMouseEvent * event) {

  int pos = event->pos().x(), diff = pos - mousePrevPosX_;

  curOffsPos_ += diff;

  if (curOffsPos_ > curDashStep_) curOffsPos_ = 0;
  if (curOffsPos_ < 0) curOffsPos_ = curDashStep_;

  qint64 offs = -tmScale_ * diff - 1;
  if (diff < 0) offs = -tmScale_ * diff + 1;

  tmInterval_.first += offs;
  tmInterval_.second += offs;

  curIntervSec_ = (tmInterval_.second - tmInterval_.first) / 1000;

  tmScale_ = (double)(tmInterval_.second - tmInterval_.first) / width();

  mousePrevPosX_ = pos;

  emit req_axisChange();

}

void AxisTimeWidget::mousePressEvent(QMouseEvent * event) {

  mousePrevPosX_ = event->pos().x();
}

void AxisTimeWidget::scale(int delta, int mpos) {

  qint64 mposTimeBefore = (double(tmInterval_.second - tmInterval_.first) * mpos) / width() + tmInterval_.first;

  if (delta > 0) curDashStep_++;
  else curDashStep_--;

  if (curDashStep_ > 3 * fontMetr_) curDashStep_ = 2 * fontMetr_;
  else if (curDashStep_ < fontMetr_ * 1.1) curDashStep_ = 2 * fontMetr_;

  int offs = 10000;

  if (curIntervSec_ > 86400) offs *= 1000;
  else if (curIntervSec_ > 3600) offs *= 100;
  else if (curIntervSec_ < 1) offs /= 1000;
  else if (curIntervSec_ < 60) offs /= 10;

  if (delta > 0) {

    tmInterval_.first += offs;
    tmInterval_.second += -offs;

    if (tmInterval_.first >= tmInterval_.second) {

      qint64 mdl = abs(tmInterval_.second + tmInterval_.first) / 2;

      tmInterval_.first = mdl - 10;
      tmInterval_.second = mdl + 10;
    }
  }
  else {
    tmInterval_.first += -offs;
    tmInterval_.second += offs;
  }
  qint64 mposTimePost = (double(tmInterval_.second - tmInterval_.first) * mpos) / width() + tmInterval_.first;

  tmInterval_.first -= mposTimePost - mposTimeBefore;
  tmInterval_.second -= mposTimePost - mposTimeBefore;

  curIntervSec_ = (tmInterval_.second - tmInterval_.first) / 1000;

  tmScale_ = (double)(tmInterval_.second - tmInterval_.first) / width();

  emit req_axisChange();

}

void AxisTimeWidget::wheelEvent(QWheelEvent * event) {

  scale(event->delta(), event->pos().x());
  event->accept();
}

void AxisTimeWidget::resizeEvent(QResizeEvent * event) {

  tmScale_ = (double)(tmInterval_.second - tmInterval_.first) / width();

  emit req_axisChange();
}

void AxisTimeWidget::drawDashLines(QPainter& painter) {

  int w = width();

  painter.setPen(Qt::gray);
  painter.drawLine(QPoint(0, 0), QPoint(w, 0));

  painter.setPen(Qt::black);
  int cWidth = curOffsPos_ % curDashStep_;

  while (cWidth < w) {

    painter.drawLine(QPoint(cWidth, 0), QPoint(cWidth, cng_dashHeight_));

    cWidth += curDashStep_;
  }
}

QString AxisTimeWidget::getTimeMark(int offs) {

  QString timeMark = "";

  QDateTime dt = QDateTime::fromMSecsSinceEpoch(tmInterval_.first + tmScale_ * offs);

  timeMark = dt.time().toString("hh:mm:ss:zzz");

  if (curIntervSec_ > 86400) timeMark = dt.toString("dd.MM.yy hh:mm");
  else if (curIntervSec_ > 60) timeMark = dt.time().toString("hh:mm:ss");

  return timeMark;
}

void AxisTimeWidget::drawTimeMark(QPainter& painter) {

  int w = width(), h = height();

  int cWidth = curOffsPos_ % curDashStep_;

  while (cWidth < w) {

    QString timeMark = getTimeMark(cWidth);

    fontMetr_ = this->fontMetrics().width(timeMark);

    painter.drawText(QPoint(cWidth - fontMetr_ / 2, h / 2), timeMark);

    cWidth += curDashStep_;
  }
}

void AxisTimeWidget::paintEvent(QPaintEvent *event) {

  QPainter painter(this);

  drawDashLines(painter);

  drawTimeMark(painter);

}

void AxisTimeWidget::setTimeInterval(qint64 beginTime, qint64 endTime) {

  tmInterval_.first = beginTime;
  tmInterval_.second = endTime;

  curIntervSec_ = (tmInterval_.second - tmInterval_.first) / 1000;

  tmScale_ = (double)(tmInterval_.second - tmInterval_.first) / width();
  int cWidth = curOffsPos_ % curDashStep_;

  QString timeMark = getTimeMark(cWidth);

  fontMetr_ = this->fontMetrics().width(timeMark);

  if (curDashStep_ > 3 * fontMetr_) curDashStep_ = 2 * fontMetr_;
  else if (curDashStep_ < fontMetr_ * 1.1) curDashStep_ = 2 * fontMetr_;
}

QVector<int> AxisTimeWidget::getAxisMark() {

  QVector<int> mark;

  int w = width();

  int cWidth = curOffsPos_ % curDashStep_;

  while (cWidth < w) {

    mark.push_back(cWidth);

    cWidth += curDashStep_;
  }

  return mark;
}
