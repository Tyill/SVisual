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
#pragma once

#include <string>
#include <QObject>

class QSerialPort;
class MainWin;

class SerialPortReader : public QObject
{
  Q_OBJECT

public:

  struct Config{

    QString name;
    int speed = 9600;
    int cycleRecMs;           ///< период записи - задает пользователь
    int packetSz;             ///< размер пакета - задает пользователь

    Config(QString name_, int speed_, int cycleRecMs_, int packetSz_) :
      name(name_), speed(speed_), cycleRecMs(cycleRecMs_), packetSz(packetSz_){}
  };

  SerialPortReader(const Config&, MainWin* parent = nullptr);

  bool start();
  void stop();

  /// задать польз callback - получение данных  
  using DataCBack = std::function<void(std::string &inout, std::string &out)>;
  void setDataCBack(DataCBack uf);

public slots:
  void hReadData();

private:

  DataCBack ufReceiveData_ = nullptr;

  QSerialPort* pSerialPort_ = nullptr;
  std::string readData_;
    
  bool isConnect_ = false;

  Config cng;
};
