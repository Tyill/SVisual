#pragma once


class wdgMarker : public QWidget
{
	Q_OBJECT
private:
		
	int minPosX_ = -1, maxPosX_ = -1, minPosY_ = -1, maxPosY_ = -1;
	int presPosX_ = 0;
	
public:
	wdgMarker(QWidget *parent = 0);
	~wdgMarker();

	bool IsSelect = false;

	void setPos(QPoint pos);
	
	void setLimitPosX(int min, int max);
	void setLimitPosY(int min, int max);
		
protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

signals:
	void req_markerChange();

};