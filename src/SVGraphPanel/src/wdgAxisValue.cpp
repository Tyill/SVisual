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
#include "wdgAxisValue.h"

wdgAxisValue::wdgAxisValue(QWidget *parent){

	setParent(parent);

	valInterval_.first = 0;
	valInterval_.second = 1000;

	scale_ = (valInterval_.second - valInterval_.first) / height();

}

wdgAxisValue::~wdgAxisValue(){}

void wdgAxisValue::mouseMoveEvent(QMouseEvent * event){

    if (!axisAttr_.isAuto) return;

	int pos = event->pos().y(), diff = pos - mousePrevPosY_;

	curOffsPos_ += diff;

	if (curOffsPos_ > curDashStep_) curOffsPos_ = 0;
	if (curOffsPos_ < 0) curOffsPos_ = curDashStep_;

	valInterval_.first += scale_ * diff;
	valInterval_.second += scale_ * diff;

    curInterv_ = abs(valInterval_.second - valInterval_.first);

	mousePrevPosY_ = pos;

	emit req_axisChange();

}

void wdgAxisValue::mousePressEvent(QMouseEvent * event){

	mousePrevPosY_ = event->pos().y();
}

void wdgAxisValue::scale(int delta){
		
    if (!axisAttr_.isAuto) return;

	if (delta > 0) curDashStep_++;
	else curDashStep_--;

	if (curDashStep_ > cng_maxDashStep_) curDashStep_ = cng_minDashStep_;
	else if (curDashStep_ < cng_minDashStep_) curDashStep_ = cng_maxDashStep_;

	int offs = 10;

	if (curInterv_ > 1000) offs *= 10;
	else if (curInterv_ > 10000) offs *= 100;
	else if (curInterv_ < 100) offs /= 10;

	if (delta > 0){ 
		valInterval_.first += offs;
		valInterval_.second -= offs;

		if (valInterval_.first >= valInterval_.second) valInterval_.first = valInterval_.second - 0.1;
	}
	else{ 
		valInterval_.first -= offs;
		valInterval_.second += offs;
	}

	curInterv_ = abs(valInterval_.second - valInterval_.first);

	scale_ = (valInterval_.second - valInterval_.first) / height();

	emit req_axisChange();
}

void wdgAxisValue::setAxisAttr(const SV_Graph::axisAttr& attr){
    
    if (axisAttr_.isAuto && attr.isAuto) return;

    valInterval_.first = attr.min;
    valInterval_.second = attr.max;

    if (abs(attr.max - attr.min) < 0.000001)
        valInterval_.first = attr.max - 0.000001;

    curInterv_ = abs(valInterval_.second - valInterval_.first);

    scale_ = (valInterval_.second - valInterval_.first) / height();

    curDashStep_ = qMax(int(abs(attr.step) / scale_ + 0.5), 1);
    curOffsPos_ = curDashStep_;

    axisAttr_ = attr;
}

SV_Graph::axisAttr wdgAxisValue::getAxisAttr(){

    axisAttr_.min = valInterval_.first;
    axisAttr_.max = valInterval_.second;

    if (axisAttr_.isAuto)
        axisAttr_.step = curDashStep_ * scale_;

    return axisAttr_;
}

void wdgAxisValue::wheelEvent(QWheelEvent * event){

	scale(event->delta());

}

void wdgAxisValue::resizeEvent(QResizeEvent * event){

	scale_ = (valInterval_.second - valInterval_.first) / height();
    
    if (!axisAttr_.isAuto){
        curDashStep_ = qMax(int(abs(axisAttr_.step) / scale_ + 0.5), 1);
        curOffsPos_ = curDashStep_;
    }

    emit req_axisChange();
}


void wdgAxisValue::setValInterval(double minv, double maxv){

    if (!axisAttr_.isAuto) return;

    if (minv != maxv){
        valInterval_.first = minv;
        valInterval_.second = maxv;
    }
    else{
        valInterval_.first = minv - 0.1;
        valInterval_.second = maxv + 0.1;
    }

    curInterv_ = abs(valInterval_.second - valInterval_.first);

    scale_ = (valInterval_.second - valInterval_.first) / height();
}

QPair<double, double> wdgAxisValue::getValInterval(){

    return valInterval_;

}

double wdgAxisValue::getValScale(){

    return scale_;
}

QVector<int> wdgAxisValue::getAxisMark(){

    QVector<int> mark;

    int h = height();

    int cHeight = curOffsPos_ % curDashStep_;

    while (cHeight < h){

        mark.push_back(cHeight);

        cHeight += curDashStep_;
    }

    return mark;
}

void wdgAxisValue::paintEvent(QPaintEvent *event){

	QPainter painter(this);

	drawDashLines(painter);

	drawValMark(painter);
}

void wdgAxisValue::drawDashLines(QPainter& painter){

    int w = width(), h = height();

    painter.setPen(Qt::gray);
    painter.drawLine(QPoint(w - 1, 0), QPoint(w - 1, h));

    painter.setPen(Qt::black);
    int cHeight = curOffsPos_ % curDashStep_;

    while (cHeight < h){

        painter.drawLine(QPoint(w - cng_dashHeight_, cHeight), QPoint(w, cHeight));

        cHeight += curDashStep_;
    }
}

void wdgAxisValue::drawValMark(QPainter& painter){

    int w = width(), h = height();

    int cHeight = curOffsPos_ % curDashStep_;

    double cHeightVal = axisAttr_.max;

    while (cHeight < h){

        double vl = valInterval_.second - scale_ * cHeight;
        if (!axisAttr_.isAuto)
            vl = cHeightVal;

        QString valMark = getValMark(vl);

        int fontMetr = this->fontMetrics().width(valMark),
            xp = std::max(0, w - fontMetr - 15);

        painter.drawText(QPoint(xp, cHeight), valMark);

        cHeight += curDashStep_;
        cHeightVal -= axisAttr_.step;
    }
}

QString wdgAxisValue::getValMark(double vl){
       
    int sign = vl > 0 ? 1 : -1;

    vl *= sign;

    double diap = abs(valInterval_.second - valInterval_.first) / 2;

    if (diap > 1.0)       vl = int(vl + 0.5);
    else if (diap > 0.1)  vl = int(vl * 10 + 0.5) / 10.;
    else if (diap > 0.01) vl = int(vl * 100 + 0.5) / 100.;
    else                  vl = int(vl * 1000 + 0.5) / 1000.;

    return QString::number(vl * sign);
}
