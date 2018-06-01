
#include "stdafx.h"
#include "dragLabel.h"

dragLabel::dragLabel(QWidget *parent){

	setParent(parent);
		
}

void dragLabel::setSignal(QString sign){

	sign_ = sign;
}

void dragLabel::mousePressEvent(QMouseEvent *event){
	
	if (event->button() == Qt::LeftButton)
		startMovePos_ = event->pos();

	if (event->button() == Qt::RightButton)
		emit req_delSignal(sign_);


	//QLabel::mousePressEvent(event);

}

void dragLabel::mouseMoveEvent(QMouseEvent *event){

	if (event->buttons() & Qt::LeftButton) {

		int dist = (event->pos() - startMovePos_).manhattanLength();
		if (dist >= QApplication::startDragDistance()){
						
				QMimeData *mimeData = new QMimeData;
								
				mimeData->setText(sign_);
				QDrag *drag = new QDrag(this);
				drag->setMimeData(mimeData);
								
				drag->exec();			
		}
	}

	QLabel::mouseMoveEvent(event);
}

