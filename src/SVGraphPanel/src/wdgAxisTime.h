#pragma once

#include "stdafx.h"

class wdgAxisTime : public QWidget
{
	Q_OBJECT
private:
		
	int cng_dashHeight_ = 3;
	
	int curDashStep_ = 100;
	int curOffsPos_ = 0;
	int fontMetr_ = 0;
	int curIntervSec_ = 0;

	QPair<qint64, qint64> tmInterval_;
		
	double tmScale_ = 1.0;

	int mousePrevPosX_ = 0;

	
	void resizeEvent(QResizeEvent * event);

	void drawDashLines(QPainter& painter);
	void drawTimeMark(QPainter& painter);

	QString getTimeMark(int offs);


public:
	wdgAxisTime(QWidget *parent = 0);
	~wdgAxisTime();

	void setTimeInterval(qint64, qint64);

	QPair<qint64, qint64> getTimeInterval();

	double getTimeScale();

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