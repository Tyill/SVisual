
#include <functional>
#include "stdafx.h"
#include "SVServer/SVServer.h"
#include "server.h"

using namespace std;

server serv;

namespace SV_Srv {

	/// задать статус callback
	/// \param pfStatusMess
	void setStatusCBack(statusCBack cback){

		serv.pfStatusCBack = cback;
	}

    /// старт сервера
    bool startServer(config cng){

        return serv.startServer(cng);
    }

    /// стоп сервера
    void stopServer(){

		serv.stopServer();
    }

    /// задать конфиг
    void setConfig(config cng){

        serv.setConfig(cng);
	}

    // получение данных сервером callBack
    void receiveData(std::string& inout, std::string& out){

		serv.receiveData(inout, out);
	}

    // обновление данных callBack
    void setOnUpdateSignalsCBack(onUpdateSignalsCBack cback){

		serv.pfUpdateSignalsCBack = cback;
    }

    // добавление сигнала callBack
    void setOnAddSignalsCBack(onAddSignalsCBack cback){

		serv.pfAddSignalsCBack = cback;
	}

    // сработал триггер callBack
    void setOnTriggerCBack(onTriggerCBack cback){

		serv.pfTriggerCBack = cback;
	}

    void setOnModuleConnectCBack(onModuleConnectCBack cback){

        serv.pfModuleConnectCBack = cback;
	}

    void setOnModuleDisconnectCBack(onModuleDisconnectCBack cback){

        serv.pfModuleDisconnectCBack = cback;
    }

    // вернуть все модули
    std::map<std::string, SV_Cng::moduleData *> getCopyModuleRef(){

		return serv.getCopyModuleRef();
    };

    // вернуть данные модуля
    SV_Cng::moduleData* getModuleData(const std::string& module){

		return serv.getModuleData(module);
    }

    // вернуть все сигналы
    std::map<std::string, SV_Cng::signalData *> getCopySignalRef(){

		return serv.getCopySignalRef();
    };

    // вернуть данные сигнала
    SV_Cng::signalData *getSignalData(const std::string& sign){

		return serv.getSignalData(sign);
    }

    bool signalBufferEna(const std::string& sign){

        return serv.signalBufferEna(sign);
    }


    // вернуть все триггеры
	std::map<std::string, SV_Cng::triggerData*> getCopyTriggerRef(){

		return serv.getCopyTriggerRef();
    }

    // вернуть данные триггера
    SV_Cng::triggerData *getTriggerData(const string& name){

		return serv.getTriggerData(name);
    }

    // добавить триггер
    bool addTrigger(const string& name, SV_Cng::triggerData* td){

		return serv.addTrigger(name, td);
    }

    // удалить триггер
    bool delTrigger(const string& name){

		return serv.delTrigger(name);
    }
}