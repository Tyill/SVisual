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
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "SVClient/SVClient.h"
#include "SVMisc/misc.h"

#ifdef _WIN32
#include "windows.h"

BOOL WINAPI CloseHandler(DWORD CEvent)
{
  switch (CEvent)
  {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_LOGOFF_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    SV::svDisconnect();
    break;
  }
  return TRUE;
}
#endif

int main(int argc, char *argv[]){

  int diap = 9; double vl = 134.6656;

  if (diap > 100) vl = int(vl + 0.5);
  else if (diap > 10) vl = int(vl * 10 + 0.5) / 10.;
  else if (diap > 1) vl = int(vl * 100 + 0.5) / 100.;
  else vl = int(vl * 1000 + 0.5) / 1000.;

#ifdef _WIN32
  SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
#endif

  std::string nm = argc >= 2 ? argv[1] : "client";

  int cyc = argc >= 3 ? atoi(argv[2]) : 100;
  int packSz = argc >= 4 ? atoi(argv[3]) : 10;

  SV::svSetParam(cyc, packSz);

  if (SV::svConnect(nm.c_str(), "127.0.0.1", 2144)){

    std::cout << "connect " << nm.c_str() << " ok" << std::endl;
  }
  else std::cout << "connect no" << std::endl;

  int cp = 0;
  while (true){
        
    std::string val = "sin";
    SV::svAddIntValue("sin", int(sin(cp * M_PI / 180.0) * 100));
    SV::svAddIntValue("абрвал0", int(sin((cp + 1) * M_PI / 180.0) * 100));

    SV::svAddBoolValue("sinb", cp > 100);
    SV::svAddBoolValue("sinb1", cp > 50);
    SV::svAddBoolValue("sinb2", cp > 0);

    cp += 1; if (cp > 359) cp = 0;
      
    SV_Misc::sleepMs(100);
  }
  return 0;
}
