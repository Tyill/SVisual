//=============================================================================
// 
//=============================================================================

#pragma once

namespace svisual{
  
  class  sv_client{
  
  private:
    bool isConnect_ = false, isWiFi_ = false, isCom_ = false;
         
	char module_[SV_NAMESZ]{0};
	
	byte ipServer_[4]{ 0, 0, 0, 0 };
  
    char* ssid_ = NULL,* pass_ = NULL;
  
    int portServ_ = 1330;
	int curCycCnt_ = 0; 

	Client* ethClient_ = NULL;
	
	map values_;
	
	bool tcpConnect();

	bool sendDataOfEthernet();

	bool sendDataOfCom();

	bool sendData();
	    
  public:
  
    // connect to server
     bool connectOfEthernet(const char* module, const char* macAddrModule, const char* ipAddrModule, const char* ipAddrServ, int port);
	 
	 // connect to server
     bool connectOfWiFi(const char* module, const char* macAddrModule, const char* ipAddrModule, const char* ipAddrServ, int port);

	 // connect to server
 	 bool connectOfCOM(const char* module, int speed);
 		
    // add value for rec
	bool addValue(const char* name, valueType type, value val, bool onlyFront = false);
	
	void doSendCyc();
	
  };
}
