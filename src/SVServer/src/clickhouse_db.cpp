#ifdef USE_ClickHouseDB

#include "clickhouse_db.h"
#include "SVMisc/misc.h"
#include "SVServer/sv_server.h"
#include "SVBase/sv_limits.h"

#include <clickhouse/client.h>

#include <algorithm>
#include <chrono>

namespace ch = clickhouse;

void statusMessage(const std::string&);

ClickHouseDB::ClickHouseDB(const SV_Srv::Config& _cng):
    cng(_cng)
{
    emplaceWorker([this]() {
        try {
            if (auto clt = newClient(); clt) {
                clt->Select("SELECT id, sname, module FROM tblSignal;", [this](const ch::Block& block) {
                    std::lock_guard lk(m_mtx);
                    for (size_t i = 0; i < block.GetRowCount(); ++i) {
                        int sId = block[0]->As<ch::ColumnInt32>()->At(i);
                        std::string sname = std::string(block[1]->As<ch::ColumnString>()->At(i));
                        std::string module = std::string(block[2]->As<ch::ColumnString>()->At(i));

                        m_signals.insert({ sname + module, sId });
                    }
                    });
            }
        }
        catch (const std::exception& e) {
            statusMessage("ClickHouseDB read signals error: " + std::string(e.what()));
        }
        });
}

ClickHouseDB::~ClickHouseDB() {
    m_stopping.store(true, std::memory_order_release);
    joinWorkers();
}

void ClickHouseDB::reapWorkersLocked() {
    m_workers.erase(
        std::remove_if(m_workers.begin(), m_workers.end(),
            [](std::future<void>& f) {
                return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
            }),
        m_workers.end());
}

void ClickHouseDB::emplaceWorker(std::function<void()> fn) {
    if (m_stopping.load(std::memory_order_acquire)) return;
    std::lock_guard lk(m_workersMtx);
    if (m_stopping.load(std::memory_order_acquire)) return;
    reapWorkersLocked();
    m_workers.push_back(std::async(std::launch::async, std::move(fn)));
}

void ClickHouseDB::joinWorkers() {
    std::vector<std::future<void>> workers;
    {
        std::lock_guard lk(m_workersMtx);
        workers = std::move(m_workers);
    }
    for (auto& f : workers) {
        if (f.valid()) {
            f.wait();
        }
    }
}

bool ClickHouseDB::isConnect()const
{
    try{
        if (auto clt = newClient(); clt){
            clt->Ping();
            return true;
        }
    }catch(const std::exception& e){
        statusMessage("ClickHouseDB connect error: " + std::string(e.what()));
    }
    return false;
}

void ClickHouseDB::addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype)
{
    const std::string key = sname + module;
    bool needWorker = false;

    {
        std::lock_guard lk(m_mtx);
        if (m_signals.count(key)) return;
        m_signals.insert({key, static_cast<int>(m_signals.size())});

        if (!m_signalBlock){
            m_signalBlock = newSignalBlock();
            needWorker = true;
        }
        auto cId = column(m_signalBlock, "id")->AsStrict<ch::ColumnInt32>();
        auto cSName = column(m_signalBlock, "sname")->AsStrict<ch::ColumnString>();
        auto cModule = column(m_signalBlock, "module")->AsStrict<ch::ColumnString>();
        auto cType = column(m_signalBlock, "stype")->AsStrict<ch::ColumnInt32>();

        cId->Append(m_signals[key]);
        cSName->Append(sname);
        cModule->Append(module);
        cType->Append(int(stype));
    }

    if (needWorker){
        emplaceWorker([this](){
            SV_Misc::sleepMs(1000);
            try{
                std::lock_guard lk(m_mtx);
                if (auto clt = newClient(); clt && m_signalBlock){
                    m_signalBlock->RefreshRowCount();
                    clt->Insert("tblSignal", *m_signalBlock);
                }
                m_signalBlock.reset();
            }catch(const std::exception& e){
                statusMessage("ClickHouseDB::addSignal save error: " + std::string(e.what()));
            }
        });
    }
}

void ClickHouseDB::saveSData(bool onClose, const std::map<std::string, int>& valPos, const std::map<std::string, std::vector<SV_Base::RecData>>& sdata)
{    
    auto dataBlock = newSDataBlock();

    auto cId = column(dataBlock, "id")->AsStrict<ch::ColumnInt32>();
    auto cTs = column(dataBlock, "ts")->AsStrict<ch::ColumnUInt64>();
    auto cValue = column(dataBlock, "value")->AsStrict<ch::ColumnFloat32>();

    for (const auto& sd : sdata){

        int sid = 0;
        int vcnt = 0;
        {
            std::lock_guard lk(m_mtx);
            auto it = m_signals.find(sd.first);
            if (it == m_signals.end()) continue;
            auto vpIt = valPos.find(sd.first);
            if (vpIt == valPos.end() || vpIt->second == 0) continue;
            sid = it->second;
            vcnt = vpIt->second;
        }

        auto sign = SV_Srv::getSignalData(sd.first);
        if (!sign) continue;

        if (sign->type == SV_Base::ValueType::FLOAT){
            for(int i = 0; i < vcnt; ++i){
                const auto& rd = sd.second[i];
                for (int j = 0; j < SV_PACKETSZ; ++j){
                    cId->Append(sid);
                    cTs->Append(rd.beginTime);
                    cValue->Append(rd.vals[j].vFloat);
                }
            }
        }else{
            for(int i = 0; i < vcnt; ++i){
                const auto& rd = sd.second[i];
                for (int j = 0; j < SV_PACKETSZ; ++j){
                    cId->Append(sid);
                    cTs->Append(rd.beginTime);
                    cValue->Append(rd.vals[j].vInt);
                }
            }
        }        
    }
    if (cId->Size() > 0){
        auto sendToDb =[this](const auto& dblock){
            try{
                if (auto clt = newClient(); clt){
                    dblock->RefreshRowCount();
                    clt->Insert("tblSData", *dblock);
                }
            }catch(const std::exception& e){
                statusMessage("ClickHouseDB::addSData save error: " +  std::string(e.what()));
            }
        };
        if (!onClose){
            emplaceWorker([this, sendToDb, dblock = std::move(dataBlock)](){
               sendToDb(dblock);
            });
        }else{
            sendToDb(dataBlock);
        }
    }
}

clickhouse::ColumnRef ClickHouseDB::column(const std::unique_ptr<clickhouse::Block>& block, const std::string& colName)const
{
    for (auto it = block->begin(); it != block->end(); ++it){
        if (it.Name() == colName){
            return it.Column();
        }
    }
    return clickhouse::ColumnRef();
}

std::unique_ptr<clickhouse::Client> ClickHouseDB::newClient()const
{
    ch::ClientOptions opts;{
        opts.SetDefaultDatabase(cng.outDataBaseName);
        const auto sp = cng.outDataBaseAddr.find(':');
        opts.SetHost(sp == std::string::npos ? cng.outDataBaseAddr
                                             : cng.outDataBaseAddr.substr(0, sp));
        if (sp != std::string::npos && sp + 1 < cng.outDataBaseAddr.size()){
            opts.SetPort(std::stoi(cng.outDataBaseAddr.substr(sp + 1)));
        }
    }
    return std::make_unique<ch::Client>(opts);
}

std::unique_ptr<clickhouse::Block> ClickHouseDB::newSignalBlock()const
{
    auto chBlock = std::make_unique<ch::Block>();
    chBlock->AppendColumn("id", std::make_shared<ch::ColumnInt32>());
    chBlock->AppendColumn("sname", std::make_shared<ch::ColumnString>());
    chBlock->AppendColumn("module", std::make_shared<ch::ColumnString>());
    chBlock->AppendColumn("stype", std::make_shared<ch::ColumnInt32>());

    return chBlock;
}

std::unique_ptr<clickhouse::Block> ClickHouseDB::newSDataBlock()const
{
    auto chBlock = std::make_unique<ch::Block>();
    chBlock->AppendColumn("id", std::make_shared<ch::ColumnInt32>());
    chBlock->AppendColumn("ts", std::make_shared<ch::ColumnUInt64>());
    chBlock->AppendColumn("value", std::make_shared<ch::ColumnFloat32>());

    return chBlock;
}
#endif
