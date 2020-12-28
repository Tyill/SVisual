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
#ifdef SVGRAPHPANEL_EXPORTS
#define SVGRAPHPANEL_API __declspec(dllexport)
#else
#define SVGRAPHPANEL_API __declspec(dllimport)
#endif
#else
#define SVGRAPHPANEL_API
#endif

namespace SV_Graph {

    enum class modeGr{
        viewer = 0,
        player = 1,
    };

    struct config {
        bool isShowTable = true;
        int cycleRecMs;
        int packetSz;

        modeGr mode;

        config(int cycleRecMs_ = 100, int packetSz_ = 10, modeGr mode_ = modeGr::player) :
            cycleRecMs(cycleRecMs_),
            packetSz(packetSz_),
            mode(mode_) {}
    };
        
    struct graphSetting{
        bool darkTheme = false;
        int transparent = 0;
        int lineWidth = 1;        
    };
   
    struct signalAttr{
        QColor color;
    };

    struct axisAttr{
        bool isAuto = true;
        double min = 0.0,
               max = 0.0,
               step = 0.0;
    };

    SVGRAPHPANEL_API QWidget *createGraphPanel(QWidget* gp, const config& cng);

    SVGRAPHPANEL_API void setGraphSetting(QWidget* gp, const graphSetting&);

    SVGRAPHPANEL_API void setSignalAttr(QWidget* gp, const QString& sign, const signalAttr&);

    SVGRAPHPANEL_API void setAxisAttr(QWidget* gp, const QVector<axisAttr>&);

    SVGRAPHPANEL_API QVector<axisAttr> getAxisAttr(QWidget* gp);

    typedef QMap<QString, SV_Base::SignalData*>(*pf_getCopySignalRef)();
    SVGRAPHPANEL_API void setGetCopySignalRef(QWidget* gp, pf_getCopySignalRef f);

    typedef SV_Base::SignalData *(*pf_getSignalData)(const QString& sign);
    SVGRAPHPANEL_API void setGetSignalData(QWidget* gp, pf_getSignalData f);

    typedef bool (*pf_loadSignalData)(const QString& sign);
    SVGRAPHPANEL_API void setLoadSignalData(QWidget* gp, pf_loadSignalData f);

    typedef bool (*pf_getSignalAttr)(const QString& sign, signalAttr& out);
    SVGRAPHPANEL_API void setGetSignalAttr(QWidget* gp, pf_getSignalAttr f);

    SVGRAPHPANEL_API void addSignal(QWidget* gp, QString sname, int section = 0);

    SVGRAPHPANEL_API void update(QWidget* gp);
        
    SVGRAPHPANEL_API QPair<qint64, qint64> getTimeInterval(QWidget* gp);

  SVGRAPHPANEL_API void setTimeInterval(QWidget* gp, qint64 stTime, qint64 enTime);

    SVGRAPHPANEL_API QVector<QVector<QString>> getLocateSignals(QWidget *gp);

}