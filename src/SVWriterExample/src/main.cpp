
#include "SVAuxFunc/auxFunc.h"
#include "SVAuxFunc/serverTCP.h"
#include "SVServer/SVServer.h"
#include <iostream>

using  namespace std;

void statusMess(const string& mess){

    cout << mess << endl;
}

int main(int argc, char* argv[]){

    SV_TcpSrv::setErrorCBack(statusMess);
    SV_Srv::setStatusCBack(statusMess);

    SV_TcpSrv::setDataCBack(SV_Srv::receiveData);

    SV_Srv::config scng;
    scng.outArchiveEna = true;

	string addr = argc > 1 ? argv[1] : "127.0.0.1";
	int port = argc > 2 ? atoi(argv[2]) : 2144;

	if (SV_Srv::startServer(scng) && SV_TcpSrv::runServer(addr, port)){
		statusMess("Run " + addr + " " + to_string(port));
		while (true)
			SV_Aux::SleepMs(1000);
	}
	else
	{
		statusMess("No run " + addr + " " + to_string(port));
		cin.get();
		
	}
    return 0;
}

