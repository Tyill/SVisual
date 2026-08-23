[English](../en/export.md) | [Русский](export.md) | [Содержание](../ru/README.md)

# Экспорт

Диалог экспорта (Monitor или Viewer):

![Экспорт](../images/export.png)

Форматы (`export_dialog_impl.cpp`):

| Формат | Примечание |
| --- | --- |
| **json** | Всегда |
| **csv** | Всегда |
| **xlsx** | Если сборка с `USE_QtXlsxWriter` |

Выберите сигналы и интервал времени, затем сохраните файл.
