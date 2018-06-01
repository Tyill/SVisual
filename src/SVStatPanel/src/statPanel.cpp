#include "stdafx.h"
#include "forms/statPanel.h"
#include "SVConfig/SVConfigLimits.h"


using namespace SV_Cng;

statPanel::statPanel(QWidget *parent, SV_Stat::config cng_){
		
	setParent(parent);
	
#ifdef SV_EN
	QTranslator translator;
	translator.load(":/SVStat/svstatpanel_en.qm");
	QCoreApplication::installTranslator(&translator);
#endif

	setAcceptDrops(true);

	cng = cng_;

	ui.setupUi(this);

	ui.txtDiapMin->setEnabled(false);
	ui.txtDiapMax->setEnabled(false);

	connect(ui.chbDiapEna, &QCheckBox::stateChanged, [this]() {
		if (ui.chbDiapEna->isChecked()){
			diapEna_ = true;
			ui.txtDiapMin->setEnabled(true);
			ui.txtDiapMax->setEnabled(true);
			selectSignalChange();
		}
		else{
			diapEna_ = false;
			ui.txtDiapMin->setEnabled(false);
			ui.txtDiapMax->setEnabled(false);
			selectSignalChange();
		}
	});

	connect(ui.chbWinDiap, SIGNAL(stateChanged(int)), SLOT(selectSignalChange()));
	connect(ui.chbFront, SIGNAL(stateChanged(int)), SLOT(selectSignalChange()));

	connect(ui.wgtGraph, SIGNAL(selValueChange()), this, SLOT(graphValueChange()));
	connect(ui.cmbSignals, SIGNAL(currentIndexChanged(int)), this, SLOT(selectSignalChange()));
	connect(ui.listSignTimeChange, SIGNAL(currentRowChanged(int)), this, SLOT(selectSignalTime(int)));
	connect(ui.txtDiapMin, SIGNAL(textChanged(QString)), this, SLOT(selectSignalChange()));
	connect(ui.txtDiapMax, SIGNAL(textChanged(QString)), this, SLOT(selectSignalChange()));
	connect(ui.txtValue, SIGNAL(textChanged(QString)), ui.wgtGraph, SLOT(setTargPos(QString)));

}

statPanel::~statPanel(){


}

void statPanel::selectSignalChange(){

	QString sign = ui.cmbSignals->currentData().toString();	

	if (sign.isEmpty()) return;

	sign_[sign].hist = calcHist(sign);

	ui.wgtGraph->setGraphPnt(sign_[sign].hist);

	graphValueChange();
}

QVector<recData> statPanel::getSignData(QString sname){
		
	signalData* sdata = pfGetSignalData(sname);

	if (!ui.chbWinDiap->isChecked()){

		int sz = sdata->buffData.size();
		QVector<recData> res(sz);
		for (int i =0; i < sz; ++i)
			res[i] = sdata->buffData[i];

		return res;
	}

	QPair<qint64, qint64> tmInterval = pfGetTimeInterval ? pfGetTimeInterval() : QPair<qint64, qint64>();
		
	int znSz = sdata->buffData.size();

	uint64_t tmZnBegin = sdata->buffMinTime,
		tmZnEnd = sdata->buffMaxTime,
		tmMinInterval = tmInterval.first,
		tmMaxInterval = tmInterval.second;

	if ((tmZnBegin >= tmMaxInterval) || (tmZnEnd <= tmMinInterval)) return QVector<recData>();


	QVector<recData> res;
		
	tmZnEnd = sdata->buffData[0].beginTime + SV_CYCLESAVE_MS;
	uint64_t tmZnEndPrev = sdata->buffData[0].beginTime;

	int z = 0, pntPrev = 0;
	while (tmZnBegin < tmMaxInterval){

		if (tmZnEnd > tmMinInterval){
			res.append(sdata->buffData[z]);
		}

		z++;

		if (z < znSz){
			tmZnBegin = sdata->buffData[z].beginTime,
			tmZnEnd = sdata->buffData[z].beginTime + SV_CYCLESAVE_MS;
		}
		else break;
	}

	return res;
}

void statPanel::dragEnterEvent(QDragEnterEvent *event)
{
	if (qobject_cast<QTreeWidget *>(event->source()) ||
		qobject_cast<QLabel *>(event->source())) {

		event->accept();

	}
}

void statPanel::dragMoveEvent(QDragMoveEvent *event){

	if (qobject_cast<QTreeWidget *>(event->source()) ||
		qobject_cast<QLabel *>(event->source())) {

		event->accept();

	}
}

void statPanel::dropEvent(QDropEvent *event)
{
	QLabel* lb = qobject_cast<QLabel*>(event->source());

	if (qobject_cast<QTreeWidget *>(event->source()) || lb) {

		QString sname = event->mimeData()->text();

		if (!sname.isEmpty()){

			pfLoadSignalData(sname);
			
			signalData* sdata = pfGetSignalData(sname);

			if (!sign_.contains(sname)){

				sign_.insert(sname, graphSignData{ sname, sdata->name.c_str() });
			}
									
			ui.cmbSignals->addItem(sdata->name.c_str());

			ui.cmbSignals->setItemData(ui.cmbSignals->count() - 1, sname);

			ui.cmbSignals->setCurrentIndex(ui.cmbSignals->count() - 1);
					
			if (ui.cmbSignals->count() == 1) selectSignalChange();
		}

		event->accept();
	}
}

QVector<QPair<int,int>> statPanel::calcHist(QString sname){
		
	vars_ = getSignData(sname);

	QMap<int, int> hist; int bDuration = 0;

	if (diapEna_){

		int minVal = ui.txtDiapMin->text().toInt();
		int maxVal = ui.txtDiapMax->text().toInt();
		int sz = vars_.size();
		bool isOnlyFront = ui.chbFront->isChecked();
		int prevVal = 0;
		
		signalData* sdata = pfGetSignalData(sname);

		switch (sdata->type)
		{
		case valueType::tBool:{
			bool prev = false; hist.insert(0, 0); sign_[sname].valData.insert(0, valSData{ 0, 0 });
			for (int i = 0; i < sz; ++i){

				for (int j = 0; j < SV_PACKETSZ; ++j){

					bool val = vars_[i].vals[j].tBool;

					if (val){
						if (!isOnlyFront || (val != prevVal)) hist[0]++;
						if (val != prevVal) sign_[sname].valData[0].changeCnt++;

						if (val) sign_[sname].valData[0].duration++;
					}
					prev = val;
				}
			}
		}
			break;

		case valueType::tInt:
			
			for (int i = 0; i < sz; ++i){

				for (int j = 0; j < SV_PACKETSZ; ++j){

					int val = vars_[i].vals[j].tInt;

					if ((minVal <= val) && (val <= maxVal)){
																	
						if (!hist.contains(val)){
							hist.insert(val, 1);
							sign_[sname].valData.insert(val, valSData{ 1, 1 });
						}
						else{
							if (!isOnlyFront || (val != prevVal)) hist[val]++;
							if (val != prevVal) sign_[sname].valData[val].changeCnt++;
							sign_[sname].valData[val].duration++;
						}

						prevVal = val;
					}
				}
			}
			break;
		case valueType::tFloat:
			
			for (int i = 0; i < sz; ++i){

				for (int j = 0; j < SV_PACKETSZ; ++j){

					int val = vars_[i].vals[j].tFloat > 0 ? vars_[i].vals[j].tFloat + 0.5 : vars_[i].vals[j].tFloat - 0.5;
										
					if ((minVal <= val) && (val <= maxVal)){
												
						if (!hist.contains(val)){
							hist.insert(val, 1);
							sign_[sname].valData.insert(val, valSData{ 1, 1 });
						}
						else{
							if (!isOnlyFront || (val != prevVal)) hist[val]++;
							if (val != prevVal) sign_[sname].valData[val].changeCnt++;
							sign_[sname].valData[val].duration++;
						}

						prevVal = val;
					}
				}
			}
			break;
		default:
			break;
		}

	}
	else{
		
		int sz = vars_.size();
		bool isOnlyFront = ui.chbFront->isChecked();
		int prevVal = 0;
		signalData* sdata = pfGetSignalData(sname);

		switch (sdata->type)
		{
			case valueType::tBool:{
				bool prev = false; hist.insert(0, 0); sign_[sname].valData.insert(0, valSData{ 0, 0 });
				for (int i = 0; i < sz; ++i){

					for (int j = 0; j < SV_PACKETSZ; ++j){

						bool val = vars_[i].vals[j].tBool;
						
						if (val){
							if (!isOnlyFront || (val != prevVal)) hist[0]++;
							if (val != prevVal) sign_[sname].valData[0].changeCnt++;

							if (val) sign_[sname].valData[0].duration++;
						}
						prev = val;
					}
				}
				}
			break;

			case valueType::tInt:			
				for (int i = 0; i < sz; ++i){

					for (int j = 0; j < SV_PACKETSZ; ++j){
					
						int val = vars_[i].vals[j].tInt;
												
						if (!hist.contains(val)){
							hist.insert(val, 1);
							sign_[sname].valData.insert(val, valSData{ 1, 1 });
						}
						else{
							if (!isOnlyFront || (val != prevVal)) hist[val]++;
							if (val != prevVal) sign_[sname].valData[val].changeCnt++;
							sign_[sname].valData[val].duration++;
						}

						prevVal = val;
					}
				}
				break;
			case valueType::tFloat:
				
				for (int i = 0; i < sz; ++i){

					for (int j = 0; j < SV_PACKETSZ; ++j){

						int val = vars_[i].vals[j].tFloat > 0 ? vars_[i].vals[j].tFloat + 0.5 : vars_[i].vals[j].tFloat - 0.5;
						
						if (!hist.contains(val)){
							hist.insert(val, 1);
							sign_[sname].valData.insert(val, valSData{ 1, 1 });
						}
						else{
							if (!isOnlyFront || (val != prevVal)) hist[val]++;
							if (val != prevVal) sign_[sname].valData[val].changeCnt++;
							sign_[sname].valData[val].duration++;
						}

						prevVal = val;
					}
				}
				break;
			default:
				break;
		}	
	}

	QVector<QPair<int, int>> res;
	QList<int> keys = hist.keys(); int sz = keys.size();
	for (int i = 0; i < sz; ++i){

		res.append(QPair<int,int>(keys[i], hist[keys[i]]));		

		sign_[sname].valData[keys[i]].duration *= SV_CYCLEREC_MS;

		sign_[sname].valData[keys[i]].durationMean = (double)sign_[sname].valData[keys[i]].duration / sign_[sname].valData[keys[i]].changeCnt + 0.5;
	}	
			
	return res;
}

void statPanel::listTimeUpdate(QString sname, int selVal){
			
	ui.listSignTimeChange->clear();
	
	signalData* sdata = pfGetSignalData(sname);

	if (sign_[sname].valData.contains(selVal)){
		int duration = sign_[sname].valData[selVal].duration / 1000.0 + 0.5;
		ui.txtDurationFull->setText(QString::number(duration));
		ui.txtDurationMean->setText(QString::number(sign_[sname].valData[selVal].durationMean));
		
	}
	if (sdata->type == valueType::tBool)
		ui.txtDurationMean->setText(QString::number(sign_[sname].valData[0].durationMean));

	if (sname.isEmpty() || (!ui.chbFront->isChecked() && (sdata->type != valueType::tBool))) return;
		
	int sz = vars_.size(); QStringList itTimes; int prev = 0;
	switch (sdata->type)
	{
		case valueType::tBool:
			
			for (int i = 0; i < sz; ++i){

				for (int j = 0; j < SV_PACKETSZ; ++j){

					int val = vars_[i].vals[j].tBool;

					if (val && (val != prev)){
						itTimes.append(QDateTime::fromMSecsSinceEpoch(vars_[i].beginTime + j * SV_CYCLEREC_MS).toString("yyyy.MM.dd hh:mm:ss:zzz"));
					}
					prev = val;
				}
			}
		break;

	case valueType::tInt:

		for (int i = 0; i < sz; ++i){

			for (int j = 0; j < SV_PACKETSZ; ++j){

				int val = vars_[i].vals[j].tInt;

				if ((val != prev) && (val == selVal)){
					itTimes.append(QDateTime::fromMSecsSinceEpoch(vars_[i].beginTime + j * SV_CYCLEREC_MS).toString("yyyy.MM.dd hh:mm:ss:zzz"));
				}
				prev = val;
			}
		}
		break;
	case valueType::tFloat:

		for (int i = 0; i < sz; ++i){

			for (int j = 0; j < SV_PACKETSZ; ++j){

				int val = vars_[i].vals[j].tFloat;

				if ((val != prev) && (val == selVal)){
					itTimes.append(QDateTime::fromMSecsSinceEpoch(vars_[i].beginTime + j * SV_CYCLEREC_MS).toString("yyyy.MM.dd hh:mm:ss:zzz"));
				}
				prev = val;
			}
		}
		break;
	default:
		break;
	}
	ui.listSignTimeChange->addItems(itTimes);

	ui.txtHistValue->setText(QString::number(itTimes.size()));	
}

void statPanel::graphValueChange(){

	ui.txtCurrArea->setText(QString::number(ui.wgtGraph->getAreaByPos()));
	
	QPoint vp = ui.wgtGraph->getTargPos();

	ui.txtValue->setText(QString::number(vp.x()));
	
	ui.txtHistValue->setText(QString::number(vp.y()));
	
	QString sname = ui.cmbSignals->currentData().toString();

	int mx = Mx(sign_[sname].hist);
	ui.txtMx->setText(QString::number(mx));

	listTimeUpdate(sname, vp.x());
}

void statPanel::selectSignalTime(int row){

	QListWidgetItem* item = ui.listSignTimeChange->currentItem();

	if (!item) return;
		
	QDateTime dt = QDateTime::fromString(item->text(),"yyyy.MM.dd hh:mm:ss:zzz");

	if (pfSetTimeInterval)
    	pfSetTimeInterval(dt.addSecs(-1).toMSecsSinceEpoch(), dt.addSecs(1).toMSecsSinceEpoch());
	
}

// Mx Hist
int statPanel::Mx(QVector<QPair<int, int>>& hist)
{
	int64_t Square = 0;
	int64_t Mx = 0;

	int sz = hist.size();
	for (int i = 0; i < sz; i++)
	{
		Mx += hist[i].first * hist[i].second;
		Square += hist[i].second;
	}

	if (Square > 0) return (double)Mx / Square + 0.5;

	return 0;
}

