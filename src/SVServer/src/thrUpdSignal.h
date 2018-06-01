
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
