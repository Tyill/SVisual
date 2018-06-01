
#include <stdlib.h>
#include <string.h>
#include <Ethernet.h>
#include <WiFi.h>

#include "MsTimer2.h"
#include "sv_auxFunc.h"
#include "sv_structurs.h"
#include "sv_binTree.h"
#include "sv_objClient.h"
#include "SVClient.h"

namespace svisual{

  sv_client objCln;

  // connect to server of ethernet
      // module - name module (create on server or the first call)
	  // macAddrModule - mac addr module
	  // ipAddrModule - ipaddr module
	  // ipAddrServ - ipaddr server
  bool connectOfEthernet(const char* module, const char* macAddrModule, const char* ipAddrModule, const char* ipAddrServ, int portServ){
    
	  return objCln.connectOfEthernet(module, macAddrModule, ipAddrModule, ipAddrServ, portServ);
	     
  }
  
  // connect to server of wifi
      // module - name module (create on server or the first call)
	  // ssid - your network SSID
	  // pass - secret password network
	  // ipAddrServ - ipaddr server
	  // portServ - port server
  bool connectOfWiFi(const char* module, const char* ssid, const char* pass, const char* ipAddrServ, int portServ){
    
	  return objCln.connectOfWiFi(module, ssid, pass, ipAddrServ, portServ);
	     
  }
      
  // connect to server of COM
	  // module - name module
	  // comPort - com port
	  // ipAddrServ - ipaddr server
	  // portServ - port server
  bool connectOfCOM(const char* module, int speed){
  
		return objCln.connectOfCOM(module, speed);
  }
	  
  // add value for rec
  bool addBoolValue(const char* name, bool value_in, bool onlyPosFront){

    value val; val.tBool = value_in;

	return objCln.addValue(name, valueType::tBool, val, onlyPosFront);
	
  }
  
  // add value for rec
  bool addIntValue(const char* name, int value_in){

    value val; val.tInt = value_in;

	return objCln.addValue(name, valueType::tInt, val);
	
  }
  
  bool sv_client::tcpConnect(){
	   	 
	  if (isWiFi_)
          WiFi.begin(ssid_, pass_);
      		 
	  return ethClient_->connect(ipServer_, portServ_);
  }
  
  void thrProc(){
		
	objCln.doSendCyc();
	
  }
   
  bool sv_client::connectOfEthernet(const char* moduleName, const char* macAddrModule, const char* ipAddrModule, const char* ipAddrServ, int port){

	if (isConnect_) return true;

	int len = strlen(moduleName);
	if ((len == 0) || (len >= SV_NAMESZ)) return false;

	if (strstr(moduleName, "=end=") || strstr(moduleName, "=begin=")) return false; 
			
	byte macModule[6];
	byte ipModule[4];
	
	strcpy(module_, moduleName);
	parseBytes(macAddrModule, '-', macModule, 6, 16);
	parseBytes(ipAddrModule, '.', ipModule, 4, 10);
	parseBytes(ipAddrServ, '.', ipServer_, 4, 10);
	portServ_ = port;

	Ethernet.begin(macModule, ipModule);
		
	if (!ethClient_) ethClient_ = new EthernetClient();	
		
	for (int i = 0; i < 10; ++i){
	
	   isConnect_ = ethClient_->connect(ipServer_, portServ_);
	   if (isConnect_) break;	   
	   
	   delay(1000); 	     
	}
						
	if (isConnect_ ){
		
		MsTimer2::set(SV_CYCLEREC_MS, thrProc);
		MsTimer2::start();
	}
	
	return isConnect_;
  }
  
   bool sv_client::connectOfWiFi(const char* moduleName, const char* ssid, const char* pass, const char* ipAddrServ, int port){

	if (isConnect_) return true;

	int len = strlen(moduleName);
	if ((len == 0) || (len >= SV_NAMESZ)) return false;
	
	if (strstr(moduleName, "=end=") || strstr(moduleName, "=begin=")) return false; 
			
	strcpy(module_, moduleName);
	parseBytes(ipAddrServ, '.', ipServer_, 4, 10);
	portServ_ = port;
	
    for (int i = 0; i < 3; ++i){
	
		if (WiFi.begin(ssid, pass) == WL_CONNECTED){
			isConnect_ = true;
			break;
		}
		
		delay(10000);
	}
			
	if (!isConnect_) return false;		

	isWiFi_ = true;	
		
    ssid_ = realloc(ssid_, strlen(ssid) + 1); strcpy(ssid_, ssid);
	pass_ = realloc(pass_, strlen(pass) + 1); strcpy(pass_, pass);
		
	if (!ethClient_) ethClient_ = new WiFiClient();
		
	for (int i = 0; i < 10; ++i){
	
	   isConnect_ = ethClient_->connect(ipServer_, portServ_);
	   if (isConnect_) break;	   
	   
	   delay(1000); 	     
	}
							
	if (isConnect_ ){
		
		MsTimer2::set(SV_CYCLEREC_MS, thrProc);
		MsTimer2::start();
	}
	
	return isConnect_;
  }
   
   bool sv_client::connectOfCOM(const char* moduleName, int speed){
  		
	    if (isConnect_) return true;

		int len = strlen(moduleName);
		if ((len == 0) || (len >= SV_NAMESZ)) return false;
		
		if (strstr(moduleName, "=end=") || strstr(moduleName, "=begin=")) return false; 
					
		strcpy(module_, moduleName);
		
		Serial.begin(speed);
		
		isConnect_  = true;
		isCom_ = true;
			
		MsTimer2::set(SV_CYCLEREC_MS, thrProc);
		MsTimer2::start();
				
		return isConnect_;
   }
   
  bool sv_client::addValue(const char* name, valueType type, value val, bool onlyPosFront){

		if (!isConnect_) return false;
				
		valueRec* vr = values_.find(name);
		if (!vr){
					
			int sz = values_.size();
			if ((sz + 1) > SV_VALS_MAX_CNT)	return false;	
					
			int len = strlen(name);
			if ((len == 0) || (len >= SV_NAMESZ)) return false;
			
			if (strstr(name, "=end=") || strstr(name, "=begin=")) return false; 
			
			vr = new valueRec();
			vr->name = malloc(len + 1); strcpy(vr->name, name);
			vr->type = type;
			memset(vr->vals, 0, sizeof(value) * SV_PACKETSZ);

			values_.insert(vr->name, vr);				
		}
		
		vr->vals[curCycCnt_] = val;
		vr->isActive = true;
		vr->vals[curCycCnt_] = val; // на случай прерывания в этом месте как раз
		vr->isOnlyFront = onlyPosFront;
		
	    return true;	
	}
	
  bool sv_client::sendDataOfEthernet(){
     
	 bool ok = true;
	 
	 ethClient_->flush();		
	 if (ethClient_->connected()) {

		int sz = values_.size();
		
		if (sz == 0) return ok;
		
        char* start = "=begin=";
		ethClient_->write(start, strlen(start));
		
		int vlSz = sizeof(dataRef); long int allSz = SV_NAMESZ + vlSz * sz;
		ethClient_->write((char*)&allSz, 4);
		
		ethClient_->write(module_, SV_NAMESZ);
		
		valueRec* data; dataRef auxSD; memset(auxSD.vals, 0, 4 * SV_PACKETSZ);
		for (int i = 0; i < sz; ++i){
		
			data = (valueRec*)values_.at(i);
			strcpy(auxSD.name, data->name);
			auxSD.type = (long int)data->type;
			for (int j = 0; j < SV_PACKETSZ; ++j){
			   auxSD.vals[j] = data->vals[j].tInt;
			}
			ethClient_->write((char*)&auxSD, vlSz);  							
		}
		
		char* end = "=end=";
		ethClient_->write(end, strlen(end));	
				
	 }
	 else {
		 ethClient_->stop();
		
		 ok = false;
	 }
	 
	 return ok;
  }

  bool sv_client::sendDataOfCom(){
      
	int sz = values_.size();
	
	if (sz == 0) return true;
	
	Serial.flush();
		
	char* start = "=begin=";
	Serial.write(start, strlen(start));
	
	int vlSz = sizeof(dataRef); long int allSz = SV_NAMESZ + vlSz * sz;
	Serial.write((char*)&allSz, 4);
	
	Serial.write(module_, SV_NAMESZ);
		
	valueRec* data; dataRef auxSD; memset(auxSD.vals, 0, 4 * SV_PACKETSZ);
	for (int i = 0; i < sz; ++i){
	
		data = (valueRec*)values_.at(i);
		strcpy(auxSD.name, data->name);
		auxSD.type = (long int)data->type;
		for (int j = 0; j < SV_PACKETSZ; ++j){
		   auxSD.vals[j] = data->vals[j].tInt;
		}
		Serial.write((char*)&auxSD, vlSz);  							
	}
	
	char* end = "=end=";
	Serial.write(end, strlen(end));	
			 	 
	 return true;
  }
	
  bool sv_client::sendData(){

    bool ok = false;
	if (isCom_) ok = sendDataOfCom();  
    else ok = sendDataOfEthernet();
   
	return ok; 
  }
  
  void sv_client::doSendCyc(){
	   
	  // connection support
	  if (!isConnect_)
		  isConnect_ = tcpConnect();
	
	  // check active value in current cycle
	  int prevCyc = curCycCnt_ - 1; if (prevCyc < 0) prevCyc = SV_PACKETSZ - 1;
	  int sz = values_.size(); valueRec* vr;
	  for (int i = 0; i < sz; ++i){

		  vr = (valueRec*)values_.at(i);

		  if (!vr->isActive){
			  vr->vals[curCycCnt_] = vr->vals[prevCyc];

			  if ((vr->type == valueType::tBool) && vr->isOnlyFront)
				  vr->vals[curCycCnt_].tBool = false;
		  }

		  vr->isActive = false;
	  }

	  int next = curCycCnt_ + 1;
 
	  // send data
	  if (next >= SV_PACKETSZ) {

		  isConnect_ = (isConnect_) ? sendData() : false;

		  curCycCnt_ = 0;
	  }
	  else ++curCycCnt_;
	 
	 
	}

}

