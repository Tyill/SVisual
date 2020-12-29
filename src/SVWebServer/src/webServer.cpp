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
#include "webServer.h"
#include "SVConfig/config_data.h"
#include "SVConfig/config_limits.h"
#include "http_parser.h"
#include <QJsonDocument>
#include <QDir>
#include <QUrl>

void webServer::setConfig(const SV_Web::config& cng_){

    cng = cng_;
}

void webServer::incomingConnection(qintptr handle){

    clientSocket* socket = new clientSocket(this);
    socket->setSocketDescriptor(handle);
        
    connect(socket, SIGNAL(readyRead()), socket, SLOT(readData()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

clientSocket::clientSocket(QObject* parent) 
    : QTcpSocket(parent){
    
    http_parser_init(&parser_, HTTP_REQUEST);

    parser_.data = this;

    server_ = (webServer*)parent;
}

int response(http_parser* parser);

void clientSocket::readData(){
       
    http_parser_settings parser_settings{0};
    
    parser_settings.on_url = [](http_parser* parser, const char* url, size_t length){
    
        struct http_parser_url u{ 0 };
        if (http_parser_parse_url(url, length, 0, &u) != 0)
            return -1;        

        auto socket = (clientSocket*)parser->data;
        socket->reqSignals_.clear();

        if (u.field_set & (1 << UF_PATH)) {
            for (int i = 0; i < UF_MAX; ++i){

                QString fld = QString(url).mid(u.field_data[i].off, u.field_data[i].len);

                if (fld.isEmpty()) continue;

                if (fld[0] == '/')
                    socket->reqPage_ = fld;
                else if (fld.startsWith("sname"))
                    socket->reqSignals_ = fld.split('&');
            }
        }
        return 0;
    };
    parser_settings.on_header_field = [](http_parser* parser, const char* url, size_t length){

        QString fld = QString::fromLocal8Bit(url, length);
        
        ((clientSocket*)parser->data)->cField_ = fld;
        ((clientSocket*)parser->data)->reqFields_[fld] = "";
        
        return 0;
    };
    parser_settings.on_header_value = [](http_parser* parser, const char* url, size_t length){

        QString fld = ((clientSocket*)parser->data)->cField_;

        ((clientSocket*)parser->data)->reqFields_[fld] = QString::fromLocal8Bit(url, length);

        return 0;
    };
    parser_settings.on_message_complete = response;

    auto buff = this->readAll();

    size_t nread = buff.size(),
           parsed = http_parser_execute(&parser_, &parser_settings, buff.data(), nread);

    if (parsed < nread) {
               
        QByteArray resp;
        resp += QString("HTTP/1.1 400 Bad Request\r\n")
            + "\r\n";

        this->writeData(resp.data(), resp.size());
    }
}

int response(http_parser* parser){
      
    clientSocket* socket = (clientSocket*)parser->data;

    QString& page = socket->reqPage_;
      
    if (page.startsWith("/api")){

        QByteArray json;

        if (page == "/api/allSignals")
            json = socket->server_->jsonGetAllSignals();
        else if (page == "/api/dataParams")
            json = socket->server_->jsonGetDataParams();
        else if (page == "/api/lastSignalData")
            json = socket->server_->jsonGetLastSignalData(socket->reqSignals_);
        else if (page == "/api/allModules")
            json = socket->server_->jsonGetAllModules();

        QByteArray resp;
        resp += QString("HTTP/1.1 200 OK\r\n")
            + "Content-Type: application/json; charset=utf-8\r\n"
            + "Content-Length: " + QString::number(json.size()) + "\r\n"
            + "Connection: keep-alive\r\n"           
            + "\r\n";

        resp += json;

        socket->writeData(resp.data(), resp.size());
    }
    else{

        if (page == "/")
            page = "/index.html";

        auto& fields = socket->reqFields_;

        if (!fields.contains("Accept") || fields["Accept"].contains("text/html"))
            fields["Accept"] = "text/html";
        else if (fields["Accept"].contains("text/css"))
            fields["Accept"] = "text/css";

        QByteArray html;

        QFile file(QCoreApplication::applicationDirPath() + "/web" + page);
        if (file.exists()){

            file.open(QIODevice::ReadOnly);

            html = file.readAll();

            file.close();
        }

        QByteArray resp;
        resp += QString("HTTP/1.1 200 OK\r\n")
            + "Content-Type: " + fields["Accept"] + "\r\n"
            + "Connection: keep-alive\r\n"
            + "Content-Length: " + QString::number(html.size()) + "\r\n"
            + "\r\n";

        resp += html;

        socket->writeData(resp.data(), resp.size());
    }

    return 0;
}

QByteArray webServer::jsonGetAllSignals(){
       
    QJsonObject jnObject;
        
    auto sref = pfGetCopySignalRef();
    for (auto sign : sref){
            
        QJsonObject jnSign;
        jnSign["name"] = QString::fromStdString(sign->name);
        jnSign["module"] = QString::fromStdString(sign->module);
        jnSign["type"] = int(sign->type);
        jnSign["group"] = QString::fromStdString(sign->group);
        jnSign["comment"] = QString::fromStdString(sign->comment);
        jnSign["isActive"] = sign->isActive;

        jnObject[QString::fromStdString(sign->name + sign->module)] = jnSign;
    }

    QJsonDocument jsDoc(jnObject);
   
    return jsDoc.toJson();
}

QByteArray webServer::jsonGetDataParams(){

    QJsonObject jnParams;
    jnParams["packetSize"] = SV_PACKETSZ;
    jnParams["cycleTimeMs"] = SV_CYCLEREC_MS;

    QJsonDocument jsDoc(jnParams);

    return jsDoc.toJson();
}

QByteArray webServer::jsonGetLastSignalData(const QStringList& snames){
        
    QJsonObject jnObject;
            
    for (auto& spair : snames){
        
        auto sname = spair.split("=")[1];

        sname = QUrl(sname).toString();

        auto sd = pfGetSignalData(sname);

        if (!sd) continue;

        QJsonObject jnSign;
        jnSign["beginTime"] = qint64(sd->lastData.beginTime);
                
        QJsonArray jnVals;
        for (int i = 0; i < SV_PACKETSZ; ++i){

            switch (sd->type){
              case SV_Base::ValueType::BOOL: jnVals.append(sd->lastData.vals[i].BOOL); break;
              case SV_Base::ValueType::INT: jnVals.append(sd->lastData.vals[i].INT); break;
              case SV_Base::ValueType::FLOAT: jnVals.append(sd->lastData.vals[i].FLOAT); break;
            }
        }
        jnSign["vals"] = jnVals;

        jnObject[sname] = jnSign;
    }

    QJsonDocument jsDoc(jnObject);

    return jsDoc.toJson();
}

QByteArray webServer::jsonGetAllModules(){
        
    QJsonObject jnObject;

    auto mref = pfGetCopyModuleRef();
    
    for (auto m : mref){

        QJsonObject jnMod;
        jnMod["isActive"] = m->isActive;

        jnObject[QString::fromStdString(m->module)] = jnMod;
    }

    QJsonDocument jsDoc(jnObject);

    return jsDoc.toJson();
}
