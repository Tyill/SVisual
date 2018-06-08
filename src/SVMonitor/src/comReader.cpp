

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "stdafx.h"
#include "comReader.h"
#include "SVConfig/SVConfigLimits.h"


void statusMess(QString mess);

SerialPortReader::SerialPortReader(SerialPortReader::config cng_) : cng(cng_)
{
			
}

SerialPortReader::~SerialPortReader()
{
	
}

bool SerialPortReader::startServer(){

	if (isConnect_ ) return true;

	pSerialPort_ = new QSerialPort(cng.name);

	pSerialPort_->setBaudRate((QSerialPort::BaudRate)cng.speed);
	pSerialPort_->setDataBits(QSerialPort::DataBits::Data8);
	pSerialPort_->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
	pSerialPort_->setParity(QSerialPort::Parity::NoParity);
	pSerialPort_->setStopBits(QSerialPort::StopBits::OneStop);

	if (pSerialPort_->open(QIODevice::ReadOnly))
	{
		connect(pSerialPort_, &QSerialPort::readyRead, this, &SerialPortReader::hReadData);

		connect(pSerialPort_, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
			this, &SerialPortReader::hError);
		
		isConnect_ = true;
	}
	else
	   statusMess(pSerialPort_->errorString());

	if (isConnect_ && !tmCheckConnect_){
		// проверка соединения
		tmCheckConnect_ = new QTimer(this);
		connect(tmCheckConnect_, &QTimer::timeout, [=]() {

			if (!pSerialPort_->isOpen())
				isConnect_ = pSerialPort_->open(QIODevice::ReadOnly);
			else{
				if (!isConnect_) pSerialPort_->close();
				else isConnect_ = false;
			}
		});

		tmCheckConnect_->start(SV_CYCLESAVE_MS * checkConnTOut);
	}

	return isConnect_;
}

void SerialPortReader::stopServer(){

	if (pSerialPort_ && pSerialPort_->isOpen()) pSerialPort_->close();
}

bool SerialPortReader::isRunning(){

	return pSerialPort_ && pSerialPort_->isOpen();
}

void SerialPortReader::disconnect(){

	if (pSerialPort_ && pSerialPort_->isOpen()) pSerialPort_->close();
}

void SerialPortReader::setDataCBack(dataCBack uf){
	
	ufReceiveData_ = uf;
}

void SerialPortReader::hReadData()
{	
	readData_ += pSerialPort_->readAll().toStdString();
	
	std::string out;
	if (ufReceiveData_)
		ufReceiveData_(readData_, out);

	isConnect_ = true;
}

void SerialPortReader::hError(QSerialPort::SerialPortError serialPortError)
{
	if (serialPortError == QSerialPort::ReadError)	
		statusMess(QString(tr("%1 Ошибка получения данных").arg(cng.name)));

}
