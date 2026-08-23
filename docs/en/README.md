[English](README.md) | [Русский](../ru/README.md)

# SVisual manual

SVisual records signals from microcontroller devices, plots them in real time, and can archive, export, and alert on them.

Applications:

| App | Role |
| --- | --- |
| **SVMonitor** | Live view, settings, triggers, archive recording, web / Zabbix / ClickHouse |
| **SVViewer** | Open recorded archives, statistics, export |
| **SVClient** | C++ library that *sends* values to the server |
| **SVArduino** | Same protocol for Arduino (COM, Ethernet, Wi-Fi) |

Limits from the current code (`src/SVBase/sv_limits.h`, Arduino `SVClient.h`):

| | Server (all modules) | Arduino client |
| --- | --- | --- |
| Max signals | 2048 | 128 |
| Name length | 24 characters | 24 |
| Default rate | 10 Hz (`cycleRecMs = 100`, `packetSz = 10`) | 100 ms × 10 samples |
| Max packet | 1 000 000 samples (`SV_PACKETSZ_MAX`) — up to ~1 MHz with time offset | 10 samples |

The Word/PDF booklet still says “up to 100 Hz”; the **code** is the source of truth.

## Contents

1. [Introduction](introduction.md)
2. [Build](compile.md)
3. [SVMonitor settings](monitor-settings.md)
4. [User interface](user-interface.md)
5. [Triggers](triggers.md)
6. [Event report](event-report.md)
7. [Archives](archives.md)
8. [Export](export.md)
9. [Scripts](scripts.md)
10. [Web, Zabbix, ClickHouse](web-zabbix.md)
11. [C++ client](client-pc.md)
12. [Arduino client](client-arduino.md)
13. [Wire protocol](protocol.md)

Original booklets: [pdf_en/SVManualEN.pdf](../pdf_en/SVManualEN.pdf), [pdf_ru/SVManualRU.pdf](../pdf_ru/SVManualRU.pdf) (Word sources in the same folders).
