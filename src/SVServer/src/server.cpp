//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//

#include "sv_server.h"
#include "buffer_data.h"
#include "thread_update.h"
#include "SVMisc/misc.h"

#include <atomic>
#include <cstring>
#include <memory>
#include <shared_mutex>

using namespace std;

SV_Srv::statusCBack pfStatusCBack = nullptr;
SV_Srv::onUpdateSignalsCBack pfUpdateSignalsCBack = nullptr;
SV_Srv::onAddSignalsCBack pfAddSignalsCBack = nullptr;
SV_Srv::onModuleConnectCBack pfModuleConnectCBack = nullptr;
SV_Srv::onModuleDisconnectCBack pfModuleDisconnectCBack = nullptr;

void statusMessage(const std::string& mess){
  if (pfStatusCBack) pfStatusCBack(mess);
}

namespace SV_Srv {

  Config cng;

  BufferData m_buffData;
  ThreadUpdate* m_pThrUpdSignal = nullptr;

  std::map <std::string, SV_Base::ModuleData*> m_moduleData;
  std::map <std::string, SV_Base::SignalData*> m_signalData;
  std::map<std::string, std::unique_ptr<SV_Base::Value[]>> m_signalBuffStorage;

  std::atomic_bool m_wasStarted{false};
  std::atomic_bool m_isRunning{false};

  std::mutex m_mtxLifecycle;
  std::mutex m_mtxCommon;
  std::shared_mutex m_mtxRW;

  bool startServer(const Config& _cng, statusCBack stsCBack){
    std::lock_guard<std::mutex> lifecycle(m_mtxLifecycle);

    bool expected = false;
    if (!m_wasStarted.compare_exchange_strong(expected, true))
      return false;

    pfStatusCBack = std::move(stsCBack);
    cng = _cng;

    m_buffData.init(cng);

    auto thr = std::make_unique<ThreadUpdate>(cng, m_buffData);

    {
      std::lock_guard<std::mutex> lck(m_mtxCommon);
      m_pThrUpdSignal = thr.release();
    }
    m_isRunning.store(true, std::memory_order_release);
    return true;
  }

  void stopServer(){
    std::lock_guard<std::mutex> lifecycle(m_mtxLifecycle);

    m_isRunning.store(false, std::memory_order_release);
    ThreadUpdate* thr = nullptr;
    {
      std::lock_guard<std::mutex> lck(m_mtxCommon);
      std::swap(thr, m_pThrUpdSignal);
    }
    delete thr;
  }

  void receiveData(std::string& inout, std::string& out){
    (void)out;
    if (!m_isRunning.load(std::memory_order_acquire)) return;

    vector<pair<size_t, size_t>> bePos;
    const std::string_view beginMess = "=begin=";
    const std::string_view endMess = "=end=";
    const size_t mlen = 4, beginLen = beginMess.size(), endLen = endMess.size();
    size_t stPos = inout.find(beginMess), endPos = 0;
    while (stPos != std::string::npos && stPos + beginLen + mlen <= inout.size()){
      int32_t allSz;
      std::memcpy(&allSz, inout.c_str() + stPos + beginLen, sizeof(int32_t));
      const size_t payloadStart = stPos + beginLen + mlen;
      const size_t maxPayload = inout.size() - payloadStart;
      if (allSz > 0 && static_cast<size_t>(allSz) <= maxPayload){
        endPos = payloadStart + allSz;
        if (endPos + endLen <= inout.size() && 
            endMess == std::string_view(inout.data() + endPos, endLen)){
          bePos.push_back({payloadStart, endPos});
          stPos = inout.find(beginMess, endPos + endLen);
          continue;
        }
      }
      stPos = inout.find(beginMess, stPos + beginLen);
    };
    const auto bTm = SV_Misc::currDateTimeSinceEpochMs();
    const auto psz = bePos.size();
    for (size_t i = 0; i < psz; ++i){
      stPos = bePos[i].first;
      endPos = bePos[i].second;
      m_buffData.updateDataSignals(std::string_view(inout.data() + stPos, endPos - stPos),
                                  bTm - (psz - i) * SV_CYCLESAVE_MS);
    }
    if (psz > 0){
      inout = std::string(inout.begin() + endPos + endLen, inout.end());
    }
  }

  void setOnUpdateSignalsCBack(onUpdateSignalsCBack cback){
    pfUpdateSignalsCBack = cback;
  }

  void setOnAddSignalsCBack(onAddSignalsCBack cback){
    pfAddSignalsCBack = cback;
  }

  void setOnModuleConnectCBack(onModuleConnectCBack cback){
    pfModuleConnectCBack = cback;
  }

  void setOnModuleDisconnectCBack(onModuleDisconnectCBack cback){
    pfModuleDisconnectCBack = cback;
  }

  std::map<std::string, SV_Base::ModuleData*> getCopyModuleRef(){
    std::lock_guard<std::mutex> lck(m_mtxCommon);
    map<string, SV_Base::ModuleData*> mref = m_moduleData;
    return mref;
  };

  SV_Base::ModuleData* getModuleData(const std::string& module){
    std::lock_guard<std::mutex> lck(m_mtxCommon);
    return m_moduleData.find(module) != m_moduleData.end() ? m_moduleData[module] : nullptr;
  }
   
  std::vector<std::string> getModuleSignals(const std::string& module){
    std::lock_guard<std::mutex> lck(m_mtxCommon);
    return m_moduleData.find(module) != m_moduleData.end() ? m_moduleData[module]->signls : std::vector<std::string>();
  }

  std::map<std::string, SV_Base::SignalData*> getCopySignalRef(){
    std::lock_guard<std::mutex> lck(m_mtxCommon);
    map<string, SV_Base::SignalData*> sref = m_signalData;
    return sref;
  };

  SV_Base::SignalData* getSignalData(const std::string& sign){
    std::lock_guard<std::mutex> lck(m_mtxCommon);
    return m_signalData.find(sign) != m_signalData.end() ? m_signalData[sign] : nullptr;
  }

  void lockReadSData() {
      m_mtxRW.lock_shared();
  }
  void unlockReadSData() {
      m_mtxRW.unlock_shared();
  }
  void lockWriteSData() {
      m_mtxRW.lock();
  }
  void unlockWriteSData() {
      m_mtxRW.unlock();
  }

  bool addSignal(SV_Base::SignalData* sd){
    if (!sd) return false;
    
    std::lock_guard<std::mutex> lck(m_mtxCommon);
        
    bool ok = false;
    string sign = sd->name + sd->module;
    if (m_signalData.find(sign) == m_signalData.end()) {
      m_signalData[sign] = sd;
      if (!m_moduleData.count(sd->module)){
          m_moduleData[sd->module] = new SV_Base::ModuleData(sd->module);
          m_moduleData[sd->module]->isEnable = true;
      }
      m_moduleData[sd->module]->signls.push_back(sign);
      ok = true;
    }
    return ok;
  }

  bool addModule(SV_Base::ModuleData* md){
    if (!md) return false;

    std::lock_guard<std::mutex> lck(m_mtxCommon);

    bool ok = false;
    if (md && (m_moduleData.find(md->module) == m_moduleData.end())) {
      m_moduleData[md->module] = md;
      ok = true;
    }
    return ok;
  }

  bool signalBufferEna(const std::string& sign){
    SV_Base::SignalData* sd = nullptr;
    {
      std::lock_guard lck(m_mtxCommon);
      auto it = m_signalData.find(sign);
      if (it == m_signalData.end()) return false;
      sd = it->second;
    }

    {
      std::lock_guard lck(m_mtxRW);
      if (sd->isBuffEnable) return true;
    }

    const int buffSz = std::max(10, static_cast<int>((2 * 3600000) / SV_CYCLESAVE_MS));
    std::vector<SV_Base::RecData> preparedBuffData(buffSz);
    auto preparedValueBlock = std::make_unique<SV_Base::Value[]>(SV_PACKETSZ * buffSz);
    for (int i = 0; i < buffSz; ++i){
      preparedBuffData[i].vals = &preparedValueBlock[i * SV_PACKETSZ];
    }

    {
      std::lock_guard lckw(m_mtxRW);
      if (sd->isBuffEnable) return true;
      sd->buffData = std::move(preparedBuffData);
      m_signalBuffStorage[sign] = std::move(preparedValueBlock);
      sd->buffBeginPos = 0;
      sd->buffValuePos = 0;
      sd->isBuffEnable = true;
    }
    return true;
  }
}
