
#include "stdafx.h"
#include "SVServer/SVServer.h"

QMap<QString, SV_Cng::moduleData *> getCopyModuleRefSrv();

QMap<QString, SV_Cng::triggerData *> getCopyTriggerRefSrv();

QMap<QString, SV_Cng::signalData *> getCopySignalRefSrv();

SV_Cng::signalData* getSignalDataSrv(const QString& sing);

SV_Cng::triggerData* getTriggerDataSrv(const QString& trg);

bool addTriggerSrv(QString name, SV_Cng::triggerData* td);

bool delTriggerSrv(QString name);