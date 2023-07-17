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
#pragma once

#include "buffer_data.h"
#include "archive.h"

#include <thread>
#include <atomic>

class ThreadUpdate{

public:

  ThreadUpdate(const SV_Srv::Config&, BufferData&);

  ~ThreadUpdate();

  void setArchiveConfig(const SV_Srv::Config&);

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
