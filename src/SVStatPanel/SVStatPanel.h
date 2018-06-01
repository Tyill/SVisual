#pragma once

#include <QtCore>
#include "SVConfig/SVConfigData.h"

#ifdef _WIN32
#ifdef SVSTATPANEL_EXPORTS
#define SVSTATPANEL_API __declspec(dllexport)
#else
#define SVSTATPANEL_API __declspec(dllimport)
#endif
#else
#define SVSTATPANEL_API
#endif

namespace SV_Stat {

	struct config {

		int cycleRecMs;
		int packetSz;

		config(int cycleRecMs_ = 100, int packetSz_ = 10) :
				cycleRecMs(cycleRecMs_),
				packetSz(packetSz_) {}
	};

	SVSTATPANEL_API QDialog* createStatPanel(QWidget* parent, config);

	typedef QMap<QString, SV_Cng::signalData*>(*pf_getCopySignalRef)();
	SVSTATPANEL_API void setGetCopySignalRef(QDialog* stPanel, pf_getCopySignalRef f);

	typedef SV_Cng::signalData *(*pf_getSignalData)(const QString &sign);
	SVSTATPANEL_API void setGetSignalData(QDialog* stPanel, pf_getSignalData f);

	typedef bool(*pf_loadSignalData)(const QString& sign);
	SVSTATPANEL_API void setLoadSignalData(QDialog* stPanel, pf_loadSignalData f);

	typedef QPair<qint64, qint64>(*pf_getTimeInterval)();
	SVSTATPANEL_API void setGetTimeInterval(QDialog* stPanel, pf_getTimeInterval f);

	typedef void(*pf_setTimeInterval)(qint64, qint64);
	SVSTATPANEL_API void setSetTimeInterval(QDialog* stPanel, pf_setTimeInterval f);

}