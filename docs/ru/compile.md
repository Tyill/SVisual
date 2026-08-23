[English](../en/compile.md) | [Русский](compile.md) | [Содержание](../ru/README.md)

# Сборка

По [README](../../README.md) проекта.

## Пакеты Linux

- Qt
- SQLite: `sudo apt install libsqlite3-dev`
- libuv: `sudo apt install libuv1-dev`
- [clickhouse-cpp](../../depends/clickhouse-cpp-2.2.1.zip), если включён `USE_ClickHouseDB`

Дополнительно в [depends/linux_deps/](../../depends/linux_deps/): lua53, qtXlsxWriter.

Опции CMake в `src/CMakeLists.txt`:

- `USE_QtXlsxWriter` — экспорт xlsx (по умолчанию выкл.)
- `USE_ClickHouseDB` — запись в ClickHouse (по умолчанию выкл.)

## CMake

```bash
cd src
rm -rf build
mkdir build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel
```

Либо открыть [`src/CMakeLists.txt`](../../src/CMakeLists.txt) в Qt Creator.

## Docker

Образ: https://hub.docker.com/r/tyiler/svdocker
