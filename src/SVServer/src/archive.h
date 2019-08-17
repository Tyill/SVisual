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
#include "server.h"
#include "SVConfig/SVConfigData.h"
#include "SVAuxFunc/TimerDelay.h"
#include "SVAuxFunc/Front.h"

// копирование архива на диск
class archive
{
public:

	struct config{
		int cycleRecMs;                ///< период записи - задает пользователь
		int packetSz;                  ///< размер пакета - задает пользователь

		std::string outArchivePath;    ///< запись архива путь
		std::string outArchiveName;    ///< запись архива имя файла
		int outArchiveHourCnt;         ///< запись архива размер файла, час

		config(int cycleRecMs_ = 50, int packetSz_ = 10) :
				cycleRecMs(cycleRecMs_),
				packetSz(packetSz_){}
	};

    archive(config, server* serv);

    bool copyToDisk(bool isStop);

    void addSignal(const std::string& sign);
    void addValue(const std::string& sign, const SV_Cng::recData& rd);

    void setConfig(config);

private:

    config cng;

    server* pServ_ = nullptr;

	std::string copyStartTime_ = "";
    std::string copyDateMem_ = "";

	int crtFileHour_ = 0;
	const int archCycleMs = 600000;     // 10мин
	int cpySz_ = 0;
	std::map<std::string, int> valPos_;
    	
    SV_Aux::Front front_;

    std::map<std::string, std::vector<SV_Cng::recData>> archiveData_;

	bool isCopyTimeHour();
    std::string getOutPath(bool isStop);

    bool compressData(size_t insz, const std::vector<char>& inArr, size_t& outsz, std::vector<char>& outArr);

    void statusMess(const std::string&);
};
