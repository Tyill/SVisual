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
#include <functional>

namespace SV_Aux {
  namespace TCPServer {

    using Server = void*;
        
    /// польз callback - получение данных
    using DataCBack = std::function<void(std::string& inout, std::string& out)>;

    using ErrorCBack = std::function<void(const std::string& err)>;
    
    /// создать сервер
    /// \param DataCBack
    /// \param ErrorCBack
    /// \return Server
    Server create(DataCBack, ErrorCBack);
       
    /// запустить сервер
    /// \param addr
    /// \param port
    /// \return true - ok
    bool start(Server, const std::string& addr, uint16_t port);

    /// остановить сервер
    void stop(Server);       
  }
}
