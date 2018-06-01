//=============================================================================
// SVisual client for MK Arduino
//=============================================================================

#pragma once

namespace svisual{
  
    // connect to server of ethernet
      // module - name module(leng max 24)
	  // macAddrModule - mac addr module
	  // ipAddrModule - ipaddr module
	  // ipAddrServ - ipaddr server
	bool connectOfEthernet(const char* module, const char* macAddrModule, const char* ipAddrModule, const char* ipAddrServ, int portServ);
	
	// connect to server of wi-fi
      // module - name module (leng max 24)
	  // ssid - your network SSID
	  // pass - secret password network
	  // ipAddrServ - ipaddr server
	  // portServ - port server
	bool connectOfWiFi(const char* module, const char* ssid, const char* pass, const char* ipAddrServ, int portServ);
    
    // connect to server of COM
	  // module - name module (leng max 24)
	  // ipAddrServ - ipaddr server
	  // portServ - port server
    bool connectOfCOM(const char* module, int speed = 9600);
		
    // add bool value for rec
	// name - value name (leng max 24)
    bool addBoolValue(const char* name, bool value, bool onlyPosFront = false);
    
    // add int value for rec
	// name - value name (leng max 24)
    bool addIntValue(const char* name, int value);
	
};
