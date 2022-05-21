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

#include "zbx_server.h"
#include "SVBase/base.h"
#include "SVBase/limits.h"
#include <QDir>
#include <QUrl>

void ZbxServer::setConfig(const SV_Zbx::Config& cng_) {

  cng = cng_;
}

void ZbxServer::incomingConnection(qintptr handle) {

  zbxClientSocket* socket = new zbxClientSocket(this);
  socket->setSocketDescriptor(handle);

  connect(socket, SIGNAL(readyRead()), socket, SLOT(readData()));
  connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

zbxClientSocket::zbxClientSocket(QObject* parent)
  : QTcpSocket(parent) {

  server_ = (ZbxServer*)parent;
}

void zbxClientSocket::readData() {

  QString sname = this->readAll();

  sname = sname.trimmed();

  QString sval = server_->getLastValueStr(sname);

  this->write(sval.toStdString().c_str());
}

QString ZbxServer::getLastValueStr(const QString& sname) {

  auto sdata = pfGetSignalData(sname);

  if (sdata) {

    auto val = sdata->lastData.vals[SV_PACKETSZ - 1];

    switch (sdata->type) {
    case SV_Base::ValueType::BOOL:  return val.vBool ? "1" : "0";
    case SV_Base::ValueType::INT:   return QString::number(val.vInt);
    case SV_Base::ValueType::FLOAT: return QString::number(val.vFloat);
    default: return "0";
    }
  }
  else
    return "0";
}