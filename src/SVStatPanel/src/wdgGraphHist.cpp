
#include "stdafx.h"
#include "wdgGraphHist.h"

using namespace std;

wdgGraphHist::wdgGraphHist(QWidget *parent){
	
	QPalette Pal(palette());

	Pal.setColor(QPalette::Background, Qt::white);
	setAutoFillBackground(true);
	setPalette(Pal);
}
wdgGraphHist::~wdgGraphHist(){}

void wdgGraphHist::setGraphPnt(QVector<QPair<int, int>> pnts){

	grPnts_ = pnts;

	update();
}

void wdgGraphHist::setGraphPnt(vector<pair<int, int>> pnts){

	grPnts_.clear();

	int sz = pnts.size();
	for (int i = 0; i < sz; ++i)		
		grPnts_.push_back(QPair<int, int>(pnts[i].first, pnts[i].second));
	
	update();
}

void wdgGraphHist::paintEvent(QPaintEvent *event){
	
	if (grPnts_.isEmpty()) return;

	double maxX = INT32_MIN, maxY = INT32_MIN;
	
	minYPos_ = INT32_MAX;
	minXPos_ = INT32_MAX;

	int sz = grPnts_.size();
	for (int i = 0; i < sz; ++i){
		if (grPnts_[i].first > maxX) maxX = grPnts_[i].first;
		if (grPnts_[i].first < minXPos_) minXPos_ = grPnts_[i].first;

		if (grPnts_[i].second > maxY) maxY = grPnts_[i].second;
		if (grPnts_[i].second < minYPos_) minYPos_ = grPnts_[i].second;
	}
	sclX_ = width() / (maxX - minXPos_);
	sclY_ = height() / (maxY - minYPos_);
	
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(0, height());
	painter.scale(1.0, -1.0);
	
	
	painter.setPen(hColor_);
	
	for (int i = 0; i < sz; ++i) {
		int x1 = (grPnts_[i].first - minXPos_) * sclX_ + 0.5;		
		int y1 = 0;
		int y2 = (grPnts_[i].second - minYPos_) * sclY_ + 0.5; 
		if (y2 == 0) y2 = grPnts_[i].second * sclY_ / 2 + 0.5;
		painter.drawLine(x1, y1, x1, y2);
	}
	painter.setPen(Qt::GlobalColor::black);

	painter.drawLine(targPos_, 0, targPos_, height());
	painter.restore();
	//painter.setPen(QColor(0,0,0));
	
	//painter.drawText(QPoint(0, height()), QString::number(minX));
	//painter.drawText(QPoint(width() - 20, height()), QString::number(maxX));
}

void wdgGraphHist::mouseMoveEvent(QMouseEvent * event){

	targPos_ = event->pos().x();
		
}

void wdgGraphHist::mousePressEvent(QMouseEvent * event){

	targPos_ = event->pos().x();

	emit selValueChange();

	update();
}

void wdgGraphHist::setColor(QColor clr){

	hColor_ = clr;
}

void wdgGraphHist::setTargPos(QString pos){

	targPos_ = (pos.toInt() - minXPos_) * sclX_;

	emit selValueChange();

	update();
}

QPoint wdgGraphHist::getTargPos(){
	
	double vp = targPos_ / sclX_ + minXPos_;

	int val = vp > 0 ? vp + 0.5 : vp - 0.5, valHist = 0, sz = grPnts_.size();
	for (int i = 1; i < sz; ++i) {
		
		if ((grPnts_[i - 1].first <= val) && (val <= grPnts_[i].first)){
			
			vp = grPnts_[i].second / sclY_ + minYPos_;
			valHist = vp > 0 ? vp + 0.5 : vp - 0.5;
			break;
		}
	}

	return QPoint(val, valHist);
}

int wdgGraphHist::getAreaHist(int leftPos, int rightPos){

	int areaSum = 0, area = 0, sz = grPnts_.size();
	for (int i = 0; i < sz; ++i){
		areaSum += abs(grPnts_[i].second);
		if ((leftPos <= grPnts_[i].first) && (grPnts_[i].first <= rightPos)) area += abs(grPnts_[i].second);
	}

	return (area * 100.0) / areaSum;
}

int wdgGraphHist::getAreaByPos(){
		
	return getAreaHist(minXPos_, targPos_ / sclX_ + minXPos_);
}