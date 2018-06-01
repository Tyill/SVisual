
#include "stdafx.h"
#include "thrUpdTrigger.h"

using namespace SV_Cng;
using namespace std;


thrUpdTrigger::thrUpdTrigger(SV_Srv::config cng_, server* serv) {

	cng = cng_;

	pServ_ = serv;

	thr_ = std::thread([](thrUpdTrigger *lp) { lp->updCycle(); }, this);
}

thrUpdTrigger::~thrUpdTrigger(){

	thrStop_ = true;
	if (thr_.joinable()) thr_.join();
}

bool thrUpdTrigger::checkCondition(triggerData* tr, signalData* sd){


	bool ena = true, isImpulse = tr->condTOut <= 0;
	switch (tr->condType){
		case eventType::equals:
			if (isImpulse){
				ena = false;
				if (sd->type == valueType::tInt){
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tInt == tr->condValue){
							ena = true; break;
						}
					}
				}
				else{
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tFloat == tr->condValue){
							ena = true; break;
						}
					}
				}
			}
			else{
				ena = true;
				if (sd->type == valueType::tInt){
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tInt != tr->condValue){
							ena = false; break;
						}
					}
				}
				else{
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tFloat != tr->condValue){
							ena = false; break;
						}
					}
				}
			}
			break;
		case eventType::less:
			if (isImpulse){
				ena = false;
				if (sd->type == valueType::tInt){
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tInt < tr->condValue){
							ena = true; break;
						}
					}
				}
				else{
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tFloat < tr->condValue){
							ena = true; break;
						}
					}
				}
			}
			else{
				ena = true;
				if (sd->type == valueType::tInt){
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tInt >= tr->condValue){
							ena = false; break;
						}
					}
				}
				else{
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tFloat >= tr->condValue){
							ena = false; break;
						}
					}
				}
			}
			break;
		case eventType::more:
			if (isImpulse){
				ena = false;
				if (sd->type == valueType::tInt){
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tInt > tr->condValue){
							ena = true; break;
						}
					}
				}
				else{
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tFloat > tr->condValue){
							ena = true; break;
						}
					}
				}
			}
			else{
				ena = true;
				if (sd->type == valueType::tInt){
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tInt <= tr->condValue){
							ena = false; break;
						}
					}
				}
				else{
					for (int i = 0; i < SV_PACKETSZ; ++i) {
						if (sd->lastData.vals[i].tFloat <= tr->condValue){
							ena = false; break;
						}
					}
				}
			}
			break;
		case eventType::posFront:
			if (isImpulse){
				ena = false;
				for (int i = 0; i < SV_PACKETSZ; ++i) {
					if (sd->lastData.vals[i].tBool){
						ena = true; break;
					}
				}
			}
			else{
				ena = true;
				for (int i = 0; i < SV_PACKETSZ; ++i) {
					if (!sd->lastData.vals[i].tBool){
						ena = false; break;
					}
				}
			}
			break;
		case eventType::negFront:
			if (isImpulse){
				ena = false;
				for (int i = 0; i < SV_PACKETSZ; ++i) {
					if (!sd->lastData.vals[i].tBool){
						ena = true; break;
					}
				}
			}
			else{
				ena = true;
				for (int i = 0; i < SV_PACKETSZ; ++i) {
					if (sd->lastData.vals[i].tBool){
						ena = false; break;
					}
				}
			}
			break;

		case eventType::connectModule:
			ena = tr->condValue == 1;
			break;

		case eventType::disconnectModule:
			ena = tr->condValue == 0;
			break;

		default: ena = false;
	}

	return ena;
}

void thrUpdTrigger::updCycle(){

	map<string, int> trgId; int cid = 0;
	map<string, signalData*> sdata;

	while (!thrStop_){

		auto tref = SV_Srv::getCopyTriggerRef();

		tmDelay_.UpdateCycTime();

		for (auto& t : tref){

			if (trgId.find(t.first) == trgId.end()) {
			    ++cid;
				trgId[t.first] = cid;
				sdata[t.first] = SV_Srv::getSignalData(t.second->signal + t.second->module);
			}

			if (!sdata[t.first] && (t.second->condType != SV_Cng::eventType::connectModule) &&
				(t.second->condType != SV_Cng::eventType::disconnectModule)) continue;

			if (front_.PosFront(tmDelay_.OnDelTmSec(t.second->isActive && checkCondition(t.second, sdata[t.first]),
													t.second->condTOut, trgId[t.first]), trgId[t.first])){

				if (pServ_->pfTriggerCBack)
					pServ_->pfTriggerCBack(t.first);
			}
		}

		SV_Aux::SleepMs(SV_CYCLESAVE_MS - SV_CYCLEREC_MS);
	}
}