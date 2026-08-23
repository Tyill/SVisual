[English](introduction.md) | [Русский](../ru/introduction.md) | [Contents](../en/README.md)

# Introduction

SVisual is meant for watching MCU firmware: live plots, disk archives, and user alerts when a condition fires.

**SVMonitor** can:

- Connect to devices over COM/UART (USB on Arduino) or TCP (Ethernet / Wi-Fi)
- Sample signals in real time (default **10 Hz**; configurable down to 1 ms period, packet size up to 1e6)
- Record up to **2048** signals across all modules
- Plot selected signals and show marker values in a table
- Write an archive to disk and optionally to **ClickHouse**
- Open archives later in **SVViewer**
- Run a user process when a **trigger** fires
- Register signals from the client only — no extra setup on the PC besides connection settings
- Export to **json / csv / xlsx**
- Serve a **web** view and a passive **Zabbix** agent

Clients **push** values. There is no subscribe/get API on the server.

## System requirements (booklet)

- OS: Windows 64-bit (7, 8, 10) or Linux
- CPU: Pentium 4 or newer (booklet); a current 64-bit CPU is assumed
- RAM: 512 MB minimum (booklet)

Linux build extras are listed in [compile.md](compile.md).
