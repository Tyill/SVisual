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

#include "SVViewer/forms/ts_database_dialog.h"

TsDataBaseDialog::TsDataBaseDialog(QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.btnSelect, &QPushButton::clicked, this, [this](){
        const auto dtBegin = ui.beginDTime->dateTime();
        const auto dtEnd = ui.endDTime->dateTime();
        if (dtBegin.isValid() && dtEnd.isValid() && dtBegin < dtEnd){
            accept();
        }else{
            reject();
        }
    });
}

TsDataBaseDialog::~TsDataBaseDialog()
{
}

void TsDataBaseDialog::setInterval(const QPair<QDateTime, QDateTime>& dt)
{
    ui.beginDTime->setDateTime(dt.first);
    ui.endDTime->setDateTime(dt.second);
}

QPair<QDateTime, QDateTime> TsDataBaseDialog::getInterval()const
{
    return {ui.beginDTime->dateTime(), ui.endDTime->dateTime()};
}
