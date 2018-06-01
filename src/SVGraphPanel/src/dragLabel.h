
#pragma once

#include "stdafx.h"

class dragLabel : public QLabel
{
	Q_OBJECT

private:
	QPoint startMovePos_;
	QString sign_;

public:
	dragLabel(QWidget *parent = 0);

	void setSignal(QString sign);
protected:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:
	void req_delSignal(QString sign);
};