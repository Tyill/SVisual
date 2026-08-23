[English](archives.md) | [Русский](../ru/archives.md) | [Contents](../en/README.md)

# Archives

When **Save to file** is on, SVMonitor writes signal archives to the folder from settings. Each file covers `outArchiveHourCnt` hours (default **2**, range 1–12). Internally a ~10 minute buffer is flushed to disk (`src/SVServer/src/archive.cpp`).

Optional second sink: **ClickHouse** (see [Web, Zabbix, ClickHouse](web-zabbix.md)).

## SVViewer

Open archive files with **SVViewer** (booklet name SVViewer / archive viewer).

![Archive viewer](../images/archive-viewer.png)

Statistics / histogram:

![Histogram](../images/histogram.png)

![Stats](../images/stats.png)

Drag a signal into the stats window to build a histogram. Comments and groups from Monitor are available when browsing.
