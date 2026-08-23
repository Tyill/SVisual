[English](compile.md) | [Русский](../ru/compile.md) | [Contents](../en/README.md)

# Build

From the project [README](../../README.md).

## Linux packages

- Qt
- SQLite: `sudo apt install libsqlite3-dev`
- libuv: `sudo apt install libuv1-dev`
- [clickhouse-cpp](../../depends/clickhouse-cpp-2.2.1.zip) if `USE_ClickHouseDB` is on

Auxiliary libs under [depends/linux_deps/](../../depends/linux_deps/): lua53, qtXlsxWriter.

CMake options in `src/CMakeLists.txt`:

- `USE_QtXlsxWriter` — xlsx export (off by default)
- `USE_ClickHouseDB` — ClickHouse writer (off by default)

## CMake

```bash
cd src
rm -rf build
mkdir build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel
```

Or open [`src/CMakeLists.txt`](../../src/CMakeLists.txt) in Qt Creator.

## Docker

Image: https://hub.docker.com/r/tyiler/svdocker
