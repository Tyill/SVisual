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

#include "SVServer/server.h"
#include "SVBase/limits.h"

#include <mutex>

/// циклический буфер данных
class BufferData
{
public:
   
  BufferData() = default;

  /// инициализировать буфер
  void init(const SV_Srv::Config&);

  /// входная переменная
  struct InputData{
    bool isActive = false;     
    std::string name;          
    std::string module;        
    SV_Base::ValueType type;   
    SV_Base::RecData data;     
  };

  /// обновить данные буфера
  /// \param in новые данные
  /// \param bTm отметка времени, мс
  /// \return true - ok
  void updateDataSignals(const std::string& in, uint64_t bTm);

  /// вернуть данные по текущей позиции чтения
  /// \return
  InputData getDataByReadPos();

  /// инкремент позиции чтения
  void incReadPos();
  
  static const size_t BUFF_SZ;

private:
    
  SV_Srv::Config cng;

  /// данные  
  InputData _buffer[SV_VALUE_MAX_CNT * 10]; // 10 сек - запас;

  size_t _buffReadPos = 0;  ///< тек позиция чтения
  size_t _buffWritePos = 0; ///< тек позиция записи

  std::mutex _mtx;
};
