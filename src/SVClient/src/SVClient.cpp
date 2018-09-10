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
#include "SVClient/SVClient.h"
#include "client.h"

client clientObj;

namespace SV_Cln {

    bool svConnect(const char *moduleName, const char *ipAddr, int port) {

        return clientObj.connect(moduleName, ipAddr, port);
    }

    void svDisconnect() {

        clientObj.disconnect();
    }

    bool svAddBoolValue(const char *name, bool value_in, bool onlyPosFront) {

        SV_Cng::value val;
        val.tBool = value_in;

        return clientObj.addValue(name, SV_Cng::valueType::tBool, val, onlyPosFront);
    }

    bool svAddIntValue(const char *name, int value_in) {

        SV_Cng::value val;
        val.tInt = value_in;

        return clientObj.addValue(name, SV_Cng::valueType::tInt, val, false);
    }

    bool svAddFloatValue(const char *name, float value_in) {

        SV_Cng::value val;
        val.tFloat = value_in;

        return clientObj.addValue(name, SV_Cng::valueType::tFloat, val, false);
    }

    bool svSetParam(int cycleRecMs, int packetSz) {

        clientObj.setConfig(client::config(cycleRecMs, packetSz));

        return true;
    }
}