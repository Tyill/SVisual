////
//// SVisual Project
//// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
////
//// This code is licensed under the MIT License.
////
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files(the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions :
////
//// The above copyright notice and this permission notice shall be included in
//// all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//// THE SOFTWARE.
////
//#include <string>
//#define _USE_MATH_DEFINES
//#include <math.h>
//#include <iostream>
//#include <cstring>

//#include "SVClient/SVClient.h"
//#include "SVMisc/misc.h"
//#include "SVMisc/tcp_client.h"
//#include "SVBase/sv_limits.h"

//#ifdef _WIN32
//#include "windows.h"

//BOOL WINAPI CloseHandler(DWORD CEvent)
//{
//  switch (CEvent)
//  {
//  case CTRL_C_EVENT:
//  case CTRL_BREAK_EVENT:
//  case CTRL_CLOSE_EVENT:
//  case CTRL_LOGOFF_EVENT:
//  case CTRL_SHUTDOWN_EVENT:
//    SV::svDisconnect();
//    break;
//  }
//  return TRUE;
//}
//#endif

//bool sendData(const std::string& nmodule, const std::string& nvalue, const std::vector<int>& values);

//int main(int argc, char *argv[]){

//#ifdef _WIN32
//  SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
//#endif

//  if (SV_Misc::TCPClient::connect("127.0.0.1", 2144)){
//      std::cout << "connect " << "client" << " ok" << std::endl;
//  }else{
//      std::cout << "connect no" << std::endl;
//      return 1;
//  }
//  std::vector<int> data(1000000);
//  while (true){
//      for (int i = 0; i < 1000000; ++i){
//          data[i] = i;
//      }
//      sendData("client", "valInt", data);
//      SV_Misc::sleepMs(10000);
//  }
//  return 0;
//}

//bool sendData(const std::string& nmodule, const std::string& nvalue, const std::vector<int>& values) {

//    const char* messBegin = "=begin=";
//    const char* messEnd = "=end=";

//    size_t SINT = sizeof(int32_t),
//           /*      val name    type      vals          */
//           valSz = SV_NAMESZ + SINT + SINT * values.size(),
//           /*       mod name            vals           */
//           dataSz = SV_NAMESZ + valSz * 1,

//           startSz = strlen(messBegin), endSz = strlen(messEnd), offs = 0,
//           /*                dataSz                    */
//           messSz = startSz + SINT + dataSz + endSz;

//    std::string data(messSz, '\0');

//    char* dptr = (char*)data.c_str();
//    memcpy(dptr, messBegin, startSz);         offs += startSz;
//    memcpy(dptr + offs, &dataSz, SINT);       offs += SINT;
//    memcpy(dptr + offs, nmodule.c_str(), SV_NAMESZ); offs += SV_NAMESZ;

//    memcpy(dptr + offs, nvalue.c_str(), SV_NAMESZ);
//    int vtype = 1;
//    memcpy(dptr + offs + SV_NAMESZ, &vtype, SINT);
//    memcpy(dptr + offs + SV_NAMESZ + SINT, values.data(), values.size() * SINT);
//    offs += valSz;

//    memcpy(dptr + offs, messEnd, endSz);

//    std::string out;
//    return SV_Misc::TCPClient::sendData(data, out, false, true);
//}

