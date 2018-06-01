

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "stdafx.h"
#include "comReader.h"


void statusMess(QString mess);

SerialPortReader::SerialPortReader(SerialPortReader::config cng_) : cng(cng_)
{
			
}

SerialPortReader::~SerialPortReader()
{
}

bool SerialPortReader::startServer(){

	pSerialPort_ = new QSerialPort(cng.name);

	pSerialPort_->setBaudRate((QSerialPort::BaudRate)cng.speed);
	pSerialPort_->setDataBits(QSerialPort::DataBits::Data8);
	pSerialPort_->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
	pSerialPort_->setParity(QSerialPort::Parity::NoParity);
	pSerialPort_->setStopBits(QSerialPort::StopBits::OneStop);

	bool ok = false;

	if (pSerialPort_->open(QIODevice::ReadOnly))
	{
		connect(pSerialPort_, &QSerialPort::readyRead, this, &SerialPortReader::hReadData);

		connect(pSerialPort_, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
			this, &SerialPortReader::hError);
		
		ok = true;
	}
	else
	   statusMess(pSerialPort_->errorString());

	return ok;
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
}

void SerialPortReader::hError(QSerialPort::SerialPortError serialPortError)
{
	if (serialPortError == QSerialPort::ReadError)	
		statusMess(QString(tr("%1 Ошибка получения данных").arg(cng.name)));

}
