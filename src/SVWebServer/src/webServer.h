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


/////////////////////
#include "stdafx.h"
#include "http_parser.h"
#include <QTcpServer>
#include <QTcpSocket>

#include "SVWebServer/SVWebServer.h"

class webServer : public QTcpServer{
    
    Q_OBJECT

public:

    webServer(QObject *parent = nullptr) : QTcpServer(parent){}

    ~webServer() = default;
        
    SV_Web::pf_getCopySignalRef pfGetCopySignalRef = nullptr;

    SV_Web::pf_getCopyModuleRef pfGetCopyModuleRef = nullptr;

    SV_Web::pf_getSignalData pfGetSignalData = nullptr;

    SV_Web::pf_loadSignalData pfLoadSignalData = nullptr;
    
    void setConfig(const SV_Web::config& cng);

    QByteArray jsonGetAllSignals();
    
    QByteArray jsonGetDataParams();

    QByteArray jsonGetLastSignalData(const QStringList& snames);

    QByteArray jsonGetAllModules();

private:
    void incomingConnection(qintptr handle) override;

    SV_Web::config cng;

};

class clientSocket : public QTcpSocket
{
    Q_OBJECT
   
    friend int response(http_parser*);

public:
       
    clientSocket(QObject *parent = nullptr);

private: 
    http_parser parser_;

    QString cField_;
    QString reqPage_;

    QMap<QString, QString> reqFields_;
    QStringList reqSignals_;

    webServer* server_ = nullptr;


private slots:
    void readData();
};
