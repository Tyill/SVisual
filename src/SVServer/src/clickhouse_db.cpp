#include "clickhouse_db.h"
#include "clickhouse/client.h"
#include "SVMisc/misc.h"
#include "SVServer/server.h"
#include "SVBase/limits.h"

#include <thread>

namespace ch = clickhouse;

extern SV_Srv::statusCBack pfStatusCBack;

ClickHouseDB::ClickHouseDB(const SV_Srv::Config& _cng):
    cng(_cng)
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
    }catch(std::exception& e){
        if (pfStatusCBack){
            pfStatusCBack("ClickHouseDB isConnect error: " + std::string(e.what()));
        }
    }
    return false;
}

void ClickHouseDB::addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype)
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
        auto cType = column(m_signalBlock, "stype")->As<ch::ColumnInt32>();

        cId->Append(m_signals[sname + module]);
        cSName->Append(sname);
        cModule->Append(module);
        cType->Append(int(stype));

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

void ClickHouseDB::addSData(const std::map<std::string, uint32_t>& valPos, const std::map<std::string, std::vector<SV_Base::RecData>>& sdata)
{
    std::lock_guard lk(m_mtx);

    auto dataBlock = newSDataBlock();

    auto cId = column(dataBlock, "id")->As<ch::ColumnInt32>();
    auto cTs = column(dataBlock, "ts")->As<ch::ColumnUInt64>();
    auto cValue = column(dataBlock, "value")->As<ch::ColumnFloat32>();

    for (const auto& sd : sdata){

        if (!m_signals.count(sd.first) || valPos.at(sd.first) == 0) continue;

        auto sign = SV_Srv::getSignalData(sd.first);
        
        int sid = m_signals[sd.first];
        int vcnt = valPos.at(sd.first);
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
        std::thread([this, dblock = std::move(dataBlock)](){
            try{
                if (auto clt = newClient(); clt){
                    dblock->RefreshRowCount();
                    clt->Insert("tblSData", *dblock);
                }
            }catch(std::exception& e){
                if (pfStatusCBack){
                    pfStatusCBack("ClickHouseDB::addSData save into db error: " +  std::string(e.what()));
                }
            }
        }).detach();
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
        auto sp = cng.outDataBaseAddr.find(':');
        opts.SetHost(sp > 0 ? cng.outDataBaseAddr.substr(0, sp) : cng.outDataBaseAddr);
        if (sp > 0){
            opts.SetPort(stoi(cng.outDataBaseAddr.substr(sp + 1)));
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
    chBlock.AppendColumn("stype", std::make_shared<ch::ColumnInt32>());

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
