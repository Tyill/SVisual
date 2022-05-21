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

#include "SVBase/limits.h"
#include "com_reader.h"

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

void statusMess(const QString& mess);

SerialPortReader::SerialPortReader(const Config& cng_, QObject* parent):
  QObject(parent), cng(cng_){
}

bool SerialPortReader::start(){

  if (pSerialPort_) return true;
  
  pSerialPort_ = new QSerialPort(cng.name, this);

  pSerialPort_->setBaudRate((QSerialPort::BaudRate)cng.speed);
  pSerialPort_->setDataBits(QSerialPort::DataBits::Data8);
  pSerialPort_->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
  pSerialPort_->setParity(QSerialPort::Parity::NoParity);
  pSerialPort_->setStopBits(QSerialPort::StopBits::OneStop);

  isConnect_ = pSerialPort_->open(QIODevice::ReadOnly);

  connect(pSerialPort_, &QSerialPort::readyRead, this, &SerialPortReader::hReadData);

  connect(pSerialPort_, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
    [this](QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::ReadError)
      statusMess(QString(tr("%1 Ошибка получения данных").arg(cng.name)));
  });

  // проверка соединения  
  QTimer* tmCheckConnect = new QTimer(this);
  connect(tmCheckConnect, &QTimer::timeout, [=]() {
    if (!pSerialPort_->isOpen())
      isConnect_ = pSerialPort_->open(QIODevice::ReadOnly);
    else{
      if (!isConnect_) pSerialPort_->close();
      else isConnect_ = false;
    }
  });
  tmCheckConnect->start(SV_CYCLESAVE_MS * 5);
  
  return true;
}

void SerialPortReader::stop(){

  if (pSerialPort_ && pSerialPort_->isOpen()) pSerialPort_->close();
}

void SerialPortReader::setDataCBack(DataCBack uf){

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