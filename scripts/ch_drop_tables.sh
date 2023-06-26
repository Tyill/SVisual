#!/bin/bash

clickhouse-client -d "svdb" --query="DROP TABLE IF EXISTS tblSignal";
clickhouse-client -d "svdb" --query="DROP TABLE IF EXISTS tblSData";
