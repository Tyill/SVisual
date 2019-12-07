//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "SVAuxFunc/auxFunc.h"
#include "SVAuxFunc/serverTCP.h"
#include "SVServer/SVServer.h"
#include "SVWebServer/SVWebServer.h"

#include <iostream>

void statusMess(const std::string& mess){

	std::cout << mess << std::endl;
}

QMap<QString, SV_Cng::moduleData *> getCopyModuleRefSrv(){

	QMap<QString, SV_Cng::moduleData *> cmref;

	auto mref = SV_Srv::getCopyModuleRef();
	for (auto& s : mref)
		cmref.insert(QString::fromStdString(s.first), s.second);

	return cmref;
};

QMap<QString, SV_Cng::signalData *> getCopySignalRefSrv(){

	QMap<QString, SV_Cng::signalData*> csref;

	auto sref = SV_Srv::getCopySignalRef();
	for (auto& s : sref)
		csref.insert(QString::fromStdString(s.first), s.second);

	return csref;
};

SV_Cng::signalData* getSignalDataSrv(const QString& sing){

	return SV_Srv::getSignalData(sing.toUtf8().data());
};

struct config{

	bool outArchiveEna;        ///< запись архива активна
	std::string outArchivePath;    ///< запись архива путь
	std::string outArchiveName;    ///< запись архива имя файла
	int outArchiveHourCnt;     ///< запись архива размер файла, час

	int cycleRecMs;            ///< период записи - задает пользователь
	int packetSz;              ///< размер пакета - задает пользователь

	// связь по TCP
	std::string tcp_addr;          ///< ip
	int tcp_port;              ///< port

	// web
	bool web_ena;
	std::string web_addr;          ///< ip
	int web_port;              ///< port

};

void init(const std::string& initPath, config& cng){


	QSettings settings(QString::fromStdString(initPath), QSettings::IniFormat);
	settings.beginGroup("Param");

	cng.cycleRecMs =  settings.value("cycleRecMs", 100).toInt();
	cng.cycleRecMs = qMax(cng.cycleRecMs, 10);
	cng.packetSz = settings.value("packetSz", 10).toInt();
	cng.packetSz = qMax(cng.packetSz, 1);

	// связь по TCP
	cng.tcp_addr = settings.value("tcp_addr", "0.0.0.0").toString().toStdString();
	cng.tcp_port = settings.value("tcp_port", "2144").toInt();

	// web
	cng.web_ena = settings.value("web_ena", "1").toInt() == 1;
	cng.web_addr = settings.value("web_addr", "0.0.0.0").toString().toStdString();
	cng.web_port = settings.value("web_port", "2145").toInt();

	// копир на диск
	cng.outArchiveEna = settings.value("outArchiveEna", "1").toInt() == 1;
	cng.outArchivePath = settings.value("outArchivePath", "").toString().toStdString();

	cng.outArchiveName = settings.value("outFileName", "svrec").toString().toStdString();
	cng.outArchiveHourCnt = settings.value("outFileHourCnt", 2).toInt();
	cng.outArchiveHourCnt = qBound(1, cng.outArchiveHourCnt, 12);

	settings.endGroup();

}

int main(int argc, char* argv[]){

	if (argc == 1){
		statusMess("No ini path - arg[1]");
		return -1;
	}

  QCoreApplication a(argc, argv);

    SV_TcpSrv::setErrorCBack(statusMess);
    SV_TcpSrv::setDataCBack(SV_Srv::receiveData);

	SV_Srv::setStatusCBack(statusMess);

	config cng;

  init(argv[1], cng);

  SV_Srv::config scng;

	scng.cycleRecMs = cng.cycleRecMs;
	scng.packetSz = cng.packetSz;
	scng.outArchiveEna = cng.outArchiveEna;
	scng.outArchiveHourCnt = cng.outArchiveHourCnt;
	scng.outArchiveName = cng.outArchiveName;
	scng.outArchivePath = cng.outArchivePath;
  	
  if (SV_Srv::startServer(scng) && SV_TcpSrv::runServer(cng.tcp_addr, cng.tcp_port)){
		statusMess("TCP server run: " + cng.tcp_addr + " " + std::to_string(cng.tcp_port));
	}
	else{
		statusMess("TCP server not run: " + cng.tcp_addr + " " + std::to_string(cng.tcp_port));
		return -1;
	}

	if (cng.web_ena){

		SV_Web::setGetCopySignalRef(getCopySignalRefSrv);
		SV_Web::setGetSignalData(getSignalDataSrv);
		SV_Web::setGetCopyModuleRef(getCopyModuleRefSrv);

    if (SV_Web::startServer(QString::fromStdString(cng.web_addr), cng.web_port, SV_Web::config(SV_CYCLEREC_MS, SV_PACKETSZ)))
      statusMess("WEB server run: " + cng.web_addr + " " + std::to_string(cng.web_port));
		else
			statusMess("WEB server not run: " + cng.web_addr + " " + std::to_string(cng.web_port));
	}

  return a.exec();
}

