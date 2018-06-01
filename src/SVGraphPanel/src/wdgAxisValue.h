#pragma once

#include "stdafx.h"

class wdgAxisValue : public QWidget
{
	Q_OBJECT
private:

	int cng_dashHeight_ = 3;

	int curDashStep_ = 100;
	int cng_maxDashStep_ = 200;
	int cng_minDashStep_ = 100;
	int curOffsPos_ = 0;
	int curInterv_ = 0;

	QPair<double, double> valInterval_;

	double scale_ = 1.0;

	int mousePrevPosY_ = 0;

	
	void resizeEvent(QResizeEvent * event);

	void drawDashLines(QPainter& painter);
	void drawValMark(QPainter& painter);

	QString getValMark(int offs);


public:
	wdgAxisValue(QWidget *parent = 0);
	~wdgAxisValue();

	void setValInterval(double min, double max);

	QPair<double, double> getValInterval();

	double getValScale();

	QVector<int> getAxisMark();

	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);
	void scale(int delta);

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
	void req_axisChange();
};