
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

				QString sign = item->text(4);
				
				mimeData->setText(sign);
				QDrag *drag = new QDrag(this);
				drag->setMimeData(mimeData);
				
				drag->exec();
			}
		}
	}
	
	QTreeWidget::mouseMoveEvent(event);
}

