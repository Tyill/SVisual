
#pragma once

#include "stdafx.h"

class treeWidgetExt : public QTreeWidget
{
	Q_OBJECT


private:
	QPoint startMovePos_;


protected:
		
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
public:

	treeWidgetExt(QWidget *parent = 0);
	~treeWidgetExt();


};


