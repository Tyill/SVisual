
#include "stdafx.h"
#include "forms/graphPanel.h"
#include "forms/wdgGraph.h"
#include "wdgAxisTime.h"
#include "wdgAxisValue.h"
#include "wdgMarker.h"
#include "dragLabel.h"
#include "wdgPlot.h"
#include "SVConfig/SVConfigLimits.h"
#include "SVConfig/SVConfigData.h"

using namespace SV_Cng;

wdgGraph::wdgGraph(QWidget *parent, SV_Graph::config cng_){

#ifdef SV_EN
	QTranslator translator;
	translator.load(":/SVGp/svgraphpanel_en.qm");
	QCoreApplication::installTranslator(&translator);
#endif

	setParent(parent);

	ui.setupUi(this);
			
	setAcceptDrops(true);
		
	ui.wPlot->installEventFilter(this);
	ui.wPlot->setAxisValue(ui.wAxisValue);

    leftMarker_ = new wdgMarker(ui.wPlot);
	rightMarker_ = new wdgMarker(ui.wPlot);
				

	rightMarker_->setPos(QPoint(400,0));

	grPanel_ = (graphPanel*)parent;
	cng = cng_;
		
	connect(ui.wAxisValue, SIGNAL(req_axisChange()), this, SLOT(axisValueChange()));
	connect(ui.btnClose, &QPushButton::clicked, this, [this] {

		emit req_close();
		close();
		
	});

	connect(ui.btnUp, &QPushButton::clicked, this, [this] {

		emit req_graphUp(this->objectName());
		
	});
	connect(ui.btnDn, &QPushButton::clicked, this, [this] {

		emit req_graphDn(this->objectName());

	});

	
	connect(ui.wPlot, SIGNAL(req_rctChange()), this, SLOT(resizeByRect()));
    connect(ui.wPlot, &wdgPlot::req_updMarker, this, &wdgGraph::showMarkPos);
	connect(leftMarker_, SIGNAL(req_markerChange()), this, SLOT(updateByMarker()));
	connect(rightMarker_, SIGNAL(req_markerChange()), this, SLOT(updateByMarker()));
	
}

wdgGraph::~wdgGraph(){}

QSize wdgGraph::sizeHint(){

	return this->size();
}

void wdgGraph::resizeEvent(QResizeEvent * event){

	leftMarker_->setLimitPosX(0, ui.wPlot->width() - rightMarker_->width());
	leftMarker_->setLimitPosY(ui.wPlot->height() - rightMarker_->height(), ui.wPlot->height() - rightMarker_->height());
	
	rightMarker_->setLimitPosX(0, ui.wPlot->width() - rightMarker_->width());
	rightMarker_->setLimitPosY(ui.wPlot->height() - rightMarker_->height(), ui.wPlot->height() - rightMarker_->height());

	repaintEna_ = true;

	QWidget::resizeEvent(event);
}

void wdgGraph::addPosToHistory(){

	QPair<double, double> valIntl = ui.wAxisValue->getValInterval();
	
	QPair<qint64, qint64> tmIntl = axisTime_->getTimeInterval();

	historyPos_.push_back(histPos{ valIntl, tmIntl });
}

void wdgGraph::paintSignals(){

	int h = ui.wPlot->height(), w = ui.wPlot->width();
		
	imSign_ = QImage(ui.wPlot->size(), QImage::Format_RGB888);
	imSign_.fill(Qt::white);

	QPainter painter;
	painter.begin(&imSign_);

	painter.translate(0, h);
	painter.scale(1.0, -1.0);

	QPen pn; pn.setColor(Qt::gray); pn.setWidthF(0.2);
	painter.setPen(pn);

	QVector<int> axisMarkX = axisTime_->getAxisMark();
	QVector<int> axisMarkY = ui.wAxisValue->getAxisMark();

	// axisX
	int sz = axisMarkX.size();
	for (int i = 0; i < sz; ++i){
		painter.drawLine(axisMarkX[i], 0, axisMarkX[i], h);
	}

	// axisY
	sz = axisMarkY.size();
	for (int i = 0; i < sz; ++i){
		painter.drawLine(0, h - axisMarkY[i], w, h - axisMarkY[i]);
	}

	QPair<qint64, qint64> tmInterv = axisTime_->getTimeInterval();

	bool paintPnt = (tmInterv.second - tmInterv.first) < SV_CYCLESAVE_MS;

	// �������
	int signBoolCnt = 0;
	auto sref = grPanel_->pfGetCopySignalRef();
	for (auto& s : signalListAlter_) if (sref[s]->type == valueType::tBool) ++signBoolCnt;
	for (int s = signalList_.size() - 1; s >= 0; --s){

		auto& sign = signals_[signalList_[s]];

		
		painter.setBrush(sign.color);
		painter.setPen(sign.color);
		
		if (sign.type != valueType::tBool){

		int znSz = sign.pnts.size();
			for (int z = 0; z < znSz; ++z){

				QVector<QPair<int, int>>& zonePnts = sign.pnts[z];

				int sz = zonePnts.size();
												
				for (int i = 1; i < sz; ++i){
					painter.drawLine(zonePnts[i - 1].first, zonePnts[i - 1].second, zonePnts[i].first, zonePnts[i].second);
										
				}
								
				if (paintPnt){
					QPen ptPen(sign.color);
					ptPen.setCapStyle(Qt::RoundCap);
					ptPen.setWidth(10);
					painter.setPen(ptPen);
					for (int i = 0; i < sz; ++i){
						painter.drawPoint(zonePnts[i].first, zonePnts[i].second);
					}
				}
			}
		}
		else{

			int znSz = sign.pnts.size(), sDist = 15, sH = 10;
			for (int z = 0; z < znSz; ++z){

				QVector<QPair<int, int>>& zonePnts = sign.pnts[z];

				int sz = zonePnts.size(), prevPos = 1, prevVal = zonePnts[0].second;
				for (int i = 1; i < sz; ++i){

					if ((prevVal == 0) && ((zonePnts[i].second > 0) || (i == (sz - 1)))){
						painter.drawLine(zonePnts[prevPos - 1].first, signBoolCnt * sDist + 1, zonePnts[i].first, signBoolCnt * sDist + 1);

						prevVal = 1;
						prevPos = i;
					}
					else if ((prevVal > 0) && ((zonePnts[i].second == 0) || (i == (sz - 1)))){
						
						painter.drawRect(QRect(QPoint(zonePnts[prevPos - 1].first, signBoolCnt * sDist + 1),
							QPoint(zonePnts[i - 1].first, signBoolCnt * sDist + 1 + sH)));
												
						prevVal = 0;
						prevPos = i;
					}

				}
			}
			++signBoolCnt;
		}

	}
	painter.end();

}

void wdgGraph::paintSignalsAlter(){

	int h = ui.wPlot->height(), w = ui.wPlot->width();
		
	QPainter painter;
	painter.begin(&imSign_);

	painter.translate(0, h);
	painter.scale(1.0, -1.0);

	QPen pn; pn.setColor(Qt::gray); pn.setWidthF(0.2);
	painter.setPen(pn);

	
	QPair<qint64, qint64> tmInterv = axisTime_->getTimeInterval();

	bool paintPnt = (tmInterv.second - tmInterv.first) < SV_CYCLESAVE_MS;

	// �������
	int signBoolCnt = 0;
	for (int s = signalListAlter_.size() - 1; s >= 0; --s){

		auto& sign = signalsAlter_[signalListAlter_[s]];

		painter.setBrush(sign.color);
		painter.setPen(sign.color);

		if (sign.type != valueType::tBool){
			int znSz = sign.pnts.size();
			for (int z = 0; z < znSz; ++z){

				QVector<QPair<int, int>>& zonePnts = sign.pnts[z];

				int sz = zonePnts.size();

				for (int i = 1; i < sz; ++i){
					painter.drawLine(zonePnts[i - 1].first, zonePnts[i - 1].second, zonePnts[i].first, zonePnts[i].second);
				}

				
				if (paintPnt){
					QPen ptPen(sign.color);
					ptPen.setCapStyle(Qt::RoundCap);
					ptPen.setWidth(10);
					painter.setPen(ptPen);
					for (int i = 0; i < sz; ++i){
						painter.drawPoint(zonePnts[i].first, zonePnts[i].second);
					}
				}
			}
		}
		else{

			int znSz = sign.pnts.size(), sDist = 15, sH = 10;
			for (int z = 0; z < znSz; ++z){

				QVector<QPair<int, int>>& zonePnts = sign.pnts[z];

				int sz = zonePnts.size(), prevPos = 1, prevVal = zonePnts[0].second; 
				for (int i = 1; i < sz; ++i){

					if ((prevVal == 0) && ((zonePnts[i].second > 0) || (i == (sz - 1)))){
						painter.drawLine(zonePnts[prevPos - 1].first, signBoolCnt * sDist + 1, zonePnts[i].first, signBoolCnt * sDist + 1);

						prevVal = 1;
						prevPos = i;
					}
					else if ((prevVal > 0) && ((zonePnts[i].second == 0) || (i == (sz - 1)))){
												
						painter.drawRect(QRect(QPoint(zonePnts[prevPos - 1].first, signBoolCnt * sDist + 1),
							                   QPoint(zonePnts[i - 1].first, signBoolCnt * sDist + 1 + sH)));
						
						prevVal = 0;
						prevPos = i;
					}

				}
			}
			++signBoolCnt;
		}

	}
	painter.end();

}

void wdgGraph::paintObjects(){

	int h = ui.wPlot->height(), w = ui.wPlot->width();

	QPainter painter(ui.wPlot);

	painter.drawImage(QPoint(0, 0), imSign_);
		
	//painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(0, h);
	painter.scale(1.0, -1.0);

	painter.setPen(Qt::red);

	// leftMarker
	int mLeftPosX = leftMarker_->pos().x() + leftMarker_->width() / 2;
	painter.drawLine(mLeftPosX, 0, mLeftPosX, h);

	//// rightMarker
	int mRightPosX = rightMarker_->pos().x() + rightMarker_->width() / 2;
	painter.drawLine(mRightPosX, 0, mRightPosX, h);


	QPair<qint64, qint64 > tmIntl = axisTime_->getTimeInterval();
	double tmScale = axisTime_->getTimeScale();

	if (leftMarker_->IsSelect || selLeftMark_){
		selLeftMark_ = false;
		QDateTime dtm = QDateTime::fromMSecsSinceEpoch(tmIntl.first + mLeftPosX*tmScale);

		QToolTip::showText(this->cursor().pos(), dtm.toString("dd.MM.yy hh:mm:ss:zzz"), this);
	}
	auto sValuePnt = getSignalValueByMarkerPos(mLeftPosX);

	for (auto s : sValuePnt){

		if (s.type != valueType::tBool){

			signals_[s.sign].lbLeftMarkVal->move(QPoint(s.xPix + 2, ui.wPlot->height() - s.yPix - 22));
			signals_[s.sign].lbLeftMarkVal->setText(getSValue(s.type, s.val).c_str());
			signals_[s.sign].lbLeftMarkVal->show();
		}
	}
	

	if (rightMarker_->IsSelect || selRigthMark_){
		selRigthMark_ = false;
		QDateTime dtm = QDateTime::fromMSecsSinceEpoch(tmIntl.first + mRightPosX*tmScale);

		QToolTip::showText(this->cursor().pos(), dtm.toString("dd.MM.yy hh:mm:ss:zzz"), this);
	}

	sValuePnt = getSignalValueByMarkerPos(mRightPosX);
	for (auto s : sValuePnt){

		if (s.type != valueType::tBool){

			signals_[s.sign].lbRightMarkVal->move(QPoint(s.xPix + 2, ui.wPlot->height() - s.yPix - 22));
			signals_[s.sign].lbRightMarkVal->setText(getSValue(s.type, s.val).c_str());
			signals_[s.sign].lbRightMarkVal->show();
		}
	}
}

void wdgGraph::paintObjectsAlter(){
		
	// leftMarker
	int mLeftPosX = leftMarker_->pos().x() + leftMarker_->width() / 2;

	//// rightMarker
	int mRightPosX = rightMarker_->pos().x() + rightMarker_->width() / 2;

	QPair<qint64, qint64 > tmIntl = axisTime_->getTimeInterval();
	double tmScale = axisTime_->getTimeScale();

	if (leftMarker_->IsSelect || selLeftMark_){
		selLeftMark_ = false;

		QDateTime dtm = QDateTime::fromMSecsSinceEpoch(tmIntl.first + mLeftPosX*tmScale);

		QToolTip::showText(this->cursor().pos(), dtm.toString("dd.MM.yy hh:mm:ss:zzz"), this);
	}
	auto sValuePnt = getSignalAlterValueByMarkerPos(mLeftPosX);
	for (auto s : sValuePnt){

		if (s.type != valueType::tBool){

			signalsAlter_[s.sign].lbLeftMarkVal->move(QPoint(s.xPix + 2, ui.wPlot->height() - s.yPix - 22));
			signalsAlter_[s.sign].lbLeftMarkVal->setText(getSValue(s.type, s.val).c_str());
			signalsAlter_[s.sign].lbLeftMarkVal->show();
		}
	}

	if (rightMarker_->IsSelect || selRigthMark_){
		selRigthMark_ = false;

		QDateTime dtm = QDateTime::fromMSecsSinceEpoch(tmIntl.first + mRightPosX*tmScale);

		QToolTip::showText(this->cursor().pos(), dtm.toString("dd.MM.yy hh:mm:ss:zzz"), this);
	}

	sValuePnt = getSignalAlterValueByMarkerPos(mRightPosX);
	for (auto s : sValuePnt){

		if (s.type != valueType::tBool){

			signalsAlter_[s.sign].lbRightMarkVal->move(QPoint(s.xPix + 2, ui.wPlot->height() - s.yPix - 22));
			signalsAlter_[s.sign].lbRightMarkVal->setText(getSValue(s.type, s.val).c_str());
			signalsAlter_[s.sign].lbRightMarkVal->show();
		}
	}

}

bool wdgGraph::eventFilter(QObject *target, QEvent *event){

	if ((target->objectName() == "wPlot") && (event->type() == QEvent::Paint)){
		
		if (repaintEna_){
			paintSignals();
			if (!signalListAlter_.isEmpty()) paintSignalsAlter();
		}
		
		paintObjects();
		if (!signalListAlter_.isEmpty()) paintObjectsAlter();


		repaintEna_ = false;				
	}

	if (event->type() == QEvent::MouseButtonPress){
				
		emit req_selectGraph(this->objectName());
			
	}

	return QWidget::eventFilter(target, event);
}

void wdgGraph::addSignal(QString sign){
	
	if (!signals_.contains(sign)){
				
		int num = signals_.size();

		colorCnt_ += 30;
		QColor clr = QColor((num * (60 + colorCnt_)) % 255, (num * (120 + colorCnt_)) % 255, (num * (180 + colorCnt_)) % 255, 255);

		dragLabel* lb = new dragLabel(this);
        QLabel* lbLeftVal = new QLabel(ui.wPlot);
		QLabel* lbRightVal = new QLabel(ui.wPlot);

		signalData* sd = grPanel_->pfGetSignalData(sign);

		signals_.insert(sign, graphSignData{ sign, sd->name.c_str(), sd->type, num, clr, lb, lbLeftVal, lbRightVal });
		signalList_.push_front(sign);

		QPalette palette = lb->palette();
		palette.setColor(lb->foregroundRole(), clr);
		lb->setPalette(palette);
		lb->setText((sd->name + " " + sd->comment).c_str());
		lb->setSignal(sign);
		ui.vLayoutSignName->insertWidget(0,lb);
        lbLeftVal->setPalette(palette);
		lbRightVal->setPalette(palette);

		connect(lb, SIGNAL(req_delSignal(QString)), this, SLOT(delSignal(QString)));
		
		// ��� ��������
		if (sd->type != valueType::tBool){

			if (sd->buffMinValue < sd->buffMaxValue)
				ui.wAxisValue->setValInterval(sd->buffMinValue - 1, sd->buffMaxValue + 1);
			else
				ui.wAxisValue->setValInterval(-100, 100);
		}
		
		axisValueChange();

		addPosToHistory();	
	}
}

void wdgGraph::addAlterSignal(QString sign){

	if (!signalsAlter_.contains(sign)){

		int num = signalsAlter_.size() + 1;

		colorCnt_ += 30;
		QColor clr = QColor((num * (60 + colorCnt_)) % 255, (num * (120 + colorCnt_)) % 255, (num * (180 + colorCnt_)) % 255, 255);

		dragLabel* lb = new dragLabel(this);
		QLabel* lbLeftVal = new QLabel(ui.wPlot);
		QLabel* lbRightVal = new QLabel(ui.wPlot);

		signalData* sd = grPanel_->pfGetSignalData(sign);

		signalsAlter_.insert(sign, graphSignData{ sign, sd->name.c_str(), sd->type, num, clr, lb, lbLeftVal, lbRightVal });
		signalListAlter_.push_front(sign);

		QPalette palette = lb->palette();
		palette.setColor(lb->foregroundRole(), clr);
		lb->setPalette(palette);
		lb->setText((sd->name + " " + sd->comment).c_str());
		lb->setSignal(sign);
		ui.vLayoutAuxSignName->insertWidget(0, lb);
		lbLeftVal->setPalette(palette);
		lbRightVal->setPalette(palette);

		connect(lb, SIGNAL(req_delSignal(QString)), this, SLOT(delSignalAlter(QString)));

		axisTimeChange();
				
		addPosToHistory();
	}

}

QStringList wdgGraph::getAllSignals(){

	return signalList_;
}

QStringList wdgGraph::getAllAlterSignals(){


	return signalListAlter_;
}
void wdgGraph::setAxisTime(wdgAxisTime* axisTime){

	axisTime_ = axisTime;

	ui.wPlot->setAxisTime(axisTime);

}

void wdgGraph::dragEnterEvent(QDragEnterEvent *event)
{
	if (qobject_cast<QTreeWidget *>(event->source()) ||
		qobject_cast<dragLabel *>(event->source())) {

		event->accept();
	}
}

void wdgGraph::dragMoveEvent(QDragMoveEvent *event){

	if (qobject_cast<QTreeWidget *>(event->source()) ||
		qobject_cast<dragLabel *>(event->source())) {

		event->accept();
	}
}

void wdgGraph::dropEvent(QDropEvent *event)
{
	dragLabel* lb = qobject_cast<dragLabel *>(event->source());

	if (qobject_cast<QTreeWidget *>(event->source()) || lb) {
		
		QString sign = event->mimeData()->text();

		if (!sign.isEmpty()){

			auto sd = grPanel_->pfGetSignalData(sign);

			if (sd && !sd->isBuffEnable && grPanel_->pfLoadSignalData)
				grPanel_->pfLoadSignalData(sign);

			if (lb){

				if (signals_.contains(sign)){
					delSignal(sign, false);
					addAlterSignal(sign);
				}
				else{
					delSignalAlter(sign, false);
					addSignal(sign);
				}				
			}
			else addSignal(sign);
	
	
	
			emit req_markerChange(this->objectName());
		}	

		event->accept();
	}
}

void wdgGraph::setMarkersPos(QPoint left, QPoint right){

	left.setX(left.x() - leftMarker_->width() / 2);
	right.setX(right.x() - rightMarker_->width() / 2);

	leftMarker_->setPos(left);
	rightMarker_->setPos(right);
}

void wdgGraph::getMarkersPos(QPoint& left, QPoint& right){

	left = leftMarker_->pos(); left.setX(left.x() + leftMarker_->width() / 2);
	right = rightMarker_->pos(); right.setX(right.x() + rightMarker_->width() / 2);
}

QVector<QVector<QPair<int, int>>> wdgGraph::getSignalPnt(signalData* sign, bool isAlter){
		
	double tmScale = axisTime_->getTimeScale(); 
	double valScale = ui.wAxisValue->getValScale(); 

	QPair<qint64, qint64> tmInterval = axisTime_->getTimeInterval();
	QPair<double, double> valInterval = ui.wAxisValue->getValInterval();
	
	double valMinInterval = valInterval.first, valMaxInterval = valInterval.second;

	if (isAlter){
		valInterval = getSignMaxMinValue(sign, tmInterval);
		valMinInterval = valInterval.first - 1, valMaxInterval = valInterval.second + 3;
		valScale = (valMaxInterval - valMinInterval) / ui.wPlot->height();
	}

    if (!sign->isBuffEnable && grPanel_->pfLoadSignalData)
    	grPanel_->pfLoadSignalData(QString::fromStdString(sign->name + sign->module));

	int znSz = sign->buffData.size();

	if (znSz == 0) return QVector<QVector<QPair<int, int>>>();
		
	uint64_t tmZnBegin = sign->buffMinTime,
		tmZnEnd = sign->buffMaxTime,
		tmMinInterval = tmInterval.first,
		tmMaxInterval = tmInterval.second;
	if ((tmZnBegin >= tmMaxInterval) || (tmZnEnd <= tmMinInterval)) return QVector<QVector<QPair<int, int>>>();
			
	int z = sign->buffBeginPos;
		
	QVector<QVector<QPair<int, int>>> zonePnts(1);
	zonePnts.back().reserve(axisTime_->width());
		
	int prevPos = 0, endPos = sign->buffValuePos;
	uint64_t tmZnEndPrev = sign->buffData[z].beginTime;
	
	tmZnEnd = tmZnBegin + SV_CYCLESAVE_MS;
		
	QVector<double> tmPosMem;
	for (int i = 0; i < SV_PACKETSZ; ++i)
        tmPosMem.push_back((i * SV_CYCLEREC_MS - double(tmMinInterval)) / tmScale);

	double valPosMem = valMinInterval / valScale;
	double tmZnBeginMem = double(tmZnBegin) / tmScale;
	
	QPair<int , int> pnt;
	int valMem = 0, backVal = 0, prevBackVal = 0, backValInd = -1, prevBackValInd = -2;
	bool isChange = false;
    while (tmZnBegin < tmMaxInterval){

		if (tmZnEnd > tmMinInterval){
			recData& rd = sign->buffData[z];

          	if (int(tmZnBegin - tmZnEndPrev) > SV_CYCLESAVE_MS){
				zonePnts.push_back(QVector<QPair<int, int>>());
				isChange = false;
				backValInd = -1;
				prevBackValInd = -2;
			}

			tmZnBeginMem = double(tmZnBegin) / tmScale;
            auto& backZone = zonePnts.back();

			for (int i = 0; i < SV_PACKETSZ; ++i){
				pnt.first = tmPosMem[i] + tmZnBeginMem;

                switch (sign->type)	{
				case valueType::tInt: pnt.second = rd.vals[i].tInt / valScale - valPosMem; break;
				case valueType::tBool: pnt.second = rd.vals[i].tBool; break;
				case valueType::tFloat: pnt.second = rd.vals[i].tFloat / valScale - valPosMem; break;
				}

                if (pnt.first != prevPos){
					isChange = false;
					prevPos = pnt.first;

                    backZone.push_back(pnt);
					++backValInd;
					++prevBackValInd;
				}
				else if (pnt.second != valMem){
					valMem = pnt.second;
						
					if (!isChange){
                        backZone.push_back(pnt);
						++backValInd;
						++prevBackValInd;
								
						if (prevBackValInd >= 0){
							isChange = true;
							backVal = backZone[backValInd].second;
							prevBackVal = backZone[prevBackValInd].second;
						}
					}
					else{
						if (prevBackVal <= backVal){
							if (valMem < prevBackVal){
								prevBackVal = valMem;
                                backZone[prevBackValInd].second = valMem;
							}
							else if (valMem > backVal){
								backVal = valMem;
                                backZone[backValInd].second = valMem;
							}
						}
						else{
							if (valMem > prevBackVal){
								prevBackVal = valMem;
                                backZone[prevBackValInd].second = valMem;
							}
							else if (valMem < backVal){
								backVal = valMem;
                                backZone[backValInd].second = valMem;
							}
						}
					}
				}
			}
		}
		tmZnEndPrev = tmZnEnd;

		++z; if (z >= znSz) z = 0;

		if (z != endPos){
			tmZnBegin = sign->buffData[z].beginTime;
			tmZnEnd = tmZnBegin + SV_CYCLESAVE_MS;
		}
		else break;
	}
	
	int sz = zonePnts.size();
	for (int i = 0; i < sz; ++i){
		if (zonePnts[i].isEmpty()){
			zonePnts.remove(i);
			i--; sz--;
		}
	}

	return zonePnts;
}

QPair<double, double> wdgGraph::getSignMaxMinValue(graphSignData* sign){

	QVector<QVector<QPair<int, int>>>& grPnts = sign->pnts;
		
	int gSz = grPnts.size(); double minVal = INT32_MAX, maxVal = -INT32_MAX;
	for (int g = 0; g < gSz; ++g){

		int sz = grPnts[g].size();
		for (int i = 0; i < sz; ++i){

			if (grPnts[g][i].second < minVal) minVal = grPnts[g][i].second;
			if (grPnts[g][i].second > maxVal) maxVal = grPnts[g][i].second;
		}
	}

	if ((minVal == INT32_MAX) || (maxVal == -INT32_MAX)) {
		minVal = 0;
		maxVal = 1;
	}
	return QPair<double, double> (minVal, maxVal);
}

QPair<double, double> wdgGraph::getSignMaxMinValue(signalData* sign, QPair<qint64, qint64>& tmInterval){
	
	uint64_t tmZnBegin = sign->buffMinTime,
		tmZnEnd = sign->buffMaxTime,
		tmMinInterval = tmInterval.first,
		tmMaxInterval = tmInterval.second;

	if ((tmZnBegin >= tmMaxInterval) || (tmZnEnd <= tmMinInterval)) return QPair<double, double >(0,1);

	auto rdata = sign->buffData;
		
	int znSz = rdata.size(), z = 0; double minVal = INT32_MAX, maxVal = -INT32_MAX;

	switch (sign->type)
	{
		case valueType::tInt:
						
			while (tmZnBegin < tmMaxInterval){

				if (tmZnEnd > tmMinInterval){

					for (int i = 0; i < SV_PACKETSZ; ++i){

						if (rdata[z].vals[i].tInt < minVal) minVal = rdata[z].vals[i].tInt;
						if (rdata[z].vals[i].tInt > maxVal) maxVal = rdata[z].vals[i].tInt;
					}
				}

				++z;

				if (z < znSz){
					tmZnBegin = sign->buffData[z].beginTime,
					tmZnEnd = sign->buffData[z].beginTime + SV_CYCLESAVE_MS;
				}
				else break;
			}
			
			break;
		case valueType::tFloat:
			while (tmZnBegin < tmMaxInterval){

				if (tmZnEnd > tmMinInterval){

					for (int i = 0; i < SV_PACKETSZ; ++i){

						if (rdata[z].vals[i].tFloat < minVal) minVal = rdata[z].vals[i].tFloat;
						if (rdata[z].vals[i].tFloat > maxVal) maxVal = rdata[z].vals[i].tFloat;
					}
				}

				++z;

				if (z < znSz){
					tmZnBegin = sign->buffData[z].beginTime,
					tmZnEnd = sign->buffData[z].beginTime + SV_CYCLESAVE_MS;
				}
				else break;
			}
			break;
	default:
		break;
	}
	

	return QPair<double, double>(minVal, maxVal);
}

void wdgGraph::plotUpdate(){

	if (axisTime_){
					
		auto sref = grPanel_->pfGetCopySignalRef();
		for (auto it = signals_.begin(); it != signals_.end(); ++it){
						
			it->pnts = getSignalPnt(sref[it->sign]);
			
		}
		
		for (auto it = signalsAlter_.begin(); it != signalsAlter_.end(); it++){

			it->pnts = getSignalPnt(sref[it->sign], true);

		}
		
		repaintEna_ = true;

		ui.wPlot->update();
		
	}
}

void wdgGraph::axisTimeChange(){
			
	if (axisTime_){
		
		auto sref = grPanel_->pfGetCopySignalRef();
		
		for (auto it = signals_.begin(); it != signals_.end(); it++){
			it->pnts = getSignalPnt(sref[it->sign]);
		}
		
		for (auto it = signalsAlter_.begin(); it != signalsAlter_.end(); it++){
			it->pnts = getSignalPnt(sref[it->sign], true);
		}
		
		repaintEna_ = true;

		axisTime_->update();
		ui.wPlot->update();		
	}	
}

void wdgGraph::axisValueChange(){
	
	if (axisTime_){
		auto sref = grPanel_->pfGetCopySignalRef();

		for (auto it = signals_.begin(); it != signals_.end(); it++){
			it->pnts = getSignalPnt(sref[it->sign]);
		}

		for (auto it = signalsAlter_.begin(); it != signalsAlter_.end(); it++){
			it->pnts = getSignalPnt(sref[it->sign], true);
		}
		
		repaintEna_ = true;

		ui.wAxisValue->update();
		ui.wPlot->update();
		
	}
}

void wdgGraph::delSignal(QString sign, bool isLabelSender){

	if (signals_.contains(sign)){
		
		if (isLabelSender) sender()->deleteLater();
		else signals_[sign].lb->deleteLater();
		
		for (auto it = signalList_.begin(); it != signalList_.end(); ++it){
			if (*it == sign){
				signalList_.erase(it);
				break;
			}
		}

		signals_[sign].lbLeftMarkVal->deleteLater();
		signals_[sign].lbRightMarkVal->deleteLater();
		signals_.remove(sign);
			
		repaintEna_ = true;
		ui.wPlot->update();

		emit req_markerChange(this->objectName());
	}
}

void wdgGraph::delSignalAlter(QString sign, bool isLabelSender){

	if (signalsAlter_.contains(sign)){

		if (isLabelSender) sender()->deleteLater();
		else signalsAlter_[sign].lb->deleteLater();

		for (auto it = signalListAlter_.begin(); it != signalListAlter_.end(); ++it){
			if (*it == sign){
				signalListAlter_.erase(it);
				break;
			}
		}

		signalsAlter_[sign].lbLeftMarkVal->deleteLater();
		signalsAlter_[sign].lbRightMarkVal->deleteLater();
		signalsAlter_.remove(sign);

		repaintEna_ = true;
		ui.wPlot->update();
		emit req_markerChange(this->objectName());
	}
}

void wdgGraph::resizeByTime(){

	if (signalList_.isEmpty()&& signalsAlter_.isEmpty()) return;

	double minTm = INT64_MAX, maxTm = -INT64_MAX;
    auto sref = grPanel_->pfGetCopySignalRef();

	for (auto& sign : signalList_){
		
		if (sref[sign]->buffMinTime < minTm) minTm = sref[sign]->buffMinTime;
		if (sref[sign]->buffMaxTime > maxTm) maxTm = sref[sign]->buffMaxTime;
	}

	for (auto& sname : signalListAlter_){
		
		if (sref[sname]->buffMinTime < minTm) minTm = sref[sname]->buffMinTime;
		if (sref[sname]->buffMaxTime > maxTm) maxTm = sref[sname]->buffMaxTime;
	}
	
	axisTime_->setTimeInterval(minTm, maxTm);
	
	axisTimeChange();
	emit req_axisTimeUpdate(this->objectName()); 
	
}

void wdgGraph::resizeByValue(){
	
	if (signals_.isEmpty()) return;

	double minVal = INT32_MAX, maxVal = -INT32_MAX; bool isFloatSign = false;
	for (auto& sign : signals_){

		if (sign.type != valueType::tBool){

			isFloatSign = true; QPair<double, double> minMaxVal = getSignMaxMinValue(&sign);

			if (minMaxVal.first < minVal) minVal = minMaxVal.first;
			if (minMaxVal.second > maxVal) maxVal = minMaxVal.second;
		}			
	}

	if (isFloatSign){

		QPair<double, double> intl = ui.wAxisValue->getValInterval();
	
		double scale = ui.wAxisValue->getValScale();
		ui.wAxisValue->setValInterval(intl.first + minVal * scale - 1, intl.first + maxVal * scale + 1);
	
		axisValueChange();
	}
}

void wdgGraph::resizeByRect(){

	QRect rct = ui.wPlot->SelRect;
	ui.wPlot->SelRect = QRect(0, 0, 0, 0);

	if ((rct.width() < 30) || (rct.height() < 30)){
		plotUpdate();
		return;
	}

	addPosToHistory();

	QPair<qint64, qint64> tmIntl = axisTime_->getTimeInterval();

    qint64 tmBegin = tmIntl.first + rct.x() * axisTime_->getTimeScale();
    qint64 tmEnd = tmIntl.first + (rct.x() + rct.width()) *axisTime_->getTimeScale();
	
	axisTime_->setTimeInterval(tmBegin, tmEnd);
	
	QPair<double, double> valIntl = ui.wAxisValue->getValInterval();

	double valBegin = valIntl.first + (ui.wPlot->height() - rct.y() - rct.height()) * ui.wAxisValue->getValScale();
	double valEnd = valIntl.first + (ui.wPlot->height() - rct.y()) * ui.wAxisValue->getValScale();

	ui.wAxisValue->setValInterval(valBegin, valEnd);

	
	axisTime_->update();
	ui.wAxisValue->update();
	plotUpdate();
	
	emit req_axisTimeUpdate(this->objectName());

}

void wdgGraph::updateByMarker(){

	if (axisTime_){
		
		ui.wPlot->update();

		emit req_markerChange(this->objectName());

	}
}

QVector<wdgGraph::graphSignPoint> wdgGraph::getSignalValueByMarkerPos(int pos){

	QVector<graphSignPoint> res;

	QPair<double,double> valIntr = ui.wAxisValue->getValInterval();
	double valScale = ui.wAxisValue->getValScale();

	int sz = signalList_.size();
	for (int i = 0; i < sz; ++i){
		
		auto sign = signals_[signalList_[i]];
		res.append(graphSignPoint());
        res[i].sign = signalList_[i];
		res[i].name = sign.name;
		res[i].type = sign.type;
		res[i].color = sign.color;

		int sZn = sign.pnts.size(); bool exist = false;
		for (int zn = 0; zn < sZn; ++zn){

			int prev = sign.pnts[zn][0].first;
			for (auto pt : sign.pnts[zn]){
				
				if ((pt.first == pos) || ((prev < pos) && (pos < pt.first))){
										
					if (sign.type != valueType::tBool)
						res[i].val = pt.second * valScale + valIntr.first;
					else 
						res[i].val = pt.second;

					res[i].xPix = pos;
					res[i].yPix = pt.second;
					exist = true;
					break;
				}
				prev = pt.first;
			}
			if (exist) break;
		}
	}

	return res;
}

QVector<wdgGraph::graphSignPoint> wdgGraph::getSignalAlterValueByMarkerPos(int pos){

	QVector<graphSignPoint> res;

	int sz = signalListAlter_.size();
	for (int i = 0; i < sz; ++i){

		auto sign = signalsAlter_[signalListAlter_[i]];

		auto sdata = grPanel_->pfGetSignalData(signalListAlter_[i]);

		QPair<qint64, qint64> tmInterval = axisTime_->getTimeInterval();
		QPair<double, double> valInterval = getSignMaxMinValue(sdata, tmInterval);
		double valMinInterval = valInterval.first - 1, valMaxInterval = valInterval.second + 3;
		double valScale = (valMaxInterval - valMinInterval) / ui.wPlot->height();

		res.append(graphSignPoint());
		res[i].sign = signalListAlter_[i];
		res[i].name = sign.name;
		res[i].type = sign.type;
		res[i].color = sign.color;

		int sZn = sign.pnts.size(); bool exist = false;
		for (int zn = 0; zn < sZn; ++zn){

			int prev = sign.pnts[zn][0].first;
			for (auto pt : sign.pnts[zn]){

				if ((pt.first == pos) || ((prev < pos) && (pos < pt.first))){

					if (sign.type != valueType::tBool)
						res[i].val = pt.second * valScale + valMinInterval;
					else
						res[i].val = pt.second;

					res[i].xPix = pos;
					res[i].yPix = pt.second;

					exist = true;
					break;
				}
				prev = pt.first;
			}
			if (exist) break;
		}
	}

	return res;
}
void wdgGraph::undoCmd(){

	if (historyPos_.isEmpty()) return;

	histPos histP = historyPos_.back();

	axisTime_->setTimeInterval(histP.tmIntl.first, histP.tmIntl.second);
		
	ui.wAxisValue->setValInterval(histP.valIntl.first, histP.valIntl.second);

	axisTime_->update();
	ui.wAxisValue->update();
	plotUpdate();

	if (historyPos_.size() > 1) historyPos_.pop_back();

	emit req_axisTimeUpdate(this->objectName());
}

void wdgGraph::colorUpdate(){

	for (auto it = signals_.begin(); it != signals_.end(); it++){

		colorCnt_ += 30;

		int num = it->num;
		QColor clr = QColor((num * (60 + colorCnt_)) % 255,
			(num * (120 + colorCnt_)) % 255,
			(num * (180 + colorCnt_)) % 255, 255);

		it->color = clr;
				
		it->lb->setStyleSheet("color : " + clr.name() + "; ");
        it->lbLeftMarkVal->setStyleSheet("color : " + clr.name() + "; ");
		it->lbRightMarkVal->setStyleSheet("color : " + clr.name() + "; ");

	}

	for (auto it = signalsAlter_.begin(); it != signalsAlter_.end(); it++){

		colorCnt_ += 30;

		int num = it->num;
		QColor clr = QColor((num * (60 + colorCnt_)) % 255,
			(num * (120 + colorCnt_)) % 255,
			(num * (180 + colorCnt_)) % 255, 255);

		it->color = clr;

		it->lb->setStyleSheet("color : " + clr.name() + "; ");
		it->lbLeftMarkVal->setStyleSheet("color : " + clr.name() + "; ");
		it->lbRightMarkVal->setStyleSheet("color : " + clr.name() + "; ");
	}
	repaintEna_ = true;
	ui.wPlot->update();

	emit req_markerChange(this->objectName());
}

void wdgGraph::showMarkPos(){
		QPoint leftMarkPos, rightMarkPos;
		getMarkersPos(leftMarkPos, rightMarkPos);
		
		int crPos = ui.wPlot->mapFromGlobal(QCursor::pos()).x();
		if (abs(crPos - leftMarkPos.x()) > abs(crPos - rightMarkPos.x())){
			rightMarkPos.setX(crPos);
			selRigthMark_ = true;
		}
		else{
			leftMarkPos.setX(crPos);
			selLeftMark_ = true;
		}
		setMarkersPos(leftMarkPos, rightMarkPos);
		updateByMarker();
}

void wdgGraph::scale(bool posNeg){

	ui.wPlot->scale(posNeg ? 5 : -5);
}
