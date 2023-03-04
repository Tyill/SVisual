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
#include "SVMisc/tcp_server.h"
#include "SVServer/server.h"
#include <iostream>

using  namespace std;

void statusMess(const string& mess){

  cout << mess << endl;
}

int main(int argc, char* argv[]){

  SV_Misc::TCPServer::setErrorCBack(statusMess);
  SV_Srv::setStatusCBack(statusMess);

  SV_Misc::TCPServer::setDataCBack(SV_Srv::receiveData);

  SV_Srv::Config scng;
  scng.outArchiveEna = true;

  string addr = argc > 1 ? argv[1] : "127.0.0.1";
  int port = argc > 2 ? atoi(argv[2]) : 2144;

  if (SV_Srv::startServer(scng) && SV_Misc::TCPServer::start(addr, port)){
    statusMess("Run " + addr + " " + to_string(port));
    while (true)
      SV_Misc::sleepMs(1000);
  }
  else
  {
    statusMess("No run " + addr + " " + to_string(port));
    cin.get();

  }
  return 0;
}

