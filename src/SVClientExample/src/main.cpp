
#include <string>
#include <iostream>
#include "SVClient/SVClient.h"
#include "SVAuxFunc/auxFunc.h"

int main(int argc, char *argv[]){

	std::string nm = argc >= 2 ? argv[1] : "client";

	int cyc = argc >= 3 ? atoi(argv[2]) : 100;
	int packSz = argc >= 4 ? atoi(argv[3]) : 10;

	SV_Cl::sv_setParam(cyc, packSz);

	if (SV_Cl::sv_connect(nm.c_str(), "127.0.0.1", 2144)){

		std::cout << "connect " << nm.c_str() << " ok" << std::endl;

		for (int i = 0; i < 100; ++i){

			std::string val = "абрвал" + std::to_string(i);
			SV_Cl::sv_addIntValue(val.c_str(), 0);

			val = "front" + std::to_string(i);
			SV_Cl::sv_addBoolValue(val.c_str(), true);
		}
	}
	else std::cout << "connect no" << std::endl;

	int cp = 0;
	while (true){

		for (int i = 0; i < 100; i++){
				
			std::string val = "dde" + std::to_string(i);
			SV_Cl::sv_addIntValue(val.c_str(), cp);

			val = "front" + std::to_string(i);
			SV_Cl::sv_addBoolValue(val.c_str(), cp%2);
		}
		
		cp += 1; if (cp > 100) cp = -100;
			
		SV_Aux::SleepMs(cyc);
	}
	return 0;
}