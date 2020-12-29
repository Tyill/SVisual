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

//=============================================================================
// SVisual
// proj: SVClient - for send data
// ver: 1.0.1
//=============================================================================

#pragma once

#ifdef _WIN32
  #ifdef SVCLDLL_EXPORTS
    #define SV_API extern "C" __declspec(dllexport)
  #else
    #define SV_API extern "C" __declspec(dllimport)
  #endif
#else
  #define SV_API extern "C"
#endif

namespace SV{

  // connect of server
  // moduleName - module (leng max 24)
  // ipAddrServ - ip
  SV_API bool svConnect(const char *moduleName, const char *ipAddrServ, int portServ);

  // disconnect of server
  SV_API void svDisconnect();

  // add Value for rec
  // name - Value name (leng max 24)
  SV_API bool svAddBoolValue(const char *name, bool Value, bool onlyFront = false);

  // add Value for rec
  // name - Value name (leng max 24)
  SV_API bool svAddIntValue(const char *name, int Value);

  // add Value for rec
  // name - Value name (leng max 24)
  SV_API bool svAddFloatValue(const char *name, float Value);

  // frecuence rec
  SV_API bool svSetParam(int cycleRecMs, int packetSz);
}