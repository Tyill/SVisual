
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
		int cycleRecMs;           ///< период записи - задает пользователь
		int packetSz;             ///< размер пакета - задает пользователь

		std::string outArchivePath;      ///< запись архива путь
		std::string outArchiveName;      ///< запись архива имя файла
		int outArchiveHourCnt;       ///< запись архива размер файла, час

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

	SV_Aux::TimerDelay  tm_;
    SV_Aux::Front front_;

    std::map<std::string, std::vector<SV_Cng::recData>> archiveData_;

	bool isCopyTimeHour();
    std::string getOutPath(bool isStop);

    bool compressData(uint8_t* arr, size_t sz, size_t& outsz, uint8_t** outArr);

    void statusMess(const std::string&);
};
