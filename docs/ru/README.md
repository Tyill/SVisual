[English](../en/README.md) | [Русский](README.md)

# Руководство SVisual

SVisual принимает сигналы с МК, рисует их в реальном времени, пишет архив и умеет оповещать о событиях.

Программы:

| Приложение | Назначение |
| --- | --- |
| **SVMonitor** | Живой просмотр, настройки, триггеры, запись архива, web / Zabbix / ClickHouse |
| **SVViewer** | Просмотр архивов, статистика, экспорт |
| **SVClient** | Библиотека C++, *отправка* значений на сервер |
| **SVArduino** | Тот же протокол для Arduino (COM, Ethernet, Wi-Fi) |

Лимиты из кода (`src/SVBase/sv_limits.h`, Arduino `SVClient.h`):

| | Сервер (все модули) | Клиент Arduino |
| --- | --- | --- |
| Макс. сигналов | 2048 | 128 |
| Длина имени | 24 символа | 24 |
| Частота по умолчанию | 10 Гц (`cycleRecMs = 100`, `packetSz = 10`) | 100 мс × 10 отсчётов |
| Пакет | до 1 000 000 отсчётов (`SV_PACKETSZ_MAX`, порядка 1 МГц со сдвигом времени) | 10 отсчётов |

В старом Word/PDF ещё написано «до 100 Гц»; ориентир — **код**.

## Содержание

1. [Назначение](introduction.md)
2. [Сборка](compile.md)
3. [Настройки SVMonitor](monitor-settings.md)
4. [Интерфейс](user-interface.md)
5. [Триггеры](triggers.md)
6. [Отчёт событий](event-report.md)
7. [Архивы](archives.md)
8. [Экспорт](export.md)
9. [Скрипты](scripts.md)
10. [Web, Zabbix, ClickHouse](web-zabbix.md)
11. [Клиент C++](client-pc.md)
12. [Клиент Arduino](client-arduino.md)
13. [Протокол](protocol.md)

Исходные буклеты: [pdf_en/SVManualEN.pdf](../pdf_en/SVManualEN.pdf), [pdf_ru/SVManualRU.pdf](../pdf_ru/SVManualRU.pdf) (Word лежит в тех же папках).
