
#pragma once

#include <string>

namespace SV_TcpSrv {

/// запустить сервер
/// \param addr IP адрес
/// \param port порт
/// \param keepAlive не обывать связь
/// \param noReceive не отвечать
/// \param tout ждать после потери связи
/// \return true - ok
    bool runServer(std::string addr, int port, bool keepAlive = false, int tout = 60);

/// остановить сервер
    void stopServer();

/// задать польз callback - получение данных
    typedef void(*dataCBack)(std::string &inout, std::string &out);
    void setDataCBack(dataCBack uf);

/// задать польз callback - ошибка сервера
    typedef void(*errorCBack)(const std::string &mess);
    void setErrorCBack(errorCBack uf);
}
