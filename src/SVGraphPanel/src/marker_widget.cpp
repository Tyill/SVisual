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

#include "marker_widget.h"

#include <QtGui>

MarkerWidget::MarkerWidget(QWidget *parent):
    QWidget(parent)
{

  QPalette p(palette());
  p.setColor(QPalette::Background, Qt::red);
  this->setAutoFillBackground(true);
  this->setPalette(p);

  setFixedSize(5, 20);
  setToolTipDuration(0);
}

MarkerWidget::~MarkerWidget() {}


void MarkerWidget::mouseMoveEvent(QMouseEvent * event) {

  if (isSelect_) {

    int cp = this->cursor().pos().x();

    int dx = cp - presPosX_;
    presPosX_ = cp;

    int x = this->pos().x() + dx;

    x = qMax(minPosX_, x); x = qMin(maxPosX_, x);
    QRect gm = this->geometry(); gm.setX(x);

    this->setGeometry(gm);

    emit req_markerChange();

  }

  //QWidget::mouseMoveEvent(event);
}

void MarkerWidget::mousePressEvent(QMouseEvent * event) {

  if (event->button() == Qt::LeftButton) {
    presPosX_ = this->cursor().pos().x();
    isSelect_ = true;
  }

  //QWidget::mousePressEvent(event);
}

void MarkerWidget::mouseReleaseEvent(QMouseEvent * event) {

  isSelect_ = false;

  //QWidget::mouseReleaseEvent(event);
}


void MarkerWidget::setLimitPosX(int min, int max) {

  minPosX_ = min;
  maxPosX_ = max;

  int x = std::max(minPosX_, this->pos().x()); x = std::min(maxPosX_, x);

  QRect gm = this->geometry(); gm.setX(x);

  this->setGeometry(gm);

}

void MarkerWidget::setLimitPosY(int min, int max) {

  minPosY_ = min;
  maxPosY_ = max;

  int y = std::max(minPosY_, this->pos().y()); y = std::min(maxPosY_, y);

  QRect gm = this->geometry(); gm.setY(y);

  this->setGeometry(gm);

}

void MarkerWidget::setPos(QPoint pos) {

  QRect gm = this->geometry(); gm.setX(pos.x()); //gm.setY(pos.y());

  this->setGeometry(gm);
}
