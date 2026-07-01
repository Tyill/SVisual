//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
#pragma once

#include "SVMisc/front.h"
#include "SVServer/sv_server.h"

#include <map>
#include <memory>
#include <thread>
#include <vector>

class ClickHouseDB;

class Archive
{
public:
    
  Archive() = default;
  ~Archive();

  void init(const SV_Srv::Config&);

  void copyToDisk(bool isStop);

  void addSignal(const std::string& sname, const std::string& module, SV_Base::ValueType stype);
  void addValue(const std::string& sign, const SV_Base::RecData& rd);

private:
  void joinSaveThread();
  void copyToDiskImpl(bool isStop, int archiveIndex);
  bool isCopyTimeHour();
  std::string getOutPath(bool isStop);
  bool compressData(size_t insz, const std::vector<char>& inArr, size_t& outsz, std::vector<char>& outArr);

private:

  SV_Srv::Config cng;

  std::string m_copyStartTime = "";
  std::string m_copyDateMem = "";

  int m_crtFileHour = 0;
  int m_archiveIndex = 0;
  size_t m_copySz = 0;
  std::map<std::string, int> m_valPos[2];

  SV_Misc::Front m_front;

  std::map<std::string, std::vector<SV_Base::RecData>> m_archiveData[2];
  std::map<std::string, std::unique_ptr<SV_Base::Value[]>> m_archiveValueStorage[2];

  ClickHouseDB* m_chdb{};
  std::shared_ptr<std::thread> m_saveThread;

};
