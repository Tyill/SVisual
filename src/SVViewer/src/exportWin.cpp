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
#include "forms/exportWin.h"

exportWin::exportWin(QWidget *parent){

	setParent(parent);
		
	ui.setupUi(this);

	/*ui.txtBeginDate->setCalendarPopup(true);
	ui.txtEndDate->setCalendarPopup(true);


	connect(ui.btnShowOrder, SIGNAL(clicked()), this, SLOT(showOrder()));
*/
}

exportWin::~exportWin(){}

void exportWin::showEvent(QShowEvent * event){

	/*ui.txtBeginDate->setDateTime(QDateTime::currentDateTime());
	ui.txtBeginDate->setTime(QTime::fromString("00:00"));

	ui.txtEndDate->setDateTime(QDateTime::currentDateTime());

	showOrder();*/
}