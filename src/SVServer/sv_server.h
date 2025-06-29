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

#ifdef _WIN32
#ifdef SVSERVERDLL_EXPORTS
#define SV_API __declspec(dllexport)
#else
#define SV_API __declspec(dllimport)
#endif
#else
#define SV_API
#endif

#include "SVBase/base.h"
#include <map>
#include <functional>

namespace SV_Srv {

  /// конфиг
  struct Config {

    bool outArchiveEna{};          ///< запись архива активна
    std::string outArchivePath;  ///< запись архива путь
    std::string outArchiveName;  ///< запись архива имя файла
    int outArchiveHourCnt;       ///< запись архива размер файла, час

    bool outDataBaseEna{};         ///< запись архива в БД активна
    std::string outDataBaseName;
    std::string outDataBaseAddr;

    int cycleRecMs;              ///< период записи - задает пользователь
    int packetSz;                ///< размер пакета - задает пользователь
    int offsetMs;                ///< сдвиг вперед по времени, мс - задает пользователь для записи с частотой > 1кГц

    Config() :
      outArchiveEna(false),
      outArchiveName("svrec"),
      outArchiveHourCnt(2),
      outDataBaseEna(false),
      outDataBaseName("svdb"),
      outDataBaseAddr("localhost:9000"),
      cycleRecMs(100),
      packetSz(10),
      offsetMs(0) {}
  };

  /// задать статус callback
  /// \param pfStatusMess
  using statusCBack = std::function<void(const std::string&)>;
  SV_API void setStatusCBack(statusCBack stsCBack);

  /// старт сервера
  SV_API bool startServer(const Config&);

  /// стоп сервера
  SV_API void stopServer();

  /// задать конфиг
  SV_API void setConfig(const Config&);

  // получение данных сервером
  SV_API void receiveData(std::string& inout, std::string& out);

  // обновление данных callBack
  using onUpdateSignalsCBack = std::function<void()>;
  SV_API void setOnUpdateSignalsCBack(onUpdateSignalsCBack);

  // добавление сигнала callBack
  using onAddSignalsCBack = std::function<void()>;
  SV_API void setOnAddSignalsCBack(onAddSignalsCBack);

  // модуль подключен
  using onModuleConnectCBack = std::function<void(const std::string& module)>;
  SV_API void setOnModuleConnectCBack(onModuleConnectCBack);

  // модуль отключен
  using onModuleDisconnectCBack = std::function<void(const std::string& module)>;
  SV_API void setOnModuleDisconnectCBack(onModuleDisconnectCBack);

  // вернуть все модули
  SV_API std::map<std::string, SV_Base::ModuleData*> getCopyModuleRef();

  // вернуть данные модуля
  SV_API SV_Base::ModuleData* getModuleData(const std::string& module);

  // вернуть сигналы модуля
  SV_API std::vector<std::string> getModuleSignals(const std::string& module);

  // вернуть все сигналы
  // key = name + module
  SV_API std::map<std::string, SV_Base::SignalData*> getCopySignalRef();

  /// вернуть данные сигнала
  /// \param sign = name + module
  /// \return
  SV_API SV_Base::SignalData* getSignalData(const std::string& sign);

  SV_API void lockReadSData();
  SV_API void unlockReadSData();
  SV_API void lockWriteSData();
  SV_API void unlockWriteSData();

  /// добавить сигнал
  SV_API bool addSignal(SV_Base::SignalData*);

  /// добавить модуль
  SV_API bool addModule(SV_Base::ModuleData*);

  /// разрешить буферизацию сигнала
  /// \param sign = name + module
  /// \return true - ok
  SV_API bool signalBufferEna(const std::string& sign);

}

