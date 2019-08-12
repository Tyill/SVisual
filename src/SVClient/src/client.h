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

#include "stdafx.h"
#include "SVConfig/SVConfigLimits.h"
#include "SVConfig/SVConfigData.h"
#include <mutex>
#include <thread>

class client
{
public:

	struct config{

		int cycleRecMs;
		int packetSz;

		config(int cycleRecMs_ = 100, int packetSz_ = 10) :
				cycleRecMs(cycleRecMs_),
				packetSz(packetSz_)
		{}
	};

	client() = default;

	~client();

	bool connect(const char* moduleName, const char* ipAddr, int port);
	
	void disconnect();

	bool addValue(const char* name, SV_Cng::valueType type, SV_Cng::value value, bool onlyFront);
	
	void setConfig(config cng);

private:

	struct valueRec{
		bool isActive;
		bool isOnlyFront;
		char name[SV_NAMESZ];
		SV_Cng::valueType type;
		SV_Cng::value* vals;
	};

	bool isConnect_ = false, 
         thrStop_ = false, 
         isWrite_ = false;

	std::thread thr_;
	std::mutex mtxConnect_, mtxUpdValue_;

	std::map<std::string, valueRec*> values_;

	std::string module_, addrServ_;

	int portServ_ = 0;

	int curCycCnt_ = 0;

	config cng;

	void sendCyc();

	bool sendData();
};
