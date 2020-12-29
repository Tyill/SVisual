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
#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace SV_Base {

  /// тип польз переменной
  enum class ValueType {
    BOOL = 0,
    INT = 1,
    FLOAT = 2,
  };

  /// значение польз переменной
  union Value {
    bool vBool;
    int vInt;
    float vFloat;
  };

  /// пакет записи
  struct RecData {
    uint64_t beginTime;  ///< нач время, мс
    Value *vals;         ///< данные

    RecData() : beginTime(0), vals(nullptr) {};
  };

  /// сигнал
  struct SignalData {

    bool isActive;          ///< активен
    bool isDelete;          ///< удален
    bool isBuffEnable;      ///< буфер разрешен

    std::string name;       ///< имя
    std::string module;     ///< модуль
    std::string group;      ///< группа
    std::string comment;    ///< комментарий

    ValueType type;         ///< тип

    RecData lastData;       ///< последняя запись

    ///// буфер данных
    std::vector<RecData> buffData;
    int buffBeginPos;
    int buffValuePos;
    uint64_t buffMinTime, buffMaxTime;
    double buffMinValue, buffMaxValue;
    ///////

    SignalData() {
      isActive = false;
      isDelete = false;
      isBuffEnable = false;

      type = ValueType::BOOL;

      buffBeginPos = 0;
      buffValuePos = 0;
      buffMinTime = INT32_MAX;
      buffMaxTime = INT32_MIN;
      buffMinValue = 0;
      buffMaxValue = 0;
    }
  };

  /// модуль
  struct ModuleData {

    bool isActive;                   ///< активен
    bool isEnable;                   ///< разрешен
    bool isDelete;                   ///< удален
    std::string module;              ///< название
    std::vector<std::string> signls; ///< список сигналов

    /// модуль
    /// \param modul название модуля
    ModuleData(const std::string& modul) {

      isActive = false;
      isEnable = false;
      isDelete = false;
      module = modul;
    }
  };

  /// группа сигналов
  struct GroupData {

    bool isActive;                     ///< активна
    std::string group;                 ///< название
    std::vector<std::string> signls;   ///< список сигналов

    GroupData(const std::string& grp) {
      isActive = true;
      group = grp;
    }
  };

  /// вернуть тип сигнала как значение
  /// \param str
  /// \return
  ValueType getSVType(const std::string& str);

  /// вернуть тип сигнала как строку
  /// \param type
  /// \return
  std::string getSVTypeStr(ValueType type);

  /// вернуть значение как строку
  /// \param vt
  /// \param Value
  /// \return
  std::string getSValue(ValueType vt, double Value);

}