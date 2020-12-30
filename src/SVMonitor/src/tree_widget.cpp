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
#include "stdafx.h"
#include "treeWidgetExt.h"

treeWidgetExt::treeWidgetExt(QWidget *parent){

  this->setParent(parent);

  setSelectionMode(QAbstractItemView::SingleSelection);
  setDragEnabled(true);
  setDropIndicatorShown(true);

}

treeWidgetExt::~treeWidgetExt(){


}

void treeWidgetExt::mousePressEvent(QMouseEvent *event){

  if (event->button() == Qt::LeftButton)
    startMovePos_ = event->pos();


  QTreeWidget::mousePressEvent(event);
}

void treeWidgetExt::mouseMoveEvent(QMouseEvent *event){

  if (event->buttons() & Qt::LeftButton) {

    int dist = (event->pos() - startMovePos_).manhattanLength();
    if (dist >= QApplication::startDragDistance()){

      QTreeWidgetItem *item = currentItem();
      if (item) {
        QMimeData *mimeData = new QMimeData;

        QString sign = item->text(5);

        mimeData->setText(sign);
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);

        drag->exec();
      }
    }
  }

  QTreeWidget::mouseMoveEvent(event);
}


