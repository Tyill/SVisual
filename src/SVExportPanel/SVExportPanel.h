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
#pragma once

#include <QtCore>
#include "SVConfig/SVConfigData.h"

#ifdef _WIN32
#ifdef SVEXPPANEL_EXPORTS
#define SVEXPPANEL_API __declspec(dllexport)
#else
#define SVEXPPANEL_API __declspec(dllimport)
#endif
#else
#define SVEXPPANEL_API
#endif

namespace SV_Exp {

	struct config {

		int cycleRecMs;
		int packetSz;

		config(int cycleRecMs_ = 100, int packetSz_ = 10) :
				cycleRecMs(cycleRecMs_),
				packetSz(packetSz_) {}
	};

    SVEXPPANEL_API QDialog* createExpPanel(QWidget* parent, config);

	typedef QMap<QString, SV_Cng::signalData*>(*pf_getCopySignalRef)();
    SVEXPPANEL_API void setGetCopySignalRef(QDialog* stPanel, pf_getCopySignalRef f);

	typedef SV_Cng::signalData *(*pf_getSignalData)(const QString &sign);
    SVEXPPANEL_API void setGetSignalData(QDialog* stPanel, pf_getSignalData f);

	typedef bool(*pf_loadSignalData)(const QString& sign);
    SVEXPPANEL_API void setLoadSignalData(QDialog* stPanel, pf_loadSignalData f);

	typedef QPair<qint64, qint64>(*pf_getTimeInterval)();
    SVEXPPANEL_API void setGetTimeInterval(QDialog* stPanel, pf_getTimeInterval f);

	typedef void(*pf_setTimeInterval)(qint64, qint64);
    SVEXPPANEL_API void setSetTimeInterval(QDialog* stPanel, pf_setTimeInterval f);

}