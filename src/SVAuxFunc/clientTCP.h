
#pragma once

namespace SV_TcpClnt {

/// клиент TCP подключение
/// \param addr IP адресс
/// \param port порт
/// \param noBlock Не ждать ответа
/// \return true - ok
    bool connect(std::string addr, int port, bool noBlock = false);

/// клиент TCP отключение
/// \return true - ok
    bool disconnect();

/// клиент Отправить данные
/// \param in данные
/// \param out ответ
/// \param disconn отключать после отправки
/// \param onlySend только отправка
/// \return true - ok
    bool sendData(std::string in, std::string &out, bool disconn = false, bool onlySend = false);
}

