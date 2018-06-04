
#include "stdafx.h"
#include "server.h"
#include "SVServer/SVServer.h"
#include "SVAuxFunc/TimerDelay.h"
#include "SVAuxFunc/auxFunc.h"
#include "thrUpdSignal.h"
#include "bufferData.h"
#include "archive.h"

using namespace SV_Cng;
using namespace SV_Aux;
using namespace std;

thrUpdSignal::thrUpdSignal(SV_Srv::config cng_, server* serv, bufferData* pBuff) {

    cng = cng_;

    pServ_ = serv;
    pBuffData_ = pBuff;

	archive::config arhCng(cng.cycleRecMs, cng.packetSz);
	arhCng.outArchiveHourCnt = cng.outArchiveHourCnt;
	arhCng.outArchivePath = cng.outArchivePath;
	arhCng.outArchiveName = cng.outArchiveName;

	pArchive_ = new archive(arhCng, serv);
    thr_ = std::thread([](thrUpdSignal *lp) { lp->updCycle(); }, this);
}

thrUpdSignal::~thrUpdSignal(){

	thrStop_ = true;
	if (thr_.joinable()) thr_.join();
}

void thrUpdSignal::setArchiveConfig(SV_Srv::config cng_){

	cng.outArchiveEna = cng_.outArchiveEna;

	archive::config arhCng(cng.cycleRecMs, cng.packetSz);
	arhCng.outArchiveHourCnt = cng_.outArchiveHourCnt;
	arhCng.outArchivePath = cng_.outArchivePath;
	arhCng.outArchiveName = cng_.outArchiveName;

	pArchive_->setConfig(arhCng);
}

void thrUpdSignal::addSignal(const string& sign, const bufferData::inputData& bp){

    signalData* sd = new signalData();

	sd->isActive = true;
	sd->isBuffEnable = false;
	sd->isDelete = false;

    sd->name = bp.name;
    sd->module = bp.module;
    sd->type = bp.type;

	sd->lastData.vals = new SV_Cng::value[cng.packetSz];
    sd->lastData.beginTime = SV_Aux::CurrDateTimeSinceEpochMs();
	memset(sd->lastData.vals, 0, sizeof(SV_Cng::value) * cng.packetSz);

    sd->buffMinTime = sd->lastData.beginTime - 5000;
    sd->buffMaxTime = sd->lastData.beginTime + 5000;
	sd->buffMaxValue = 1;
	sd->buffMinValue = 0;

    auto md = pServ_->getModuleData(bp.module);
    if (!md){
        md = new SV_Cng::moduleData(bp.module);
        md->isActive = false;
        md->isDelete = false;
        md->isEnable = true;
        pServ_->addModule(bp.module, md);
    }
    
    md->signls.push_back(sign);
        
    pServ_->addSignal(sign, sd);

	pArchive_->addSignal(sign);
}

void thrUpdSignal::updateSign(signalData* sign, int beginPos, int valuePos){

	sign->buffMinTime = sign->buffData[beginPos].beginTime;
	sign->buffMaxTime = sign->buffData[valuePos].beginTime + SV_CYCLESAVE_MS;

	double minValue = sign->buffMinValue, maxValue = sign->buffMaxValue;

	if (sign->type == valueType::tInt){

		value* vl = sign->buffData[valuePos].vals;

		for (int i = 0; i < SV_PACKETSZ; ++i){

			if (vl[i].tInt > maxValue) maxValue = vl[i].tInt;
			if (vl[i].tInt < minValue) minValue = vl[i].tInt;
		}

	}
	else if (sign->type == valueType::tFloat){

		value* vl = sign->buffData[valuePos].vals;
		for (int i = 0; i < SV_PACKETSZ; ++i){

			if (vl[i].tFloat > maxValue) maxValue = vl[i].tFloat;
			if (vl[i].tFloat < minValue) minValue = vl[i].tFloat;
		}
	}

	sign->buffMinValue = minValue;
	sign->buffMaxValue = maxValue;

}

void thrUpdSignal::modConnect(const string& module){

    if (pServ_->pfModuleConnectCBack)
        pServ_->pfModuleConnectCBack(module);

    auto tref = pServ_->getCopyTriggerRef();
    for (auto& tr : tref){

        if (tr.second->isActive && (tr.second->module == module) &&
			(tr.second->condType == SV_Cng::eventType::connectModule)){

			tr.second->condValue = 1;
        }
    }
}

void thrUpdSignal::modDisconnect(const string& module){

    if (pServ_->pfModuleDisconnectCBack)
        pServ_->pfModuleDisconnectCBack(module);
    
    auto tref = pServ_->getCopyTriggerRef();
    for (auto& tr : tref){

		if (tr.second->isActive && (tr.second->module == module) &&
			(tr.second->condType == SV_Cng::eventType::disconnectModule)){

			tr.second->condValue = 0;
		}
	}
}

void thrUpdSignal::updCycle(){

    auto sref = pServ_->getCopySignalRef();
	map<string, bool> signActive;
	for (auto& s : sref) {
		signActive[s.second->name + s.second->module] = true;
	}

	auto mref = pServ_->getCopyModuleRef();
	map<string, bool> moduleActive;
	for (auto& m : mref) {
		moduleActive[m.first] = true;
	}

	SV_Aux::TimerDelay tmDelay;
    tmDelay.UpdateCycTime();

	int buffSz = pServ_->BUFF_SIGN_HOUR_CNT * 3600000 / SV_CYCLESAVE_MS; //2 часа жестко
	int packSz = SV_PACKETSZ * sizeof(value);            // размер пакета
	int checkConnectTout = 5 * SV_CYCLESAVE_MS / 1000;   // проверка связи, тоже жестко:(

	while (!thrStop_){

        tmDelay.UpdateCycTime();

		bool isNewSign = false, isBuffActive = false; string sign;
		bufferData::inputData bp = pBuffData_->getDataByReadPos();
		while (bp.isActive){

			isBuffActive = true;

			sign = bp.name + bp.module;

			if (sref.find(sign) == sref.end()){
				addSignal(sign, bp);
				sref[sign] = pServ_->getSignalData(sign);
				mref[bp.module] = pServ_->getModuleData(bp.module);;
				isNewSign = true;
			}

			auto sd =  sref[sign];

			signActive[sign] = true;
			moduleActive[sd->module] = true;

			sd->lastData.beginTime = bp.data.beginTime;
			memcpy(sd->lastData.vals, bp.data.vals, packSz);

            // заполняем буфер, если разрешено
            if (sd->isBuffEnable) {
                int vp = sd->buffValuePos;
                sd->buffData[vp].beginTime = bp.data.beginTime;
                memcpy(sd->buffData[vp].vals, bp.data.vals, packSz);

                updateSign(sd, sd->buffBeginPos, vp);

                ++vp;
                if (vp == buffSz) vp = 0;
                sd->buffValuePos = vp;

                if (vp == sd->buffBeginPos) {
                    ++sd->buffBeginPos;
                    if (sd->buffBeginPos >= buffSz) sd->buffBeginPos = 0;
                }
            }

            if (cng.outArchiveEna)
			   pArchive_->addValue(sign, bp.data);

			pBuffData_->incReadPos();
			bp = pBuffData_->getDataByReadPos();
		}
		
		if (isBuffActive && pServ_->pfUpdateSignalsCBack) pServ_->pfUpdateSignalsCBack();

		if (isNewSign && pServ_->pfAddSignalsCBack) pServ_->pfAddSignalsCBack();

		// архив
		if (cng.outArchiveEna && tmDelay.HourOnc())
			pArchive_->copyToDisk(false);

		// проверка связи
		if (tmDelay.OnDelTmSec(true, checkConnectTout, 0)){
			tmDelay.OnDelTmSec(false, 0, 0);

			for (auto& s : signActive){
				sref[s.first]->isActive = s.second;
				s.second = false;
			}

			for (auto& m : moduleActive){
			    
			    if (!mref[m.first]->isActive && m.second)
			        modConnect(m.first);
			    
			    else if (mref[m.first]->isActive && !m.second)
                    modDisconnect(m.first);
			    
				mref[m.first]->isActive = m.second;
				m.second = false;
			}
		}

		SleepMs(min(SV_CYCLESAVE_MS, 10000));
	}

	if (cng.outArchiveEna) pArchive_->copyToDisk(true);
}

