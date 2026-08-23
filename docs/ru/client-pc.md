[English](../en/client-pc.md) | [Русский](client-pc.md) | [Содержание](../ru/README.md)

# Клиент C++ (`SVClient`)

Заголовок: [`src/SVClient/SVClient.h`](../../src/SVClient/SVClient.h). Клиент **отправляет** значения, не подписывается и не читает их обратно.

```cpp
namespace SV {
  bool svConnect(const char *moduleName, const char *ipAddrServ, int portServ);
  void svDisconnect();
  bool svAddBoolValue(const char *name, bool Value, bool onlyFront = false);
  bool svAddIntValue(const char *name, int Value);
  bool svAddFloatValue(const char *name, float Value);
  bool svSetParam(int cycleRecMs, int packetSz);
}
```

Имена модуля и значений — не длиннее **24** символов, без подстрок `=begin=` и `=end=`.

Пример из [`src/SVClientExample/src/main.cpp`](../../src/SVClientExample/src/main.cpp): подключение к `127.0.0.1:2144` (TCP-порт Monitor по умолчанию), `svSetParam(100, 10)`.

```cpp
SV::svSetParam(100, 10);
if (SV::svConnect("client", "127.0.0.1", 2144)) {
  SV::svAddIntValue("sin", value);
  SV::svAddBoolValue("flag", value > 0);
}
```

`cycleRecMs` / `packetSz` должны совпадать с задуманными настройками сервера. Для частоты выше ~1 кГц задайте на сервере **смещение времени**.
