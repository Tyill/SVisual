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
#ifdef SVWEBSERVER_EXPORTS
#define SVWEBSERVER_API __declspec(dllexport)
#else
#define SVWEBSERVER_API __declspec(dllimport)
#endif
#else
#define SVWEBSERVER_API
#endif

namespace SV_Web {
    
    struct config {

    int cycleRecMs;
    int packetSz;

    config(int cycleRecMs_ = 100, int packetSz_ = 10) :
        cycleRecMs(cycleRecMs_),
        packetSz(packetSz_) {}
    };

    SVWEBSERVER_API bool startServer(const QString& addr, int port, const config&);

    SVWEBSERVER_API void stopServer();
      
    typedef QMap<QString, SV_Base::SignalData*>(*pf_getCopySignalRef)();
    SVWEBSERVER_API void setGetCopySignalRef(pf_getCopySignalRef f);

    typedef QMap<QString, SV_Base::ModuleData*>(*pf_getCopyModuleRef)();
    SVWEBSERVER_API void setGetCopyModuleRef(pf_getCopyModuleRef f);

    typedef SV_Base::SignalData* (*pf_getSignalData)(const QString &sign);
    SVWEBSERVER_API void setGetSignalData(pf_getSignalData f);        
    
}