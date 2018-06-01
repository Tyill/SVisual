
#pragma once

#include "stdafx.h"
#include "SVConfig/SVConfigData.h"

// польз данные к событию
	struct userEventData {

        QString triggName;

        QString userProcPath;      ///< путь к польз процессу
        QString userProcArgs;      ///<

		userEventData(QString triggName_ = "", QString userProcPath_ = "", QString userProcArgs_ = "", qint64 dateTime_ = 0) :
			triggName(triggName_), userProcPath(userProcPath_), userProcArgs(userProcArgs_){}
	};

	// событие
	struct uEvent{
		QString triggName;
		QString module;
		QString signal;
		QString sendDateTime;

		SV_Cng::eventType condType;
		int condValue;
		int condTOut;
	};