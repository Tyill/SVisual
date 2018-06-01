
#include "stdafx.h"
#include "wdgPlot.h"
#include "wdgAxisTime.h"
#include "wdgAxisValue.h"

wdgPlot::wdgPlot(QWidget *parent){

	setParent(parent);

	QPalette p(palette());
	p.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(p);
	this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void wdgPlot::mousePressEvent(QMouseEvent *event){


	presPnt_ = event->pos();	
	
	if (event->button() == Qt::LeftButton) {
		lpm_ = true;
		
		if (!tmrMarkerPos_){
			tmrMarkerPos_ = new QTimer(this);
			connect(tmrMarkerPos_, &QTimer::timeout, [=]() {



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

void wdgPlot::mouseReleaseEvent(QMouseEvent *event){

	if (event->button() == Qt::LeftButton) {
		
		lpm_ = false;


		if (!keyCntr_) emit req_rctChange();

		keyCntr_ = false;
	}
	else if (event->button() == Qt::RightButton){
		
		rpm_ = false;

		this->setCursor(Qt::ArrowCursor);

		emit req_moveChange();				
	}
			
	QWidget::mouseReleaseEvent(event);
}

void wdgPlot::mouseMoveEvent(QMouseEvent *event){

	int dist = (event->pos() - presPnt_).manhattanLength();
	if (dist < QApplication::startDragDistance()) return;

	if (tmrMarkerPos_){
		tmrMarkerPos_->stop();
		tmrMarkerPos_->deleteLater();
		tmrMarkerPos_ = NULL;
	}
	
	if (lpm_ && keyCntr_){

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
	else if (rpm_ && axisTime_ && axisValue_){
		
		axisTime_->mouseMoveEvent(event);
		axisValue_->mouseMoveEvent(event);
	}

	QWidget::mouseMoveEvent(event);
}

void wdgPlot::wheelEvent(QWheelEvent * event){

	if (axisTime_ && axisValue_){

		axisTime_->wheelEvent(event);
		axisValue_->wheelEvent(event);
	}
}

void wdgPlot::scale(int delta){
	
	if (axisTime_ && axisValue_){

		axisTime_->scale(delta);
		axisValue_->scale(delta);
	}
}

void wdgPlot::setAxisTime(wdgAxisTime* ax){

	axisTime_ = ax;

}

void wdgPlot::setAxisValue(wdgAxisValue* ax){

	axisValue_ = ax;

}

void wdgPlot::paintEvent(QPaintEvent *event){

	QPainter painter(this);

	QPen pn(Qt::green, Qt::DashLine);	pn.setWidth(2); 
	painter.setPen(pn);

	painter.drawRect(SelRect);
	
	QWidget::paintEvent(event);
}



