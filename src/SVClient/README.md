# SVClient

Клиентская библиотека отправки сигналов SVisual: накопление значений по циклам, формирование бинарных пакетов и отправка на сервер по TCP.

Собирается как shared library (`libSVClient.so` / `SVClient.dll`), публичный API — `SVClient.h`.

## Сборка

```bash
cd src/build
cmake ..
cmake --build . --target SVClient
```

Зависимости: `SVBase`, `SVMisc` (`TCPClient`).

## Lifecycle

```
Idle ──svConnect()──► SessionActive ──svDisconnect()──► Idle
                           │
                           ├── sender thread running (always after first svConnect)
                           ├── _isConnect true/false (TCP state)
                           └── automatic reconnect in sender thread
```

| Метод | Поведение |
|-------|-----------|
| `svSetParam(cycleRecMs, packetSz)` | Только до первого `svAdd*Value` и до `svConnect`; иначе `false` |
| `svConnect(module, ip, port)` | Стартует sender thread один раз; возвращает результат **первого** TCP connect |
| `svConnect()` (повторный) | При активной сессии — `false`/`true` по текущему `_isConnect`, второй поток не создаётся |
| `svAdd*Value(name, …)` | Запись значения в текущий цикл; имя должно быть уникальным по типу |
| `svDisconnect()` | Останавливает поток, закрывает сокет; **обязателен** перед выгрузкой DLL / exit |

`false` от `svConnect()` **не означает**, что сессия не запущена: sender thread уже работает, reconnect идёт в фоне.

Порядок вызовов для типичного приложения:

```cpp
SV::svSetParam(100, 10);                          // до connect и add
SV::svConnect("myModule", "127.0.0.1", 2144);     // false ok — reconnect в фоне
while (running) {
    SV::svAddIntValue("temp", value);
    // ...
}
SV::svDisconnect();                               // обязательно перед unload
```

Пример: [`../SVClientExample/src/main.cpp`](../SVClientExample/src/main.cpp).

## Публичный API

| Функция | Назначение |
|---------|------------|
| `svSetParam(cycleRecMs, packetSz)` | Период записи (мс) и размер пакета (число циклов) |
| `svConnect(moduleName, ipAddr, port)` | Подключение и старт sender thread |
| `svDisconnect()` | Остановка потока и отключение |
| `svAddBoolValue(name, value, onlyFront)` | Bool-сигнал; `onlyFront` — импульс только в активном цикле |
| `svAddIntValue(name, value)` | Int-сигнал |
| `svAddFloatValue(name, value)` | Float-сигнал |

### Ограничения имён и параметров

- `moduleName`, `name` — non-null, длина `< 24` (`SV_NAMESZ`), без подстрок `"=begin="` и `"=end="`.
- `ipAddr` — non-null; на Linux только dotted-decimal IP (`inet_aton` в `SVMisc`).
- `cycleRecMs >= 1`, `1 <= packetSz <= SV_PACKETSZ_MAX`.

## Wire-format

Пакет совместим с парсером [`SVServer`](../SVServer/src/buffer_data.cpp):

```
=begin= | int32 dataSz | module[24] | (name[24] | ValueType | Value[packetSz])... | =end=
```

Fixed-size поля (`module`, `name`) zero-padded до 24 байт.

## Внутренняя структура

```
SVClient.h       — публичный C API
src/client.cpp   — svConnect/svDisconnect, addValue, prepareData, sendCycle
src/dllmain.cpp  — Windows DLL entry (пустой; cleanup через svDisconnect)
```

### Синхронизация

| Mutex / флаг | Область |
|--------------|---------|
| `_sessionMtx` | `svConnect` / `svDisconnect`, `_thr` |
| `_tcpMtx` | `TCPClient::connect` / `disconnect` / `sendData` |
| `_mtx` | `_values`, `_curCycle`, `cng` |
| `_isConnect` (`atomic_bool`) | Snapshot TCP-состояния; записи под `_tcpMtx`, чтения lock-free |

Порядок захвата: `_sessionMtx` → `_mtx`. `_tcpMtx` и `_mtx` **не держать одновременно**.

## Потребители

| Модуль | Использование |
|--------|---------------|
| SVClientExample | Пример отправки синусоиды |
| SVMonitor | Клиентские модули через TCP |
| SVArduino | Порт клиента для Arduino |
