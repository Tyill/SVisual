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

#include "SVAuxFunc/aux_func.h"
#include "SVAuxFunc/tcp_client.h"

#include <cstring>

namespace SV_Aux{
  namespace TCPClient {

#ifdef WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET _socket = INVALID_SOCKET;
         
bool connect(const std::string& addr, int port, bool noBlock) {

  WSADATA wsaData;
  _socket = INVALID_SOCKET;
  struct addrinfo *adInfo = NULL, hints;

  // Initialize Winsock  
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    return false;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  char cport[11];
  (sprintf)(cport, "%d", port); 
  if (getaddrinfo(addr.c_str(), cport, &hints, &adInfo) != 0) {
    WSACleanup();
    return false;
  }

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket == INVALID_SOCKET) {
    WSACleanup();
    return false;
  }

  // connects to server.
  if (connect(_socket, adInfo->ai_addr, (int)adInfo->ai_addrlen) == SOCKET_ERROR) {
    closesocket(_socket);
    return false;
  }

  if (noBlock) {
    BOOL l = TRUE;
    int er = ioctlsocket(_socket, FIONBIO, (unsigned long *)&l);
  }

  return true;
}

bool disconnect() {

  if (_socket != INVALID_SOCKET){
    shutdown(_socket, SD_BOTH);
    closesocket(_socket);
    WSACleanup();
  }
  return true;
}

int sendAll(const std::string& mess, int flags){

  int total = 0,
      len = int(mess.size());
  while (total < len){
    int n = send(_socket, mess.c_str() + total, len - total, flags);
    if (n == SOCKET_ERROR){
      total = SOCKET_ERROR;
      break;
    }
    total += n;
  }
  return total;
}

bool sendData(const std::string& in, std::string &out, bool disconn, bool onlySend) {

  if (sendAll(in, 0) == SOCKET_ERROR) {
    disconnect();
    return false;
  }

  if (!onlySend) {

    // Receive until the peer closes the connection
    const int recvbuflen = 2048;
    static char recvbuf[recvbuflen];

    out.clear();
    while (true) {
      int rlen = recv(_socket, recvbuf, recvbuflen, 0);
      if (rlen > 0) {
        size_t csz = out.size();
        out.resize(csz + size_t(rlen));
        memcpy((char*)out.data() + csz, recvbuf, rlen);

        if (recvbuf[rlen - 1] == '\0') break;
      }
      else if (WSAGetLastError() == WSAEWOULDBLOCK) {
        SV_Aux::sleepMs(0);
      }
      else break;
    }
  }

  if (disconn) disconnect();

  return true;
}
    
#else

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int _socket = -1;

bool connect(const std::string& addr, int port, bool noBlock) {

  sockaddr_in saddr;

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if(_socket < 0)
    return false;

  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  if (inet_aton(addr.c_str(), &saddr.sin_addr) == 0)
    return false;

  if(connect(_socket, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
    return false;

  if (noBlock)
    fcntl(_socket, F_SETFL, O_NONBLOCK);

  return true;
}

bool disconnect() {

  if (_socket > 0) close(_socket);
  return true;
}

int sendAll(const std::string& mess, int flags){

  int total = 0,
      len = int(mess.size());
  while (total < len) {
    int n = send(_socket, mess.c_str() + total, len - total, flags);
    if (n == -1) {
      total = -1;
      break;
    }
    total += n;
  }
  return total;
}

bool sendData(const std::string& in, std::string &out, bool disconn, bool onlySend) {

  // Send an initial buffer
  int ret = sendAll(in, 0);
  if (ret == -1) {
    disconnect();
    return false;
  }

  if (!onlySend) {

    // Receive until the peer closes the connection
    const int recvbuflen = 2048;
    static char recvbuf[recvbuflen];
    out.clear();
    while (true) {
      int rlen = recv(_socket, recvbuf, recvbuflen, 0);
      if (rlen > 0) {

        size_t csz = out.size();
        out.resize(csz + rlen);
        memcpy((char*) out.data() + csz, recvbuf, rlen);

        if (recvbuf[rlen - 1] == '\0') break;
      } 
      else if (rlen == -1) {
        SV_Aux::sleepMs(0);
      } 
      else break;
    }
  }

  if (disconn) disconnect();

  return true;
}

#endif
    
}
}