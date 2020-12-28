//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "stdafx.h"
#include "server.h"
#include "bufferData.h"
#include "thrUpdSignal.h"
#include "SVAuxFunc/auxFunc.h"
#include "Lib/rapidjson/document.h"
#include "Lib/rapidjson/stringbuffer.h"
#include "Lib/rapidjson/writer.h"


using namespace std;

/// старт сервера
bool server::startServer(SV_Srv::config cng_){

    std::lock_guard<std::mutex> lck(mtx_);

  if (isRun_) return true;

  cng = cng_;

  pBuffData_ = new bufferData(bufferData::config(cng_.cycleRecMs, cng_.packetSz));

  pthrUpdSignal_ = new thrUpdSignal(cng_, this, pBuffData_);
  
  isRun_ = true;

  return true;
}

/// стоп сервера
void server::stopServer(){

  if (isRun_) {
    delete pthrUpdSignal_;
  
    isRun_ = false;
  }
}

/// задать конфиг
void server::setConfig(SV_Srv::config cng_){

  cng.outArchiveEna = cng_.outArchiveEna;
  cng.outArchivePath = cng_.outArchivePath;
  cng.outArchiveName = cng_.outArchiveName;
  cng.outArchiveHourCnt = cng_.outArchiveHourCnt;

  if (isRun_)
     pthrUpdSignal_->setArchiveConfig(cng);
}

/// получение данных
void server::receiveData(std::string& inout, std::string& out){
    
    vector<pair<size_t, size_t>> bePos;
    size_t stPos = inout.find("=begin="), endPos = inout.find("=end=");
    while ((stPos != std::string::npos) && (endPos != std::string::npos)){

    int allSz = *(int*)(inout.c_str() + stPos + 7);

        if (allSz == (endPos - stPos - 11))
            bePos.push_back(pair<size_t, size_t>(stPos + 11, endPos));

        stPos = inout.find("=begin=", endPos + 5);
        if (stPos != std::string::npos){
            endPos = inout.find("=end=", stPos + 11);
        }
    };

    uint64_t bTm = SV_Aux::CurrDateTimeSinceEpochMs();
    auto sz = bePos.size();
    for (size_t i = 0; i < sz; ++i){

        stPos = bePos[i].first;
        endPos = bePos[i].second;

        pBuffData_->updDataSignals(string(inout.data() + stPos, inout.data() + endPos),
                   bTm - (sz - i) * SV_CYCLESAVE_MS);
    }

    // запрос json?
  if (sz == 0)
    jsonRequestData(inout, out);
    else
        inout = std::string(inout.data() + endPos + 5);
}

// вернуть все модули
std::map<std::string, SV_Base::ModuleData *> server::getCopyModuleRef(){

    std::lock_guard<std::mutex> lck(mtx_);

  map<string, SV_Base::ModuleData*> mref = moduleData_;

  return mref;
};

// добавить модуль
bool server::addModule(SV_Base::ModuleData* md){

    std::lock_guard<std::mutex> lck(mtx_);

  bool ok = false;
  if (md && (moduleData_.find(md->module) == moduleData_.end())) {
        moduleData_[md->module] = md;
        ok = true;
    }

  return ok;
}

// вернуть данные модуля
SV_Base::ModuleData* server::getModuleData(const std::string& module){

    std::lock_guard<std::mutex> lck(mtx_);

  return moduleData_.find(module) != moduleData_.end() ? moduleData_[module] : nullptr;
}

// вернуть сигналы модуля
std::vector<std::string> server::getModuleSignals(const std::string& module){
    std::lock_guard<std::mutex> lck(mtx_);

    return moduleData_.find(module) != moduleData_.end() ? moduleData_[module]->signls : std::vector<std::string>();
}

// вернуть все сигналы
std::map<std::string, SV_Base::SignalData *> server::getCopySignalRef(){

    std::lock_guard<std::mutex> lck(mtx_);

  map<string, SV_Base::SignalData*> sref = signalData_;

  return sref;
};

// добавить сигнал
bool server::addSignal(SV_Base::SignalData* sd){

    std::lock_guard<std::mutex> lck(mtx_);

    string sign = sd->name + sd->module;
  bool ok = false;
  if (sd && (signalData_.find(sign) == signalData_.end())) {
        signalData_[sign] = sd;
        moduleData_[sd->module]->signls.push_back(sign);
        ok = true;
    }

  return  ok;
}

// вернуть данные сигнала
SV_Base::SignalData* server::getSignalData(const std::string& sign){

    std::lock_guard<std::mutex> lck(mtx_);

  return signalData_.find(sign) != signalData_.end() ? signalData_[sign] : nullptr;
}


// разрешить запись буфера данных
bool server::signalBufferEna(const std::string& sign){

    std::lock_guard<std::mutex> lck(mtx_);

  if (signalData_.find(sign) == signalData_.end()) return false;

  if (!signalData_[sign]->isBuffEnable){

    int buffSz = BUFF_SIGN_HOUR_CNT * 3600000 / SV_CYCLESAVE_MS;

    signalData_[sign]->buffData.resize(buffSz);

    SV_Base::Value* buff = new SV_Base::Value[SV_PACKETSZ * buffSz];
    for (int i = 0; i < buffSz; ++i)
      signalData_[sign]->buffData[i].vals = &buff[i * SV_PACKETSZ];

    signalData_[sign]->isBuffEnable = true;
  }

  return true;
}


//////////////////////////////////////////////////////////

std::string server::jsonGetError(){

  using namespace rapidjson;

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);
  writer.StartObject();
  writer.Key("Command");
  writer.String("Error");

  writer.EndObject();

  return sb.GetString();
}

bool server::jsonCheckRequest(rapidjson::Document& doc){

  if (!doc.IsObject()) return false;

  if (!doc.HasMember("Command") || !doc["Command"].IsString()) return false;

  string cmd = doc["Command"].GetString();

  if (cmd == "getAllSignals"){
    return true;
  }
  else if (cmd == "getSignalData"){
    if (!doc.HasMember("Signal") || !doc["Signal"].IsString()) return false;
    if (!doc.HasMember("Module") || !doc["Module"].IsString()) return false;
  }
  else return false;

  return true;
}

void server::jsonRequestData(std::string &inout, std::string &out){

  using namespace rapidjson;

  if (inout.empty()) return;

  Document document;
  document.Parse(inout.c_str());

  if (!jsonCheckRequest(document)){

    if (document.IsObject()) {
      out = jsonGetError();
      inout.clear();
    }
    return;
  }

  string cmd = document["Command"].GetString();

  if (cmd == "getAllSignals")
    out = jsonGetAllSignals();
  else if (cmd == "getSignalData")
    out = jsonGetSignalData(document["Signal"].GetString(), document["Module"].GetString());

  inout.clear();
}

std::string server::jsonGetAllSignals(){

  using namespace rapidjson;

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);
  writer.StartObject();

  writer.Key("Signals");
  writer.StartArray();

  auto sref = getCopySignalRef();
  for (auto& sign : sref){

    writer.StartObject();

    writer.Key("Name");
    writer.String(sign.second->name.c_str());
    writer.Key("Module");
    writer.String(sign.second->module.c_str());
    writer.Key("Group");
    writer.String(sign.second->group.c_str());
    writer.Key("Comment");
    writer.String(sign.second->comment.c_str());
    writer.Key("Type");
    writer.String(SV_Base::getSVTypeStr(sign.second->type).c_str());

    string state = "isActive";
    if (!sign.second->isActive) state = "noActive";
    writer.Key("State");
    writer.String(state.c_str());

    writer.EndObject();
  }
  writer.EndArray();

  writer.EndObject();

  return sb.GetString();
}

std::string server::jsonGetSignalData(std::string sign, std::string mod){

  using namespace rapidjson;

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);
  writer.StartObject();
  writer.Key("Command");
  writer.String("SignalData");

  auto sref = getCopySignalRef();

  string out;

  sign += mod;
  if (sref.find(sign) != sref.end()){

    auto sd = getSignalData(sign);

    writer.Key("Signal");
    writer.String(sd->name.c_str());
    writer.Key("Module");
    writer.String(sd->module.c_str());
    writer.Key("ValueTime");
    writer.String(to_string(sd->lastData.beginTime).c_str());
    writer.Key("Value");

    switch (sd->type) {
      case SV_Base::ValueType::BOOL:
        writer.String(to_string(sd->lastData.vals[0].BOOL).c_str());
        break;
      case SV_Base::ValueType::INT:
        writer.String(to_string(sd->lastData.vals[0].INT).c_str());
        break;
      case SV_Base::ValueType::FLOAT:
        writer.String(to_string(sd->lastData.vals[0].FLOAT).c_str());
        break;
    }

    writer.EndObject();

    out = sb.GetString();
  }
  else{

    writer.Key("Signal");
    writer.String("");
    writer.Key("Module");
    writer.String("");
    writer.Key("ValueTime");
    writer.String("");
    writer.Key("Value");
    writer.String("");

    writer.EndObject();

    out = sb.GetString();
  }

  return out;
}