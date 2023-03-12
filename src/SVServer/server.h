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

#include "SVBase/base.h"
#include <map>
#include <functional>

namespace SV_Srv {

  /// конфиг
  struct Config {

    bool outArchiveEna;          ///< запись архива активна
    std::string outArchivePath;  ///< запись архива путь
    std::string outArchiveName;  ///< запись архива имя файла
    int outArchiveHourCnt;       ///< запись архива размер файла, час
                                 
    bool outDataBaseEna;         ///< запись в БД активна
    std::string outDataBaseName;
    std::string outDataBaseAddr;

    int cycleRecMs;              ///< период записи - задает пользователь
    int packetSz;                ///< размер пакета - задает пользователь

    Config() :
      outArchiveEna(false),
      outArchiveName("svrec"),
      outArchiveHourCnt(2),
      outDataBaseEna(false),
      outDataBaseAddr("localhost:9000"),
      cycleRecMs(100),
      packetSz(10) {}
  };

  /// задать статус callback
  /// \param pfStatusMess
  using statusCBack = std::function<void(const std::string&)>;
  void setStatusCBack(statusCBack stsCBack);

  /// старт сервера
  bool startServer(const Config&);

  /// стоп сервера
  void stopServer();

  /// задать конфиг
  void setConfig(const Config&);

  // получение данных сервером
  void receiveData(std::string& inout, std::string& out);

  // обновление данных callBack
  using onUpdateSignalsCBack = std::function<void()>;
  void setOnUpdateSignalsCBack(onUpdateSignalsCBack);

  // добавление сигнала callBack
  using onAddSignalsCBack = std::function<void()>;
  void setOnAddSignalsCBack(onAddSignalsCBack);

  // модуль подключен
  using onModuleConnectCBack = std::function<void(const std::string& module)>;
  void setOnModuleConnectCBack(onModuleConnectCBack);

  // модуль отключен
  using onModuleDisconnectCBack = std::function<void(const std::string& module)>;
  void setOnModuleDisconnectCBack(onModuleDisconnectCBack);

  // вернуть все модули
  std::map<std::string, SV_Base::ModuleData*> getCopyModuleRef();

  // вернуть данные модуля
  SV_Base::ModuleData* getModuleData(const std::string& module);

  // вернуть сигналы модуля
  std::vector<std::string> getModuleSignals(const std::string& module);

  // вернуть все сигналы
  // key = name + module
  std::map<std::string, SV_Base::SignalData*> getCopySignalRef();

  /// вернуть данные сигнала
  /// \param sign = name + module
  /// \return
  SV_Base::SignalData* getSignalData(const std::string& sign);

  /// добавить сигнал
  bool addSignal(SV_Base::SignalData*);

  /// добавить модуль
  bool addModule(SV_Base::ModuleData*);

  /// разрешить буферизацию сигнала
  /// \param sign = name + module
  /// \return true - ok
  bool signalBufferEna(const std::string& sign);

}

