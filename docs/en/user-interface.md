[English](user-interface.md) | [Русский](../ru/user-interface.md) | [Contents](../en/README.md)

# User interface

Main window of **SVMonitor**:

![Main window](../images/ui-main.png)

| Area | Role |
| --- | --- |
| **Signal list** (left) | Modules and signals that have connected. Double-click or drag a signal onto the plot. |
| **Status panel** | Connect/disconnect, new signals, trigger hits. |
| **Work area** | Live plots. |
| **Value table** | Values at plot markers. |
| **Toolbar** | Zoom, colors, live/follow mode, extra plot windows. |

## Signal list

![Signal list](../images/signal-list.png)

LED next to a module shows whether data is arriving.

Unused signals/modules: right-click **Delete**. An **active** module must be disconnected first.

![Delete module](../images/module-delete.png)

**Comment** and **Group** are stored for archive browsing in SVViewer.

## Status panel

![Status](../images/status-bar.png)

Typical messages: module list changes, trigger fires, physical connect/disconnect.

## Work area

![Plot](../images/work-area.png)

- Pan with the right mouse button
- Select a region of interest

![Region](../images/select-region.png)

Markers:

![Markers](../images/marker.png)

Several signals in one plot:

![Several signals](../images/many-signals.png)

Extra plot windows and an alternate axis:

![Extra windows](../images/extra-windows.png)

![Alternate axis](../images/alt-axis.png)

## Value table

![Value table](../images/value-table.png)

Shows marker times and `X2 − X1` for the selected interval.
