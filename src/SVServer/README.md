# SVServer

Библиотека приёма и обработки потока сигналов SVisual: ring-buffer входящих данных, worker-поток обновления сигналов, запись архива на диск и (опционально) в ClickHouse.

Собирается как shared library (`libSVServer.so` / `SVServer.dll`), публичный API — `sv_server.h`.

## Сборка

```bash
cd src/build
cmake .. -DUSE_ClickHouseDB=OFF   # или ON при установленном clickhouse-cpp
cmake --build . --target SVServer
```

| Опция CMake | Описание |
|-------------|----------|
| `USE_ClickHouseDB` | Включить `ClickHouseDB` и запись в ClickHouse |

Зависимости: `SVBase`, `SVMisc`, `zlib` / `z`.

## Lifecycle

Сервер стартует **один раз за жизнь процесса**. Config и status-callback задаются только при старте; hot-reload настроек архива/ClickHouse не поддерживается.

```
Idle ──startServer()──► Running ──stopServer()──► Stopped
  │                         │                         │
  │                    receiveData OK              receiveData no-op
  │                         │                         │
  └── startServer() ──► false (повторный старт) ◄────┘
```

| Метод | Поведение |
|-------|-----------|
| `startServer(config, statusCb)` | Первый вызов — `true`; все последующие (в т.ч. после `stopServer`) — `false` |
| `stopServer()` | Идемпотентен; `receiveData` сразу становится no-op |
| `receiveData(inout, out)` | Принимает TCP/COM-пакеты только пока сервер running |

`startServer` / `stopServer` сериализуются внутри (`m_mtxLifecycle`); безопасно вызывать из разных потоков.

При неуспешном старте после CAS (`m_wasStarted`) повторный `startServer` также вернёт `false` — rollback не выполняется.

### Пример

```cpp
SV_Srv::Config cfg;
cfg.cycleRecMs = 100;
cfg.packetSz = 10;
cfg.outArchiveEna = true;
cfg.outArchivePath = "/data/archive";

if (!SV_Srv::startServer(cfg, [](const std::string& msg) {
        std::cout << msg << '\n';
    })) {
    // уже стартовал ранее
}

// передать пакет из TCP/COM callback:
SV_Srv::receiveData(buffer, dummyOut);

SV_Srv::stopServer();
```

В **SVMonitor** при нескольких COM/TCP-портах: один `startServer` до цикла чтения, `receiveData` — из каждого reader'а.

## Публичный API

| Функция | Назначение |
|---------|------------|
| `setOnUpdateSignalsCBack` | Сигналы обновлены |
| `setOnAddSignalsCBack` | Добавлены новые сигналы |
| `setOnModuleConnectCBack` / `setOnModuleDisconnectCBack` | Модуль подключён / отключён |
| `getCopyModuleRef` / `getCopySignalRef` | Снимок реестра |
| `getSignalData(sign)` | Данные сигнала (`sign` = `name + module`) |
| `lockReadSData` / `unlockReadSData` | Shared-lock на данные сигналов |
| `lockWriteSData` / `unlockWriteSData` | Exclusive-lock |
| `signalBufferEna(sign)` | Включить буфер графика для сигнала |

Удалены (v2): `setStatusCBack()`, `setConfig()` — callback статуса передаётся в `startServer`, конфиг архива/CH применяется только при следующем запуске приложения.

## Внутренняя структура

```
sv_server.h          — публичный API
src/server.cpp       — lifecycle, receiveData, signalBufferEna, реестр модулей/сигналов
src/buffer_data.*    — ring buffer входящих пакетов
src/thread_update.*  — worker: разбор буфера, addSignal, callbacks, archive tick
src/archive.*        — запись бинарного архива на диск (+ ClickHouse при USE_ClickHouseDB)
src/clickhouse_db.*  — асинхронная запись в ClickHouse (только USE_ClickHouseDB)
```

### Синхронизация

| Mutex | Область |
|-------|---------|
| `m_mtxLifecycle` | `startServer` / `stopServer` |
| `m_mtxCommon` | Реестр `m_moduleData` / `m_signalData` |
| `m_mtxRW` (`shared_mutex`) | Данные сигналов, `isBuffEnable`, буферы графика |

`signalBufferEna`: поиск сигнала под `m_mtxCommon`, чтение/запись `isBuffEnable` и публикация буфера — под `m_mtxRW`. Владение памятью буфера — `m_signalBuffStorage` в `server.cpp`.

`ClickHouseDB`: `m_mtx` — `m_signals` / `m_signalBlock`; `m_workersMtx` — пул worker-потоков; `m_stopping` — запрет новых задач в деструкторе. `joinWorkers()` переносит потоки из-под lock и join'ит локально.

## Потребители

| Модуль | Использование |
|--------|---------------|
| SVMonitor | Основной сервер записи |
| SVDocker | Docker-обёртка |
| SVWriterExample | Пример writer |
