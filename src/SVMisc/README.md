# SVMisc

Статическая библиотека общих утилит SVisual: TCP client/server (libuv), асинхронный logger, функции времени/файловой системы, PLC-style таймеры и фронты.

Собирается как `libSVMisc.a` / static lib на Windows.

## Сборка

```bash
cd src/build
cmake ..
cmake --build . --target SVMisc
```

| Платформа | Зависимости |
|-----------|-------------|
| Linux | `libuv`, `pthread` |
| Windows | `libuv`, `ws2_32`, `Iphlpapi`, `psapi`, `Userenv` |

## Состав модуля

| Компонент | Header | Назначение |
|-----------|--------|------------|
| `TCPServer` | `SVMisc/tcp_server.h` | TCP-сервер на libuv (worker thread) |
| `TCPClient` | `SVMisc/tcp_client.h` | Блокирующий TCP-клиент (sockets) |
| `Logger` | `SVMisc/logger.h` | Асинхронная запись логов в файл |
| helpers | `SVMisc/misc.h` | Дата/время, `createSubDirectory`, `sleepMs` |
| `TimerDelay` | `SVMisc/timer_delay.h` | ON/OFF delay, импульсы сек/мин/час |
| `Front` | `SVMisc/front.h` | Положительный/отрицательный фронт bool |

Использование в проекте:

- `TCPServer` — `SVDocker`, `SVWriterExample`, `SVMonitor` → callback `SV_Srv::receiveData`
- `TCPClient` — `SVClient` (отправка пакетов, `onlySend=true`)
- `Logger` — `SVMonitor`
- `misc` — `SVServer` (archive, timestamps), Logger, UI

## TCPServer

### API

```cpp
const auto err = SV_Misc::TCPServer::start("0.0.0.0", 2144,
                                           SV_Srv::receiveData,
                                           [](const std::string& err) { /* ... */ });
if (err.empty()) {
    // listen готов (async mode, timeout 5 s)
}
```

| Метод | Поведение |
|-------|-----------|
| `start(...)` | Пустая строка — успех; иначе текст ошибки (init/listen/timeout/already running) |
| `stop()` | Идempotent shutdown; безопасен из `dataCBack` (см. ниже) |

Повторный `start()` при already running — `"already running"` без блокировки.

```cpp
SV_Misc::TCPServer::stop();
```

### Lifecycle (async mode)

```
Idle ──start()──► Running ──stop()──► Draining ──► Idle
  │                  │                              │
  └── start() ──► "already running"                 └── start() снова OK
```

- Собственный `uv_loop_t` (не `uv_default_loop()`).
- `stop()`: async → закрытие server/clients/async → `uv_run` возвращается → `uv_loop_close` → `u_loop=nullptr`.
- При startup abort/timeout worker проверяет `stopping` и выходит без зависания `join()`.
- `start()` перед spawn: `error.clear()`, join только завершённого failed worker (`!running`).

### stop() из dataCBack

`dataCBack` вызывается в libuv worker thread. `stop()` из callback:

1. Выставляет `stopping`, шлёт `uv_async_send` (если loop уже init).
2. **Не делает self-join** — вместо этого `detach()` worker thread.
3. Worker завершает `uv_run` и выходит; повторный `start()` join'ит detached thread не требует (не joinable).

Рекомендация: callbacks и error handler задавать **до** `start()`.

### Sync mode (`intoOtherThread=false`)

`startImpl` блокирует caller thread в `uv_run`. Возвращает `error.empty()` после штатной остановки (не проверяет `running` — он уже сброшен в `drainAndCloseLoop`).

В текущих call sites используется только async mode (default).

## TCPClient

Один глобальный socket на процесс (namespace `SV_Misc::TCPClient`).

```cpp
if (SV_Misc::TCPClient::connect("127.0.0.1", 2144)) {
    std::string out;
    SV_Misc::TCPClient::sendData(packet, out, false, true);  // onlySend
}
SV_Misc::TCPClient::disconnect();
```

| Особенность | Детали |
|-------------|--------|
| Win32 | `WSAStartup` один раз на сессию, `freeaddrinfo` на всех путях |
| Linux | cleanup fd на fail-path connect; `errno` в recv loop |
| `onlySend=true` | Ответ не читается (режим SVClient) |

Не thread-safe: один поток отправки на процесс.

## Logger

Header-only класс, writer thread + ring buffer (100 сообщений).

```cpp
SV_Misc::Logger lg("/var/log/sv/monitor.log");
lg.writeLine("message");
lg.setPathFile("/other/path.log");
```

| Поведение | Детали |
|-----------|--------|
| Overflow | Новое сообщение отбрасывается |
| Shutdown | Drain очереди в деструкторе |
| Path | `createSubDirectory` для parent path файла |

## misc helpers

```cpp
SV_Misc::currDateTime();              // UI: %d-%m-%y %H:%M:%S
SV_Misc::currDateTimeMs();            // лог: %Y-%m-%d %H:%M:%S:ms
SV_Misc::currDateTimeSinceEpochMs();
SV_Misc::createSubDirectory("path/to/file.log");  // создаёт parent dirs
SV_Misc::sleepMs(100);
```

- Функции времени — thread-safe (`localtime_r` / `localtime_s`).
- `createSubDirectory("")` и `"file.log"` → `true`; при `EEXIST` проверяется, что путь — directory (не regular file).

## TimerDelay / Front

Single-threaded PLC-style утилиты для циклов опроса (archive, trigger/script dialogs). Вызывать из одного потока после `update()` раз в цикл.

## Ограничения

- `TCPServer` — singleton (`static srv`); один экземпляр на процесс.
- `TCPClient` — не для параллельных connect/send из разных потоков.
- IPv4 only (`uv_ip4_addr` / `AF_INET`).
- Ответ сервера клиенту через `outMess` в `dataCBack` поддерживается на уровне libuv, но `SV_Srv::receiveData` ответ не использует.
