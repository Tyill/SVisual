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

#include "drag_label.h"

#include <QtGui>
#include <QApplication>

namespace SV_Graph {

DragLabel::DragLabel(QWidget *parent):
    QLabel(parent)
{

}

void DragLabel::setSignal(QString sign) {

  sign_ = sign;
}

void DragLabel::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton)
    startMovePos_ = event->pos();

  if (event->button() == Qt::RightButton)
    emit req_delSignal(sign_);


  //QLabel::mousePressEvent(event);

}

void DragLabel::mouseMoveEvent(QMouseEvent *event) {

  if (event->buttons() & Qt::LeftButton) {

    int dist = (event->pos() - startMovePos_).manhattanLength();
    if (dist >= QApplication::startDragDistance()) {

      QMimeData *mimeData = new QMimeData;

      mimeData->setText(sign_);
      QDrag *drag = new QDrag(this);
      drag->setMimeData(mimeData);

      drag->exec();
    }
  }

  QLabel::mouseMoveEvent(event);
}
}
