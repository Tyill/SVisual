#include "clickhouse_db.h"
#include "clickhouse/client.h"
#include "SVMisc/misc.h"
#include "SVServer/server.h"

#include <thread>

namespace ch = clickhouse;

extern SV_Srv::statusCBack pfStatusCBack;

ClickHouseDB::ClickHouseDB(const std::string& name, const std::string& addr):
    m_chName(name),
    m_chAddr(addr)
{
    try{
        if (auto clt = newClient(); clt){
           clt->Select("SELECT id, sname, module FROM tblSignal;", [this](const ch::Block& block){
               for (size_t i = 0; i < block.GetRowCount(); ++i){
                   int sId = block[0]->As<ch::ColumnInt32>()->At(i);
                   std::string sname = std::string(block[1]->As<ch::ColumnString>()->At(i));
                   std::string module = std::string(block[2]->As<ch::ColumnString>()->At(i));

                   m_signals.insert({sname + module, sId});
               }
           });
        }
    }catch(std::exception& e){
        if (pfStatusCBack){
            pfStatusCBack("ClickHouseDB read signals from db error: " + std::string(e.what()));
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
    }catch(...){
    }
    return false;
}

void ClickHouseDB::addSignal(const std::string& sname, const std::string& module)
{
    if (!m_signals.count(sname + module)){
        m_signals.insert({sname + module, m_signals.size()});

        std::lock_guard lk(m_mtx);

        bool isNew = false;
        if (!m_signalBlock){
            m_signalBlock = newSignalBlock();
            isNew = true;
        }
        auto cId = column(m_signalBlock, "id")->As<ch::ColumnInt32>();
        auto cSName = column(m_signalBlock, "sname")->As<ch::ColumnString>();
        auto cModule = column(m_signalBlock, "module")->As<ch::ColumnString>();

        cId->Append(m_signals[sname + module]);
        cSName->Append(sname);
        cModule->Append(module);

        if (isNew){
            std::thread([this](){
                SV_Misc::sleepMs(1000);
                try{
                    std::lock_guard lk(m_mtx);
                    if (auto clt = newClient(); clt){
                        m_signalBlock->RefreshRowCount();
                        clt->Insert("tblSignal", *m_signalBlock);
                    }
                    m_signalBlock.reset();
                }catch(std::exception& e){
                    if (pfStatusCBack){
                        pfStatusCBack("ClickHouseDB::addSignal save into db error: " + std::string(e.what()));
                    }
                }
            }).detach();
        }
    }
}

void ClickHouseDB::addSData(const std::string& sname, const std::string& module, const std::vector<SData>& sdata)
{
    if (m_signals.count(sname + module)){

        std::lock_guard lk(m_mtx);

        bool isNew = false;
        if (!m_sdataBlock){
            m_sdataBlock = newSDataBlock();
            isNew = true;
        }
        auto cId = column(m_sdataBlock, "id")->As<ch::ColumnInt32>();
        auto cTs = column(m_sdataBlock, "ts")->As<ch::ColumnUInt64>();
        auto cValue = column(m_sdataBlock, "value")->As<ch::ColumnFloat32>();

        int sid = m_signals[sname + module];
        for(const auto& sd : sdata){
            cId->Append(sid);
            cTs->Append(sd.time);
            cValue->Append(sd.value);
        }

        if (isNew){
            std::thread([this](){
                SV_Misc::sleepMs(1000);
                try{
                    std::lock_guard lk(m_mtx);
                    if (auto clt = newClient(); clt){
                        m_sdataBlock->RefreshRowCount();
                        clt->Insert("tblSData", *m_sdataBlock);
                    }
                    m_sdataBlock.reset();
                }catch(std::exception& e){
                    if (pfStatusCBack){
                        pfStatusCBack("ClickHouseDB::addSData save into db error: " +  std::string(e.what()));
                    }
                }
            }).detach();
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
        opts.SetDefaultDatabase(m_chName);
        auto sp = m_chAddr.find(':');
        opts.SetHost(sp > 0 ? m_chAddr.substr(0, sp) : m_chAddr);
        if (sp > 0){
            opts.SetPort(stoi(m_chAddr.substr(sp + 1)));
        }
    }
    return std::make_unique<ch::Client>(opts);
}

std::unique_ptr<clickhouse::Block> ClickHouseDB::newSignalBlock()const
{
    auto chBlock = ch::Block();
    chBlock.AppendColumn("id", std::make_shared<ch::ColumnInt32>());
    chBlock.AppendColumn("sname", std::make_shared<ch::ColumnString>());
    chBlock.AppendColumn("module", std::make_shared<ch::ColumnString>());

    return std::make_unique<ch::Block>(chBlock);
}

std::unique_ptr<clickhouse::Block> ClickHouseDB::newSDataBlock()const
{
    auto chBlock = ch::Block();
    chBlock.AppendColumn("id", std::make_shared<ch::ColumnInt32>());
    chBlock.AppendColumn("ts", std::make_shared<ch::ColumnUInt64>());
    chBlock.AppendColumn("value", std::make_shared<ch::ColumnFloat32>());

    return std::make_unique<ch::Block>(chBlock);
}
