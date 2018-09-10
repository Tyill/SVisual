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
#include "SVConfig/SVConfigData.h"


namespace SV_Cng {

    /// вернуть тип сигнала как значение
    /// \param str
    /// \return
    valueType getSVType(std::string str) {

        valueType res = valueType::tInt;

        if (str == "bool") res = valueType::tBool;
        else if (str == "float") res = valueType::tFloat;

        return res;
    }

    /// вернуть тип сигнала как строку
    /// \param type
    /// \return
    std::string getSVTypeStr(valueType type) {

        std::string res = "int";

        if (type == valueType::tBool) res = "bool";
        else if (type == valueType::tFloat) res = "float";

        return res;
    }

    /// вернуть значение как строку
    /// \param vt
    /// \param value
    /// \return
    std::string getSValue(valueType vt, double value) {

        std::string res = "";

        switch (vt) {
            case valueType::tBool:
                res = value > 0 ? "1" : "0";
                break;
            case valueType::tInt:
                res = value > 0 ? std::to_string((int) (value + 0.5)) : std::to_string((int) (value - 0.5));
                break;
            case valueType::tFloat:
                res = std::to_string(value);
                break;
            default:
                break;
        }

        return res;
    }

    /// вернуть тип события как строку
    /// \param type
    /// \return
    std::string getEventTypeStr(eventType type) {

        std::string res = "none";

        switch (type) {
            case eventType::connectModule:
                res = "connectModule";
                break;
            case eventType::disconnectModule:
                res = "disconnectModule";
                break;
            case eventType::less:
                res = "less";
                break;
            case eventType::equals:
                res = "equals";
                break;
            case eventType::more:
                res = "more";
                break;
            case eventType::posFront:
                res = "posFront";
                break;
            case eventType::negFront:
                res = "negFront";
                break;
            default:
                res = "none";
                break;
        }

        return res;
    }
}


