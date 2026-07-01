//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
#pragma once

#include "SVServer/sv_server.h"
#include "SVBase/sv_limits.h"

#include <map>
#include <memory>
#include <mutex>

class BufferData
{
public:
   
  BufferData() = default;

  void init(const SV_Srv::Config&);

  struct InputData{
    std::string name;          
    std::string module;        
    SV_Base::ValueType type;   
    SV_Base::RecData data;     
  };

  void updateDataSignals(std::string_view in, uint64_t bTm);

  bool getDataByReadPos(std::vector<InputData>&);

private:

  SV_Srv::Config cng;

  size_t m_buffSz = 0;
  std::vector<InputData> m_buffer;

  size_t m_buffReadPos = 0;
  size_t m_buffWritePos = 0;
  size_t m_buffWritePosForReader = 0;

  std::map<std::string, uint64_t> m_timeOffsetMs;

  std::unique_ptr<SV_Base::Value[]> m_valueStorage;

  std::mutex m_mtxRead, m_mtxWrite;
};
