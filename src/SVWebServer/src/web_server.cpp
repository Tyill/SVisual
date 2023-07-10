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

#include "SVWebServer/web_server.h"
#include "web_server_impl.h"

WebServer WServer;

namespace SV_Web {

  lockReadSDataCBack pfLockReadSData = nullptr;
  unlockReadSDataCBack pfUnlockReadSData = nullptr;

  bool startServer(const QString& addr, int port, const Config& cng) {

    if (WServer.isListening()) return true;

    WServer.setConfig(cng);

    return WServer.listen(QHostAddress(addr), port);
  }

  void stopServer() {

    if (WServer.isListening())
      WServer.close();
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

  void setGetCopySignalRef(getCopySignalRefCBack f) {

    WServer.pfGetCopySignalRef = f;
  }

  void setGetCopyModuleRef(getCopyModuleRefCBack f) {

    WServer.pfGetCopyModuleRef = f;
  }

  void setGetSignalData(getSignalDataCBack f) {

    WServer.pfGetSignalData = f;
  }
}

LockerReadSDataWeb::LockerReadSDataWeb() {
    if (SV_Web::pfLockReadSData) SV_Web::pfLockReadSData();
}
LockerReadSDataWeb::~LockerReadSDataWeb() {
    if (SV_Web::pfUnlockReadSData) SV_Web::pfUnlockReadSData();
}