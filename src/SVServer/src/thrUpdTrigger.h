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
#include "SVServer/SVServer.h"
#include <thread>
#include "SVConfig/SVConfigData.h"
#include "SVAuxFunc/TimerDelay.h"
#include "SVAuxFunc/Front.h"
#include "server.h"

class thrUpdTrigger
{

public:

    thrUpdTrigger(SV_Srv::config, server*);

    ~thrUpdTrigger();

private:

    SV_Srv::config cng;

    bool thrStop_ = false;

    std::thread thr_;
	server* pServ_ = nullptr;

	SV_Aux::TimerDelay tmDelay_;
    SV_Aux::Front front_;

	std::mutex mtx_;

	bool checkCondition(SV_Cng::triggerData* tr, SV_Cng::signalData* sd);
	void updCycle();
};