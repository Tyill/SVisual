//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "stdafx.h"
#include "SVGraphPanel/SVGraphPanel.h"
#include "SVGraphPanel/forms/graphPanel.h"
#include "SVConfig/SVConfigLimits.h"
#include <QTranslator>


namespace SV_Graph {

	QWidget *createGraphPanel(QWidget *parent, const SV_Graph::config& cng) {
				     
		return new graphPanel(parent, cng);
	}

    void setGraphSetting(QWidget *gp, const graphSetting& gs){

        if (gp)
            ((graphPanel*)gp)->setGraphSetting(gs);
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
	
    void addSignal(QWidget *gp, QString sname, int section) {

		if (gp)
            ((graphPanel*)gp)->addSignalOnGraph(sname, section);
	}

	void update(QWidget *gp) {

		if (gp)
			((graphPanel*) gp)->updateSignals();
	}

	QPair<qint64, qint64> getTimeInterval(QWidget *gp) {

		if (gp)
			return ((graphPanel*) gp)->getTimeInterval();

		return QPair<qint64, qint64>();
	}

	void setTimeInterval(QWidget* gp, qint64 stTime, qint64 enTime) {

		if (gp) {

			((graphPanel*)gp)->setTimeInterval(stTime, enTime);

			((graphPanel*)gp)->ui.axisTime->update();

			((graphPanel*)gp)->resizeByValue();
		}
	}

    QVector<QVector<QString>> getLocateSignals(QWidget *gp){

        return gp ? ((graphPanel*)gp)->getLocateSignals() : QVector<QVector<QString>>();

    }
}