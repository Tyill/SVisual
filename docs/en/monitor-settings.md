[English](monitor-settings.md) | [Русский](../ru/monitor-settings.md) | [Contents](../en/README.md)

# SVMonitor settings

Menu **File → Settings** (or the settings item shown below).

![Open settings](../images/settings-menu.png)

![Settings dialog](../images/settings.png)

The dialog in current builds has more blocks than the old screenshot (web, Zabbix, ClickHouse, time offset). Fields below match `src/SVMonitor/forms/settings_dialog.ui` and defaults in `main_win.cpp`.

## Data source

| Mode | What to set |
| --- | --- |
| **Ethernet** | Listen **IP** and **TCP port** of *this* PC. Default `127.0.0.1:2144`. Clients connect **to** Monitor. |
| **COM/UART** | Port name and baud. Add/remove ports with the buttons. Speeds in the combo: 1200 … 115200. Default baud **9600**. |

COM framing is fixed in software (8 data bits, no parity — as in the booklet).

Changes under **measurement** (period / packet / time offset) need a restart of the application.

## Web / Zabbix

| | Default |
| --- | --- |
| Web listen address / port | `127.0.0.1` / **2145** |
| Zabbix agent (passive) address / port | `127.0.0.1` / **2146** |

Enable with the **Active** checkboxes.

## Saving data

| | Meaning |
| --- | --- |
| Save to file | Folder for archive files. Enable with **Active**. |
| File length | `outArchiveHourCnt`, **1–12 hours**, default **2** (not megabytes). |
| ClickHouse | Database name (default `svdb`) and address (default `localhost:9000`). |

## Measurement

| Parameter | Default | Role |
| --- | --- | --- |
| Record period `cycleRecMs` | **100 ms** (10 Hz) | How often a sample is taken |
| Packet size `packetSz` | **10** | Samples per TCP/COM packet |
| Time offset | 0 s | Shift timestamps forward; use when sampling **> 1 kHz** |

Packet send interval is `cycleRecMs × packetSz` (1 s with the defaults).
