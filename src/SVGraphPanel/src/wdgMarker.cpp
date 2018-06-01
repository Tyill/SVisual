
#include "stdafx.h"
#include "wdgMarker.h"

using namespace std;

wdgMarker::wdgMarker(QWidget *parent){

	this->setParent(parent);

	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::red);
	this->setAutoFillBackground(true);
	this->setPalette(p);
		
	setFixedSize(5, 20);	
	setToolTipDuration(0);
}

wdgMarker::~wdgMarker(){}


void wdgMarker::mouseMoveEvent(QMouseEvent * event){

	if (IsSelect){
		
		int cp = this->cursor().pos().x();
		
		int dx = cp - presPosX_;
		presPosX_ = cp;

		int x = this->pos().x() + dx;

		x = max(minPosX_, x); x = min(maxPosX_, x);
		QRect gm = this->geometry(); gm.setX(x);

		this->setGeometry(gm);
				
		emit req_markerChange();
		
	}

	//QWidget::mouseMoveEvent(event);
}

void wdgMarker::mousePressEvent(QMouseEvent * event){

	if (event->button() == Qt::LeftButton) {
		presPosX_ = this->cursor().pos().x();
		IsSelect = true;
	}

	//QWidget::mousePressEvent(event);
}

void wdgMarker::mouseReleaseEvent(QMouseEvent * event){

	IsSelect = false;

	//QWidget::mouseReleaseEvent(event);
}


void wdgMarker::setLimitPosX(int min, int max){

	minPosX_ = min;
	maxPosX_ = max;

	int x = std::max(minPosX_, this->pos().x()); x = std::min(maxPosX_, x);

	QRect gm = this->geometry(); gm.setX(x);
	
	this->setGeometry(gm);
		
}

void wdgMarker::setLimitPosY(int min, int max){

	minPosY_ = min;
	maxPosY_ = max;

	int y = std::max(minPosY_, this->pos().y()); y = std::min(maxPosY_, y);

	QRect gm = this->geometry(); gm.setY(y);

	this->setGeometry(gm);
		
}

void wdgMarker::setPos(QPoint pos){

	QRect gm = this->geometry(); gm.setX(pos.x()); //gm.setY(pos.y());
		
	this->setGeometry(gm);
	
}

