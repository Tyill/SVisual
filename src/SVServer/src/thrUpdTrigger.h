
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