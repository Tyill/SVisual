#include "stdafx.h"
#include "SVGraphPanel/SVGraphPanel.h"
#include "SVGraphPanel/forms/graphPanel.h"
#include "SVConfig/SVConfigLimits.h"
#include <QTranslator>


namespace SV_Graph {

	QWidget *createGraphPanel(QWidget *parent, SV_Graph::config cng) {
				     
		return new graphPanel(parent, cng);
	}

	void setGetCopySignalRef(QWidget *gp, pf_getCopySignalRef f) {

		if (gp && f)
			((graphPanel*) gp)->pfGetCopySignalRef = f;
	}

	void setGetSignalData(QWidget *gp, pf_getSignalData f) {

		if (gp && f)
			((graphPanel*) gp)->pfGetSignalData = f;
	}

	void setLoadSignalData(QWidget *gp, pf_loadSignalData f) {

		if (gp && f)
			((graphPanel*) gp)->pfLoadSignalData = f;
	}
	
	void addSignal(QWidget *gp, QString sname) {

		if (gp)
			((graphPanel*) gp)->addSignalOnGraph(sname);
	}

	void update(QWidget *gp) {

		if (gp)
			((graphPanel*) gp)->updateSignals();
	}

	QPair<qint64, qint64> getTimeInterval(QWidget *gp) {

		if (gp)
			return ((graphPanel *) gp)->getTimeInterval();

		return QPair<qint64, qint64>();
	}

	void setTimeInterval(QWidget *gp, qint64 stTime, qint64 enTime) {

		if (gp) {

			((graphPanel *)gp)->setTimeInterval(stTime, enTime);

			((graphPanel *) gp)->ui.axisTime->update();

			((graphPanel *) gp)->resizeByValue();
		}
	}
}