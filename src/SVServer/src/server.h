
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
	SV_Srv::onTriggerCBack pfTriggerCBack = nullptr;
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

    // вернуть все триггеры
    std::map<std::string, SV_Cng::triggerData*> getCopyTriggerRef();

    // вернуть данные триггера
    SV_Cng::triggerData *getTriggerData(const std::string& trg);

    // добавить триггер
    bool addTrigger(const std::string& name, SV_Cng::triggerData* td);

    // удалить триггер
    bool delTrigger(const std::string& name);

private:

    SV_Srv::config cng;

    bool isRun_ = false;

    bufferData* pBuffData_ = nullptr;
    thrUpdSignal* pthrUpdSignal_ = nullptr;
    thrUpdTrigger* pthrUpdTrigger_ = nullptr;

    std::map < std::string, SV_Cng::moduleData * > moduleData_;
    std::map < std::string, SV_Cng::signalData * > signalData_;
    std::map < std::string, SV_Cng::triggerData * > triggerData_;

    std::mutex mtx_;

    bool jsonCheckRequest(rapidjson::Document& doc);
    void jsonRequestData(std::string& in, std::string& out);
    std::string jsonGetError();
    std::string jsonGetAllSignals();
    std::string jsonGetSignalData(std::string sign, std::string mod);
};
