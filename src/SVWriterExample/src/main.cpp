
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

	if (!SV_Srv::startServer(scng) || !SV_TcpSrv::runServer("127.0.0.1", 2144)){
		statusMess("No run 127.0.0.1 2144");
		cin.get();
	}
	else
	{
		statusMess("Run 127.0.0.1 2144");
		while (true)
			SV_Aux::SleepMs(1000);
	}
    return 0;
}

