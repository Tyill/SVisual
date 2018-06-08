#pragma once

#include "stdafx.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialPortReader : public QObject
{
	Q_OBJECT

public:

	struct config{

		QString name;
		int speed = 9600;
		int cycleRecMs;           ///< период записи - задает пользователь
		int packetSz;             ///< размер пакета - задает пользователь

		config(QString name_, int speed_, int cycleRecMs_, int packetSz_) :
			name(name_), speed(speed_), cycleRecMs(cycleRecMs_), packetSz(packetSz_){}
	};

	SerialPortReader(config);
	~SerialPortReader();

	bool startServer();
	void stopServer();

	bool isRunning();

	/// задать польз callback - получение данных
	typedef void(*dataCBack)(std::string &inout, std::string &out);
	void setDataCBack(dataCBack uf);

public slots:
	void hReadData();
	void hError(QSerialPort::SerialPortError serialPortError);
	void disconnect();

private:

	dataCBack ufReceiveData_ = nullptr;

	QSerialPort* pSerialPort_ = nullptr;
	std::string readData_;

	QTimer* tmCheckConnect_ = nullptr;
	const int checkConnTOut = 5;  // циклов для проверки

	bool isConnect_ = false;
	
	config cng;
		
};
