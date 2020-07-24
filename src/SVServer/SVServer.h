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

#include "SVConfig/SVConfigData.h"
#include "SVConfig/SVConfigLimits.h"

#ifdef _WIN32
   #ifdef SVSRVDLL_EXPORTS
     #define SVSRV_API __declspec(dllexport)
   #else
     #define SVSRV_API __declspec(dllimport)
   #endif
#else
   #define SVSRV_API
#endif


namespace SV_Srv {

    /// конфиг
    struct config {

        bool outArchiveEna;          ///< запись архива активна
        std::string outArchivePath;  ///< запись архива путь
        std::string outArchiveName;  ///< запись архива имя файла
        int outArchiveHourCnt;       ///< запись архива размер файла, час

        int cycleRecMs;              ///< период записи - задает пользователь
        int packetSz;                ///< размер пакета - задает пользователь

        config() :
                outArchiveEna(false),
                outArchivePath(""),
                outArchiveName("svrec"),
                outArchiveHourCnt(2),
                cycleRecMs(100),
                packetSz(10) {}
    };
        
    /// задать статус callback
    /// \param pfStatusMess
    typedef void(*statusCBack)(const std::string& mess);
    SVSRV_API void setStatusCBack(statusCBack stsCBack);

    /// старт сервера
    SVSRV_API bool startServer(config);

    /// стоп сервера
    SVSRV_API void stopServer();

    /// задать конфиг
    SVSRV_API void setConfig(config);

    // получение данных сервером
    SVSRV_API void receiveData(std::string& inout, std::string& out);

    // обновление данных callBack
    typedef void(*onUpdateSignalsCBack)();
    SVSRV_API void setOnUpdateSignalsCBack(onUpdateSignalsCBack);

    // добавление сигнала callBack
    typedef void(*onAddSignalsCBack)();
    SVSRV_API void setOnAddSignalsCBack(onAddSignalsCBack);
        
    // модуль подключен
    typedef void(*onModuleConnectCBack)(const std::string& module);
    SVSRV_API void setOnModuleConnectCBack(onModuleConnectCBack);

    // модуль отключен
    typedef void(*onModuleDisconnectCBack)(const std::string& module);
    SVSRV_API void setOnModuleDisconnectCBack(onModuleDisconnectCBack);

    // вернуть все модули
    SVSRV_API std::map<std::string, SV_Cng::moduleData*> getCopyModuleRef();

    // вернуть данные модуля
    SVSRV_API SV_Cng::moduleData* getModuleData(const std::string& module);

    // вернуть сигналы модуля
    SVSRV_API std::vector<std::string> getModuleSignals(const std::string& module);

    // вернуть все сигналы
    // key = name + module
    SVSRV_API std::map<std::string, SV_Cng::signalData*> getCopySignalRef();

    /// вернуть данные сигнала
    /// \param sign = name + module
    /// \return
    SVSRV_API SV_Cng::signalData* getSignalData(const std::string& sign);

    /// добавить сигнал
    SVSRV_API bool addSignal(SV_Cng::signalData*);

    /// добавить модуль
    SVSRV_API bool addModule(SV_Cng::moduleData*);

    /// разрешить буферизацию сигнала
    /// \param sign = name + module
    /// \return true - ok
    SVSRV_API bool signalBufferEna(const std::string& sign);

}

