
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

        int cycleRecMs;
        int packetSz;
        modeGr mode;

        config(int cycleRecMs_ = 100, int packetSz_ = 10, modeGr mode_ = modeGr::player) :
                cycleRecMs(cycleRecMs_),
                packetSz(packetSz_),
                mode(mode_) {}
    };

    SVGRAPHPANEL_API QWidget *createGraphPanel(QWidget *parent, config cng);

    typedef QMap<QString, SV_Cng::signalData *>(*pf_getCopySignalRef)();
    SVGRAPHPANEL_API void setGetCopySignalRef(QWidget *gp, pf_getCopySignalRef f);

    typedef SV_Cng::signalData *(*pf_getSignalData)(const QString& sign);
    SVGRAPHPANEL_API void setGetSignalData(QWidget *gp, pf_getSignalData f);

    typedef bool (*pf_loadSignalData)(const QString& sign);
    SVGRAPHPANEL_API void setLoadSignalData(QWidget *gp, pf_loadSignalData f);

    SVGRAPHPANEL_API void addSignal(QWidget *gp, QString sname);

    SVGRAPHPANEL_API void update(QWidget *gp);

    SVGRAPHPANEL_API QPair<qint64, qint64> getTimeInterval(QWidget *gp);

	SVGRAPHPANEL_API void setTimeInterval(QWidget *gp, qint64 stTime, qint64 enTime);

}