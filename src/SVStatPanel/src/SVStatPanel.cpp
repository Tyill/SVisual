#include "stdafx.h"
#include "forms/statPanel.h"

namespace SV_Stat {

	QDialog *createStatPanel(QWidget *parent, SV_Stat::config cng) {

		return new statPanel(parent, cng);
	}
	
	void setGetCopySignalRef(QDialog *stp, pf_getCopySignalRef f) {

		if (stp)
			((statPanel *)stp)->pfGetCopySignalRef = f;
	}

	void setGetSignalData(QDialog *stp, pf_getSignalData f) {

		if (stp)
			((statPanel *)stp)->pfGetSignalData = f;
	}

	void setLoadSignalData(QDialog *stp, pf_loadSignalData f) {

		if (stp)
			((statPanel *)stp)->pfLoadSignalData = f;
	}

	void setGetTimeInterval(QDialog* stp, pf_getTimeInterval f){

		if (stp)
			((statPanel *)stp)->pfGetTimeInterval = f;
	}

	void setSetTimeInterval(QDialog* stp, pf_setTimeInterval f){

		if (stp)
			((statPanel *)stp)->pfSetTimeInterval = f;
	}

}