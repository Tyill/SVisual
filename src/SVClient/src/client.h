
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

	bool isConnect_ = false, thrStop_ = false, isWrite_ = false;

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
