
#pragma once

#include "stdafx.h"
#include "Lib/sqlite/sqlite3.h"
#include "SVConfig/SVConfigData.h"
#include "structurs.h"

class sql
{
private:
	
	sqlite3* db_;

	QString pathDB_;

	QMutex mtx_;
			
	bool Init();

	bool Open();

	void Close();

	bool Query(const std::string& query,
		std::vector<std::vector<std::string>>& result);
	
public:
		
	sql(QString path, bool& err);

	~sql();
	
	bool saveTriggers(const std::map<std::string, SV_Cng::triggerData*>& trg);
	QVector<SV_Cng::triggerData*> getTrigger(const QString& signal, const QString& module);
	SV_Cng::triggerData* getTrigger(const QString& trname);
	void delTrigger(const QString& trname);
	
	bool saveSignals(const std::map<std::string, SV_Cng::signalData*>& signs);
	SV_Cng::signalData getSignal(const QString& signal, const QString& module);

	bool saveUserEventData(const QMap<QString, userEventData>&);
	userEventData getUserEventData(QString name);

	void saveEvent(QString trigger, QDateTime dt);
	QVector<uEvent> getEvents(QDateTime beginTime, QDateTime endTime);
};