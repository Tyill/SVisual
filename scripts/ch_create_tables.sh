#!/bin/bash

clickhouse-client --query="CREATE DATABASE IF NOT EXISTS svdb";

clickhouse-client -d "svdb" --query="CREATE TABLE IF NOT EXISTS tblSignal( \
    id Int32 CODEC(DoubleDelta),                                \
    sname String,                                               \
    module String                                               \
) ENGINE = MergeTree()                                          \
ORDER BY (module)";

clickhouse-client -d "svdb" --query="CREATE TABLE IF NOT EXISTS tblSData( \
    id Int32 CODEC(DoubleDelta),                                \
    ts UInt64 CODEC(DoubleDelta),                               \
    value Float32 CODEC(Gorilla)                                \
) ENGINE = MergeTree()                                          \
ORDER BY (id, ts)";


