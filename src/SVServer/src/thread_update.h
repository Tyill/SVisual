//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
#pragma once

#include "buffer_data.h"
#include "archive.h"

#include <thread>
#include <atomic>

class ThreadUpdate{

public:

  ThreadUpdate(const SV_Srv::Config&, BufferData&);

  ~ThreadUpdate();

private:

  SV_Srv::Config cng;

  std::atomic_bool m_thrStop = false;

  std::thread m_thr;
  BufferData& m_buffData;
  Archive m_archive;

  void updateCycle();
  void addSignal(const BufferData::InputData& bp);
  void updateSignals(std::map<std::string, SV_Base::SignalData*>&, std::map<std::string, SV_Base::ModuleData*>&,
                     std::map<std::string, bool>& signActive, std::map<std::string, bool>& moduleActive);
  void updateSignalsBuff(SV_Base::SignalData* sign, size_t beginPos, size_t valuePos);
  void moduleConnect(const std::string& module);
  void moduleDisconnect(const std::string& module);
};
