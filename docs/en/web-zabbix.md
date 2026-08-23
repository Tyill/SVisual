[English](web-zabbix.md) | [Русский](../ru/web-zabbix.md) | [Contents](../en/README.md)

# Web, Zabbix, ClickHouse

All three are enabled in **Settings**. They do not replace SVMonitor; they extra sinks / views of the same signals.

## Web view

Enable **Web server**, set bind address and port (default **2145**).

![Web settings](../images/settings-web.png)

![Browser](../images/browser.png)

## ClickHouse

Enable **Save to ClickHouse**. Defaults in current Monitor: database `svdb`, address `localhost:9000`. Requires a build with `USE_ClickHouseDB`.

![ClickHouse settings](../images/settings-clickhouse.png)

Install ClickHouse Server on Linux as in the upstream docs, then point Monitor at it.

## Zabbix

Enable the **passive Zabbix agent**. Default listen **2146**.

Point `zabbix_agentd.conf` (or the server’s agent item) at that address/port.

![Zabbix](../images/zabbix-conf.png)
