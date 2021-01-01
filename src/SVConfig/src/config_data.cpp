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
#include <iomanip>
#include <sstream>
#include "SVConfig/config_data.h"

using namespace std;

namespace SV_Base {

  /// вернуть тип сигнала как значение
  /// \param str
  /// \return
  ValueType getSVType(const std::string& str) {

    ValueType res = ValueType::INT;
    if (str == "bool") res = ValueType::BOOL;
    else if (str == "float") res = ValueType::FLOAT;
    return res;
  }

  /// вернуть тип сигнала как строку
  /// \param type
  /// \return
  std::string getSVTypeStr(ValueType type) {

    std::string res = "int";
    if (type == ValueType::BOOL) res = "bool";
    else if (type == ValueType::FLOAT) res = "float";
    return res;
  }

  /// вернуть значение как строку
  /// \param vt
  /// \param Value
  /// \return
  std::string getSValue(ValueType vt, double value) {

    std::string res = "";

    switch (vt) {
      case ValueType::BOOL:
        res = value > 0 ? "1" : "0";
        break;
      case ValueType::INT:
        res = value > 0 ? std::to_string((int)(value + 0.5)) : std::to_string((int)(value - 0.5));
        break;
      case ValueType::FLOAT:{
        int prec = 1;
        if (abs(value) < 1.0){
          if (abs(value) > 0.1)         prec = 2;
          else if (abs(value) > 0.01)   prec = 3;
          else if (abs(value) > 0.001)  prec = 4;
          else prec = 5;
        }
        stringstream stream;
        stream << fixed << setprecision(prec) << value;
        res = stream.str();
      }
      break;
      default: break;
    }

    return res;
  }
}

