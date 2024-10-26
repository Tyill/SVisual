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

#include "SVZabbix/zabbix.h"
#include "zbx_server.h"

using namespace std;

ZbxServer zServer;

namespace SV_Zbx {

  lockReadSDataCBack pfLockReadSData = nullptr;
  unlockReadSDataCBack pfUnlockReadSData = nullptr;

  bool startAgent(const QString& addr, int port, const Config& cng) {

    if (zServer.isListening()) return true;

    zServer.setConfig(cng);

    return zServer.listen(QHostAddress(addr), port);
  }

  void stopAgent() {

    if (zServer.isListening())
      zServer.close();
  }

  void setLockReadSData(lockReadSDataCBack f) {
      if (f) {
          pfLockReadSData = f;
      }
  }

  void setUnlockReadSData(unlockReadSDataCBack f) {
      if (f) {
          pfUnlockReadSData = f;
      }
  }

  void setGetSignalData(getSignalDataCBack f) {

    zServer.pfGetSignalData = f;
  }
}

LockerReadSDataZbx::LockerReadSDataZbx() {
    if (SV_Zbx::pfLockReadSData) SV_Zbx::pfLockReadSData();
}
LockerReadSDataZbx::~LockerReadSDataZbx() {
    if (SV_Zbx::pfUnlockReadSData) SV_Zbx::pfUnlockReadSData();
}