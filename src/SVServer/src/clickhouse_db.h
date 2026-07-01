//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
#pragma once

#include "SVServer/sv_server.h"

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <map>
#include <mutex>
#include <vector>

namespace clickhouse{
    class Client;
    class Block;
    class Column;
}
namespace SV_Base{
    struct RecData;
}

class ClickHouseDB{

public:
    ClickHouseDB(const SV_Srv::Config&);
    ~ClickHouseDB();

    bool isConnect()const;

    void addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType);

    void saveSData(bool onClose, const std::map<std::string, int>& valPos, const std::map<std::string, std::vector<SV_Base::RecData>>&);

private:
    void joinWorkers();
    void emplaceWorker(std::function<void()> fn);
    void reapWorkersLocked();
    std::unique_ptr<clickhouse::Client> newClient()const;
    std::unique_ptr<clickhouse::Block> newSignalBlock()const;
    std::unique_ptr<clickhouse::Block> newSDataBlock()const;

    std::shared_ptr<clickhouse::Column> column(const std::unique_ptr<clickhouse::Block>&, const std::string& colName)const;

private:
    SV_Srv::Config cng;

    std::map<std::string, int> m_signals;
    std::unique_ptr<clickhouse::Block> m_signalBlock;

    std::mutex m_mtx;
    std::mutex m_workersMtx;
    std::atomic_bool m_stopping{false};
    std::vector<std::future<void>> m_workers;
};
