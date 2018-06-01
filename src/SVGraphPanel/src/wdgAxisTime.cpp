
#include "stdafx.h"
#include "wdgAxisTime.h"

wdgAxisTime::wdgAxisTime(QWidget *parent){
		
	setParent(parent);

	tmInterval_.first = QDateTime::currentDateTime().toMSecsSinceEpoch();
	tmInterval_.second = tmInterval_.first + 3600000;

	tmScale_ = (double)(tmInterval_.second - tmInterval_.first)/ width();

}

wdgAxisTime::~wdgAxisTime(){}

void wdgAxisTime::mouseMoveEvent(QMouseEvent * event){

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

void wdgAxisTime::mousePressEvent(QMouseEvent * event){

	mousePrevPosX_ = event->pos().x();
}

void wdgAxisTime::scale(int delta){
	
	if (delta > 0) curDashStep_++;
	else curDashStep_--;

	if (curDashStep_ > 3 * fontMetr_) curDashStep_ = 2 * fontMetr_;
	else if (curDashStep_ < fontMetr_ * 1.1) curDashStep_ = 2 * fontMetr_;

	int offs = 10000;

	if (curIntervSec_ > 86400) offs *= 1000;
	else if (curIntervSec_ > 3600) offs *= 100;
	else if (curIntervSec_ < 1) offs /= 1000;
	else if (curIntervSec_ < 60) offs /= 10;

	if (delta > 0){ // ����������

		tmInterval_.first += offs;
		tmInterval_.second += -offs;

		if (tmInterval_.first >= tmInterval_.second){

			qint64 mdl = abs(tmInterval_.second + tmInterval_.first) / 2;

			tmInterval_.first = mdl - 10;
			tmInterval_.second = mdl + 10;
		}
	}
	else{ // ������
		tmInterval_.first += -offs;
		tmInterval_.second += offs;
	}

	curIntervSec_ = (tmInterval_.second - tmInterval_.first) / 1000;

	tmScale_ = (double)(tmInterval_.second - tmInterval_.first) / width();

	emit req_axisChange();

}

void wdgAxisTime::wheelEvent(QWheelEvent * event){

	scale(event->delta());		
}

void wdgAxisTime::resizeEvent(QResizeEvent * event){

	tmScale_ = (double)(tmInterval_.second - tmInterval_.first) / width();

	emit req_axisChange();
}

void wdgAxisTime::drawDashLines(QPainter& painter){

	int w = width(), h = height();
	
	painter.setPen(Qt::gray);
	painter.drawLine(QPoint(0, 0), QPoint(w, 0));

	painter.setPen(Qt::black);
	int cWidth = curOffsPos_ % curDashStep_;
	
	while (cWidth < w){
				
		painter.drawLine(QPoint(cWidth, 0), QPoint(cWidth, cng_dashHeight_));

		cWidth += curDashStep_;
	}
}

QString wdgAxisTime::getTimeMark(int offs){

	QString timeMark = "";

	QDateTime dt = QDateTime::fromMSecsSinceEpoch(tmInterval_.first + tmScale_ * offs);

	timeMark = dt.time().toString("hh:mm:ss:zzz");
		
	if (curIntervSec_ > 86400) timeMark = dt.toString("dd.MM.yy hh:mm");
	else if (curIntervSec_ > 60) timeMark = dt.time().toString("hh:mm:ss");
		
	return timeMark;
}

void wdgAxisTime::drawTimeMark(QPainter& painter){

	int w = width(), h = height();

	int cWidth = curOffsPos_ % curDashStep_;
	
	while (cWidth < w){
		
		QString timeMark = getTimeMark(cWidth);

		fontMetr_ = this->fontMetrics().width(timeMark);

		painter.drawText(QPoint(cWidth - fontMetr_ / 2, h / 2), timeMark);
		
		cWidth += curDashStep_; 
	}
}

void wdgAxisTime::paintEvent(QPaintEvent *event){

	QPainter painter(this);
	
	drawDashLines(painter);

	drawTimeMark(painter);
	
}

void wdgAxisTime::setTimeInterval(qint64 beginTime, qint64 endTime){
	
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

QPair<qint64, qint64> wdgAxisTime::getTimeInterval(){

	return tmInterval_;

}

double wdgAxisTime::getTimeScale(){

	return tmScale_;

}

QVector<int> wdgAxisTime::getAxisMark(){

	QVector<int> mark;
	
	int w = width();

	int cWidth = curOffsPos_ % curDashStep_;

	while (cWidth < w){
		
		mark.push_back(cWidth);

		cWidth += curDashStep_;
	}

	return mark;
}