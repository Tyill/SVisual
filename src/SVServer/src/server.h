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

#include "stdafx.h"
#include "bufferData.h"
#include <thread>
#include <mutex>
#include "SVServer/SVServer.h"
#include "Lib/rapidjson/document.h"

class thrUpdSignal;
class thrUpdTrigger;

class server{

    public:

	SV_Srv::statusCBack pfStatusCBack = nullptr;
	SV_Srv::onUpdateSignalsCBack pfUpdateSignalsCBack = nullptr;
	SV_Srv::onAddSignalsCBack pfAddSignalsCBack = nullptr;
    SV_Srv::onModuleConnectCBack pfModuleConnectCBack = nullptr;
    SV_Srv::onModuleDisconnectCBack pfModuleDisconnectCBack = nullptr;

    static const int BUFF_SIGN_HOUR_CNT = 2;  // жестко размер буфера, час


    /// старт сервера
    bool startServer(SV_Srv::config);

    /// стоп сервера
    void stopServer();

    /// задать конфиг
    void setConfig(SV_Srv::config);

    // получение данных
    void receiveData(std::string& inout, std::string& out);

    // вернуть все модули
    std::map<std::string, SV_Cng::moduleData *> getCopyModuleRef();

    // добавить модуль
    bool addModule(const std::string& name, SV_Cng::moduleData *);

    // вернуть данные модуля
    SV_Cng::moduleData* getModuleData(const std::string& module);


    // вернуть все сигналы
    std::map<std::string, SV_Cng::signalData *> getCopySignalRef();

    // добавить сигнал
    bool addSignal(const std::string& name, SV_Cng::signalData *);

    // загрузить данные
    bool signalBufferEna(const std::string& name);

    // вернуть данные сигнала
    SV_Cng::signalData* getSignalData(const std::string& sign);

   
private:

    SV_Srv::config cng;

    bool isRun_ = false;

    bufferData* pBuffData_ = nullptr;
    thrUpdSignal* pthrUpdSignal_ = nullptr;
   
    std::map < std::string, SV_Cng::moduleData*> moduleData_;
    std::map < std::string, SV_Cng::signalData*> signalData_;
    
    std::mutex mtx_;

    bool jsonCheckRequest(rapidjson::Document& doc);
    void jsonRequestData(std::string& in, std::string& out);
    std::string jsonGetError();
    std::string jsonGetAllSignals();
    std::string jsonGetSignalData(std::string sign, std::string mod);
};
