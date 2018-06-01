
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
        std::string outArchivePath;      ///< запись архива путь
        std::string outArchiveName;      ///< запись архива имя файла
        int outArchiveHourCnt;       ///< запись архива размер файла, час

        int cycleRecMs;           ///< период записи - задает пользователь
        int packetSz;             ///< размер пакета - задает пользователь

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

    // сработал триггер callBack
    typedef void(*onTriggerCBack)(const std::string& trigger);
    SVSRV_API void setOnTriggerCBack(onTriggerCBack);

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

    // вернуть все сигналы
    // key = name + module
    SVSRV_API std::map<std::string, SV_Cng::signalData *> getCopySignalRef();

    /// вернуть данные сигнала
    /// \param sign = name + module
    /// \return
    SVSRV_API SV_Cng::signalData* getSignalData(const std::string& sign);

    /// разрешить буферизацию сигнала
    /// \param sign = name + module
    /// \return true - ok
    SVSRV_API bool signalBufferEna(const std::string& sign);


    // вернуть все триггеры
    SVSRV_API std::map<std::string, SV_Cng::triggerData*> getCopyTriggerRef();

    // вернуть данные триггера
    SVSRV_API SV_Cng::triggerData *getTriggerData(const std::string&);

    // добавить триггер
    SVSRV_API bool addTrigger(const std::string& name, SV_Cng::triggerData* td);

    // удалить триггер
    SVSRV_API bool delTrigger(const std::string& name);

}

