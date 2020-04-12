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


#include "SVConfig/SVConfigData.h"

#ifdef _WIN32
#ifdef SVZABBIX_EXPORTS
#define SVZABBIX_API __declspec(dllexport)
#else
#define SVZABBIX_API __declspec(dllimport)
#endif
#else
#define SVZABBIX_API
#endif

namespace SV_Zbx {
    
	  struct config {
      
		int cycleRecMs;
		int packetSz;

		config(int cycleRecMs_ = 100, int packetSz_ = 10) :
				cycleRecMs(cycleRecMs_),
				packetSz(packetSz_) {}
	  };

    SVZABBIX_API bool startAgent(const QString& addr, int port, const config&);

    SVZABBIX_API void stopAgent();
    
    typedef SV_Cng::signalData* (*pf_getSignalData)(const QString& sign);
    SVZABBIX_API void setGetSignalData(pf_getSignalData f);        
    
}