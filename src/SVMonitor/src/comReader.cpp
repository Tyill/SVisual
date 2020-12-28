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

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "stdafx.h"
#include "comReader.h"
#include "SVConfig/SVConfigLimits.h"


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

  isConnect_ = pSerialPort_->open(QIODevice::ReadOnly);
  
  connect(pSerialPort_, &QSerialPort::readyRead, this, &SerialPortReader::hReadData);

  connect(pSerialPort_, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
    this, &SerialPortReader::hError);
      
  if (!tmCheckConnect_){
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

  return true;
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
