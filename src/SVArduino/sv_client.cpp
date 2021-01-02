
#include <stdlib.h>
#include <string.h>
#include <Ethernet.h>
#include <WiFi.h>

#include "MsTimer2.h"
#include "SVClient.h"
#include "sv_structurs.h"
#include "sv_binTree.h"

namespace svisual{

  bool isConnect_ = false, 
       isWiFi_ = false,
       isCom_ = false;
         
  char module_[SV_NAMESZ]{0};
  
  byte ipServer_[4]{ 0, 0, 0, 0 };
  
  char* ssid_ = NULL,* pass_ = NULL;
  
  int portServ_ = 1330;
  int curCycCnt_ = 0; 

  Client* ethClient_ = NULL;
  
  Map values_;  

  void doSendCyc();

  void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
    for (int i = 0; i < maxBytes; ++i) {
      bytes[i] = strtoul(str, NULL, base);  
      str = strchr(str, sep);               
      if (str == NULL || *str == '\0') {
        break;                            
      }
      ++str;                                
    }
  }

  bool connectOfEthernet(const char* moduleName, const char* macAddrModule, const char* ipAddrModule, const char* ipAddrServ, int port){
        
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
      
    if (!ethClient_) 
      ethClient_ = new EthernetClient();  
      
    for (int i = 0; i < 10; ++i){    
      isConnect_ = ethClient_->connect(ipServer_, portServ_);
      if (isConnect_) break;           
      delay(1000);        
    }
              
    if (isConnect_ ){      
      MsTimer2::set(SV_CYCLEREC_MS, doSendCyc);
      MsTimer2::start();
    }
    
    return isConnect_;
  }
  
  bool connectOfWiFi(const char* moduleName, const char* ssid, const char* pass, const char* ipAddrServ, int port){
    
    if (isConnect_) return true;

    int len = strlen(moduleName);
    if ((len == 0) || (len >= SV_NAMESZ) ||    
        strstr(moduleName, "=end=") || strstr(moduleName, "=begin=")) return false; 
        
    strcpy(module_, moduleName);
    parseBytes(ipAddrServ, '.', ipServer_, 4, 10);
    portServ_ = port;
    
    for (int i = 0; i < 3; ++i){    
      if (WiFi.begin((char*)ssid, pass) == WL_CONNECTED){
        isConnect_ = true;
        break;
      }      
      delay(10000);
    }        
    if (!isConnect_) return false;    

    isWiFi_ = true;  
    ssid_ = (char*)realloc((void*)ssid_, strlen(ssid) + 1); 
    strcpy(ssid_, ssid);
    
    pass_ = (char*)realloc((void*)pass_, strlen(pass) + 1);
    strcpy(pass_, pass);
      
    if (!ethClient_)
      ethClient_ = new WiFiClient();
      
    for (int i = 0; i < 10; ++i){
      isConnect_ = ethClient_->connect(ipServer_, portServ_);
      if (isConnect_) break;     
      delay(1000);        
    }
                
    if (isConnect_ ){
      MsTimer2::set(SV_CYCLEREC_MS, doSendCyc);
      MsTimer2::start();
    }
    
    return isConnect_;
  }
      
  bool connectOfCOM(const char* moduleName, int speed){
  
    if (isConnect_) return true;

    int len = strlen(moduleName);
    if ((len == 0) || (len >= SV_NAMESZ) ||
        strstr(moduleName, "=end=") || strstr(moduleName, "=begin=")) return false; 
          
    strcpy(module_, moduleName);
    
    Serial.begin(speed);
    
    isConnect_  = true;
    isCom_ = true;
      
    MsTimer2::set(SV_CYCLEREC_MS, doSendCyc);
    MsTimer2::start();
        
    return isConnect_;
  }
  
  bool addValue(const char* name, ValueType type, Value val, bool onlyPosFront);

  bool addBoolValue(const char* name, bool vl, bool onlyPosFront){
    Value val; 
    val.tBool = vl;
    return addValue(name, ValueType::tBool, val, onlyPosFront);
  }
  
  bool addIntValue(const char* name, int vl){
    Value val;
    val.tInt = vl;
    return addValue(name, ValueType::tInt, val, false);  
  }
  
  bool addFloatValue(const char* name, float vl){
    Value val; 
    val.tFloat = vl;
    return addValue(name, ValueType::tFloat, val, false);  
  }
           
  bool addValue(const char* name, ValueType type, Value val, bool onlyPosFront){

    if (!isConnect_) return false;
        
    ValueRec* vr = (ValueRec*)values_.find(name);
    if (!vr){
      int sz = values_.size();
      if ((sz + 1) > SV_VALS_MAX_CNT)  return false;  
          
      int len = strlen(name);
      if ((len == 0) || (len >= SV_NAMESZ) ||      
          strstr(name, "=end=") || strstr(name, "=begin=")) return false; 
      
      vr = new ValueRec();
      vr->name = (char*)malloc(len + 1); 
      strcpy(vr->name, name);
      
      vr->type = type;
      memset(vr->vals, 0, sizeof(Value) * SV_PACKETSZ);

      values_.insert(vr->name, vr);        
    }
    
    vr->vals[curCycCnt_] = val;
    vr->isOnlyFront = onlyPosFront;
    vr->isActive = true;
	
    return true;  
  }
  
  bool sendDataOfEthernet(){
     
    bool ok = true;
    
    ethClient_->flush();    
    if (ethClient_->connected()) {

      int sz = values_.size();
      if (sz == 0) return ok;
      
      const char* start = "=begin=";
      ethClient_->write((const uint8_t*)start, strlen(start));
      
      int vlSz = sizeof(DataRef);
      long int allSz = SV_NAMESZ + vlSz * sz;
      ethClient_->write((const uint8_t*)&allSz, 4);
      
      ethClient_->write((const uint8_t*)module_, SV_NAMESZ);
      
      ValueRec* data; 
      DataRef auxSD; 
      memset(auxSD.vals, 0, 4 * SV_PACKETSZ);
      for (int i = 0; i < sz; ++i){
        data = (ValueRec*)values_.at(i);
        strcpy(auxSD.name, data->name);
        auxSD.type = (long int)data->type;
        for (int j = 0; j < SV_PACKETSZ; ++j){
          if ((data->type == ValueType::tInt) || (data->type == ValueType::tBool))
            auxSD.vals[j].tInt = data->vals[j].tInt;
          else
            auxSD.vals[j].tFloat = data->vals[j].tFloat;
        }        
        ethClient_->write((const uint8_t*)&auxSD, vlSz);                
      }      
      const char* end = "=end=";
      ethClient_->write((const uint8_t*)end, strlen(end));            
    }
    else {
      ethClient_->stop();      
      ok = false;
    }    
    return ok;
  }

  bool sendDataOfCom(){
      
    int sz = values_.size();
    if (sz == 0) return true;
    
    Serial.flush();
      
    const char* start = "=begin=";
    Serial.write(start, strlen(start));
    
    int vlSz = sizeof(DataRef); 
    long int allSz = SV_NAMESZ + vlSz * sz;
    Serial.write((char*)&allSz, 4);
    
    Serial.write(module_, SV_NAMESZ);
      
    ValueRec* data; 
    DataRef auxSD; 
    memset(auxSD.vals, 0, 4 * SV_PACKETSZ);
    for (int i = 0; i < sz; ++i){
    
      data = (ValueRec*)values_.at(i);
      strcpy(auxSD.name, data->name);
      auxSD.type = (long int)data->type;
      for (int j = 0; j < SV_PACKETSZ; ++j){
        if ((data->type == ValueType::tInt) || (data->type == ValueType::tBool))
          auxSD.vals[j].tInt = data->vals[j].tInt;
        else  
          auxSD.vals[j].tFloat = data->vals[j].tFloat;          
      }
      
      Serial.write((char*)&auxSD, vlSz);                
    }
    
    const char* end = "=end=";
    Serial.write(end, strlen(end));  
          
    return true;
  }
  
  bool sendData(){
    bool ok = isCom_ ? sendDataOfCom() : sendDataOfEthernet();
    return ok; 
  }
  
  bool tcpConnect(){
    if (isWiFi_)
      WiFi.begin(ssid_, pass_);
    return ethClient_->connect(ipServer_, portServ_);
  }

  void doSendCyc(){
     
    // connection support
    if (!isConnect_)
      isConnect_ = tcpConnect();
  
    // check active Value in current cycle
    int prevCyc = curCycCnt_ - 1; if (prevCyc < 0) prevCyc = SV_PACKETSZ - 1;
    int sz = values_.size(); ValueRec* vr;
    for (int i = 0; i < sz; ++i){

      vr = (ValueRec*)values_.at(i);

      if (!vr->isActive){
        vr->vals[curCycCnt_] = vr->vals[prevCyc];
        if ((vr->type == ValueType::tBool) && vr->isOnlyFront)
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
    else 
      ++curCycCnt_;    
  }
}

