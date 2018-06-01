
#pragma once

#include "SVConfig/SVConfigLimits.h"
#include "SVConfig/SVConfigData.h"

struct valueData{
	char name[SV_NAMESZ];
	char module[SV_NAMESZ];
	char group[SV_NAMESZ];
	char comment[SV_COMMENTSZ];
	SV_Cng::valueType type;
	int vlCnt;
};

struct fileData{

	QString file;
	int utcOffsMs;

	struct fsd{
		bool isLoad;
		int vlsCnt;
		QVector<QPair<int, int>> patchApos;
	};

	QMap<QString, fsd> signls;

	fileData(QString fName, int utc){

		file = fName;
		utcOffsMs = utc;
	}
};



