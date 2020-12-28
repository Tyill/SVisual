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
   
    void setOnModuleConnectCBack(onModuleConnectCBack cback){

        serv.pfModuleConnectCBack = cback;
  }

    void setOnModuleDisconnectCBack(onModuleDisconnectCBack cback){

        serv.pfModuleDisconnectCBack = cback;
    }

    // вернуть все модули
    std::map<std::string, SV_Base::ModuleData *> getCopyModuleRef(){

    return serv.getCopyModuleRef();
    };

    // вернуть данные модуля
    SV_Base::ModuleData* getModuleData(const std::string& module){

    return serv.getModuleData(module);
    }

    // вернуть сигналы модуля
    std::vector<std::string> getModuleSignals(const std::string& module){

        return serv.getModuleSignals(module);
    }

    // вернуть все сигналы
    std::map<std::string, SV_Base::SignalData *> getCopySignalRef(){

    return serv.getCopySignalRef();
    };

    // вернуть данные сигнала
    SV_Base::SignalData *getSignalData(const std::string& sign){

    return serv.getSignalData(sign);
    }

    /// добавить сигнал
    bool addSignal(SV_Base::SignalData* sd){

        return serv.addSignal(sd);
    }

    /// добавить модуль
    bool addModule(SV_Base::ModuleData* md){

        return serv.addModule(md);
    }

    // разрешить буфер сигнала
    bool signalBufferEna(const std::string& sign){

        return serv.signalBufferEna(sign);
    }

}