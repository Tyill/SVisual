#pragma once


class wdgGraphHist : public QWidget
{
	Q_OBJECT
private:

	QVector<QPair<int, int>> grPnts_;
	QColor hColor_ = QColor(127, 0, 127);

	int targPos_ = 0, minXPos_ = 0, minYPos_ = 0;

	double sclX_ = 0, sclY_ = 0;

	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);

public:
	wdgGraphHist(QWidget *parent = 0);
	~wdgGraphHist();

	void setGraphPnt(QVector<QPair<int,int>> pnts);
	void setGraphPnt(std::vector<std::pair<int, int>> pnts);
	void setColor(QColor clr);

	int getAreaHist(int leftPos, int rightPos);
	
	QPoint getTargPos();
	int getAreaByPos();

public slots:
	void setTargPos(QString pos);

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
	void selValueChange();

};