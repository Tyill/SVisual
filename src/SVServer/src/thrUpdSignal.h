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
#include <thread>
#include "SVConfig/SVConfigData.h"
#include "bufferData.h"
#include "archive.h"
#include "server.h"

class thrUpdSignal {

public:

	thrUpdSignal(SV_Srv::config, server*, bufferData* );

    ~thrUpdSignal();

    void setArchiveConfig(SV_Srv::config);

private:

    SV_Srv::config cng;

	bool thrStop_ = false;

	std::thread thr_;
    bufferData* pBuffData_ = nullptr;
	archive* pArchive_ = nullptr;
    server* pServ_ = nullptr;

    std::mutex mtx_;

	void updCycle();
	void updateSign(SV_Cng::signalData* sign, int beginPos, int valuePos);
	void addSignal(const std::string& sign, const bufferData::inputData& bp);
	void modConnect(const std::string& module);
	void modDisconnect(const std::string& module);
};
