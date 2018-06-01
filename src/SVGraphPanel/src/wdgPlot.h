
#pragma once

#include "stdafx.h"
#include "wdgAxisTime.h"
#include "wdgAxisValue.h"

class wdgPlot : public QWidget
{
	Q_OBJECT

private:
	
	QPoint presPnt_;
		
	bool lpm_ = false, rpm_ = false, keyCntr_ = false;
	QTimer* tmrMarkerPos_ = NULL;

	wdgAxisTime* axisTime_ = NULL;
	wdgAxisValue* axisValue_ = NULL;

public:
	wdgPlot(QWidget *parent = 0);

	QRect SelRect;

	void setAxisTime(wdgAxisTime* ax);
	void setAxisValue(wdgAxisValue* ax);
	void scale(int delta);
	
protected:	
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent * event);

signals:
	void req_rctChange();
	void req_moveChange();
	void req_updMarker();
};