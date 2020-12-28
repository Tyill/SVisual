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

#include "SVConfig/SVConfigData.h"
#include "SVConfig/SVConfigLimits.h"
#include <mutex>

/// циклический буфер данных
class bufferData
{
public:

    struct config{
        int cycleRecMs;           ///< период записи - задает пользователь
        int packetSz;             ///< размер пакета - задает пользователь

        config(int cycleRecMs_ = 100, int packetSz_ = 10) :cycleRecMs(cycleRecMs_), packetSz(packetSz_){}
    };

    bufferData(bufferData::config);

  /// входная переменная
  struct inputData{
    bool isActive = false;     ///< активна
    std::string name;          ///< имя
    std::string module;        ///< модуль
    SV_Base::ValueType type;    /// тип
        SV_Base::RecData data;      /// данные
  };

  /// обновить данные буфера
  /// \param in новые данные
  /// \param bTm отметка времени, мс
  /// \return true - ok
  void updDataSignals(const std::string& in, uint64_t bTm);

  /// вернуть данные по текущей позиции чтения
    /// \return
  inputData getDataByReadPos();

    /// инкремент позиции чтения
  void incReadPos();

    /// размер буфера
    /// \return
  int getBuffSize();

  private:

    /// запись
    struct valueData{
        char name[SV_NAMESZ];
        SV_Base::ValueType type;
        SV_Base::Value* vals;

        valueData() : vals(NULL){};
    };

    config cng;

    /// данные
    static const int buffSz_ = SV_VALUE_MAX_CNT * 10; // 10 сек - запас
    inputData buffer_[buffSz_];

    int buffReadPos_ = 0;  ///< тек позиция чтения
  int buffWritePos_ = 0; ///< тек позиция записи

  std::mutex mtx_;
};
