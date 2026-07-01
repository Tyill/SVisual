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

#include "SVMisc/misc.h"
#include "SVMisc/tcp_client.h"

#include <cstring>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

namespace SV_Misc {
namespace TCPClient {

#ifdef WIN32

namespace {

SOCKET _socket = INVALID_SOCKET;
bool _wsaStarted = false;

bool ensureWsa() {
  if (_wsaStarted)
    return true;
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    return false;
  _wsaStarted = true;
  return true;
}

void releaseWsa() {
  if (!_wsaStarted)
    return;
  WSACleanup();
  _wsaStarted = false;
}

void closeSocket() {
  if (_socket != INVALID_SOCKET) {
    shutdown(_socket, SD_BOTH);
    closesocket(_socket);
    _socket = INVALID_SOCKET;
  }
}

} // namespace

bool connect(const std::string& addr, int port, bool noBlock) {
  closeSocket();

  if (!ensureWsa())
    return false;

  addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  char cport[11];
  (sprintf)(cport, "%d", port);

  addrinfo* adInfo = nullptr;
  if (getaddrinfo(addr.c_str(), cport, &hints, &adInfo) != 0)
    return false;

  _socket = socket(AF_INET, SOCK_STREAM, 0);
  if (_socket == INVALID_SOCKET) {
    freeaddrinfo(adInfo);
    return false;
  }

  if (::connect(_socket, adInfo->ai_addr, (int)adInfo->ai_addrlen) == SOCKET_ERROR) {
    freeaddrinfo(adInfo);
    closeSocket();
    return false;
  }

  freeaddrinfo(adInfo);

  if (noBlock) {
    u_long mode = 1;
    ioctlsocket(_socket, FIONBIO, &mode);
  }

  return true;
}

bool disconnect() {
  closeSocket();
  releaseWsa();
  return true;
}

int sendAll(const std::string& mess, int flags) {
  int total = 0;
  const int len = int(mess.size());
  while (total < len) {
    const int n = send(_socket, mess.c_str() + total, len - total, flags);
    if (n == SOCKET_ERROR)
      return SOCKET_ERROR;
    total += n;
  }
  return total;
}

bool sendData(const std::string& in, std::string& out, bool disconn, bool onlySend) {
  if (_socket == INVALID_SOCKET)
    return false;

  if (sendAll(in, 0) == SOCKET_ERROR) {
    disconnect();
    return false;
  }

  if (!onlySend) {
    const int recvbuflen = 2048;
    static char recvbuf[recvbuflen];

    out.clear();
    while (true) {
      const int rlen = recv(_socket, recvbuf, recvbuflen, 0);
      if (rlen > 0) {
        const size_t csz = out.size();
        out.resize(csz + size_t(rlen));
        memcpy((char*)out.data() + csz, recvbuf, rlen);

        if (recvbuf[rlen - 1] == '\0')
          break;
      } else if (rlen == 0) {
        break;
      } else if (rlen == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) {
        SV_Misc::sleepMs(1);
      } else {
        break;
      }
    }
  }

  if (disconn)
    disconnect();

  return true;
}

#else

namespace {

int _socket = -1;

void closeSocket() {
  if (_socket >= 0) {
    ::close(_socket);
    _socket = -1;
  }
}

} // namespace

bool connect(const std::string& addr, int port, bool noBlock) {
  closeSocket();

  _socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (_socket < 0)
    return false;

  sockaddr_in saddr{};
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  if (inet_aton(addr.c_str(), &saddr.sin_addr) == 0) {
    closeSocket();
    return false;
  }

  if (::connect(_socket, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) < 0) {
    closeSocket();
    return false;
  }

  if (noBlock)
    fcntl(_socket, F_SETFL, O_NONBLOCK);

  return true;
}

bool disconnect() {
  closeSocket();
  return true;
}

int sendAll(const std::string& mess, int flags) {
  int total = 0;
  const int len = int(mess.size());
  while (total < len) {
    const int n = send(_socket, mess.c_str() + total, len - total, flags);
    if (n == -1)
      return -1;
    total += n;
  }
  return total;
}

bool sendData(const std::string& in, std::string& out, bool disconn, bool onlySend) {
  if (_socket < 0)
    return false;

  const int ret = sendAll(in, 0);
  if (ret == -1) {
    disconnect();
    return false;
  }

  if (!onlySend) {
    const int recvbuflen = 2048;
    static char recvbuf[recvbuflen];
    out.clear();
    while (true) {
      const int rlen = recv(_socket, recvbuf, recvbuflen, 0);
      if (rlen > 0) {
        const size_t csz = out.size();
        out.resize(csz + rlen);
        memcpy((char*)out.data() + csz, recvbuf, rlen);

        if (recvbuf[rlen - 1] == '\0')
          break;
      } else if (rlen == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        SV_Misc::sleepMs(1);
      } else {
        break;
      }
    }
  }

  if (disconn)
    disconnect();

  return true;
}

#endif

}
}
