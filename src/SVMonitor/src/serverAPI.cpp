
#include "stdafx.h"
#include "SVServer/SVServer.h"

QMap<QString, SV_Cng::moduleData *> getCopyModuleRefSrv(){

	QMap<QString, SV_Cng::moduleData *> cmref;

	auto mref = SV_Srv::getCopyModuleRef();
	for (auto& s : mref)
		cmref.insert(QString::fromStdString(s.first), s.second);

	return cmref;
};

QMap<QString, SV_Cng::triggerData *> getCopyTriggerRefSrv(){

	QMap<QString, SV_Cng::triggerData*> ctref;

	auto tref = SV_Srv::getCopyTriggerRef();
	for (auto& s : tref)
		ctref.insert(QString::fromStdString(s.first), s.second);

	return ctref;
};

QMap<QString, SV_Cng::signalData *> getCopySignalRefSrv(){

	QMap<QString, SV_Cng::signalData*> csref;

	auto sref = SV_Srv::getCopySignalRef();
	for (auto& s : sref)
		csref.insert(QString::fromStdString(s.first), s.second);

	return csref;
};

SV_Cng::signalData* getSignalDataSrv(const QString& sing){

	return SV_Srv::getSignalData(sing.toUtf8().data());
};

SV_Cng::triggerData* getTriggerDataSrv(const QString& trg){

	return SV_Srv::getTriggerData(trg.toUtf8().data());
};

bool addTriggerSrv(QString name, SV_Cng::triggerData* td){

	return SV_Srv::addTrigger(name.toUtf8().data(), td);
}

bool delTriggerSrv(QString name){

	return SV_Srv::delTrigger(name.toUtf8().data());
}
//////////////////////////////////////