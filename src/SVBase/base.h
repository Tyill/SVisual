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
#include <cstdint>

namespace SV_Base {

  /// тип польз переменной
  enum class ValueType : uint32_t {
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
    uint64_t beginTime{};  ///< нач время, мс
    Value *vals{};         ///< данные
  };

  /// сигнал
  struct SignalData {
    int id{};
    bool isActive{};       ///< активен
    bool isDelete{};       ///< удален
    bool isBuffEnable{};   ///< буфер разрешен

    std::string name;      ///< имя
    std::string module;    ///< модуль
    std::string group;     ///< группа
    std::string comment;   ///< комментарий

    ValueType type{};      ///< тип

    RecData lastData;      ///< последняя запись

    ///// буфер данных
    std::vector<RecData> buffData;
    size_t buffBeginPos{};
    size_t buffValuePos{};
    uint64_t buffMinTime{INT64_MAX}, buffMaxTime{};
    double buffMinValue{}, buffMaxValue{};
    ///////
  };

  /// модуль
  struct ModuleData {
    bool isActive{};                 ///< активен
    bool isEnable{};                 ///< разрешен
    bool isDelete{};                 ///< удален
    std::string module;              ///< название
    std::vector<std::string> signls; ///< список сигналов

    ModuleData(const std::string& modul) {
      module = modul;
    }
  };

  /// группа сигналов
  struct GroupData {
    bool isActive{};                   ///< активна
    std::string group;                 ///< название
    std::vector<std::string> signls;   ///< список сигналов

    GroupData(const std::string& grp) {
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