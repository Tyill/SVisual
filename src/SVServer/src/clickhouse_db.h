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

#include <memory>
#include <map>
#include <mutex>
#include <vector>

namespace clickhouse{
    class Client;
    class Block;
    class Column;
}
namespace SV_Base{
    struct RecData;
}

class ClickHouseDB{

public:
    ClickHouseDB(const SV_Srv::Config&);

    bool isConnect()const;

    void addSignal(const std::string& sname, const std::string& module);

    void addSData(const std::map<std::string, uint32_t>& valPos, const std::map<std::string, std::vector<SV_Base::RecData>>&);

private:
    std::unique_ptr<clickhouse::Client> newClient()const;
    std::unique_ptr<clickhouse::Block> newSignalBlock()const;
    std::unique_ptr<clickhouse::Block> newSDataBlock()const;

    std::shared_ptr<clickhouse::Column> column(const std::unique_ptr<clickhouse::Block>&, const std::string& colName)const;

private:
    SV_Srv::Config cng;

    std::map<std::string, int> m_signals;  // key - sname, value - id
    std::unique_ptr<clickhouse::Block> m_signalBlock;

    std::mutex m_mtx;
};
