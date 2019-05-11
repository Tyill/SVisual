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

#include "src/stdafx.h"
#include "ui_scriptPanel.h"
#include "SVConfig/SVConfigData.h"
#include "SVScriptPanel.h"
#include "Lua/lua.hpp"

class scriptPanel : public QDialog
{
	Q_OBJECT

public:

	SV_Script::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
	SV_Script::pf_getSignalData pfGetSignalData = nullptr;
	SV_Script::pf_loadSignalData pfLoadSignalData= nullptr;
	
    scriptPanel(QWidget *parent, SV_Script::config);
    ~scriptPanel();
	
private:
    Ui::ScriptPanelClass ui;
	
    QString selDir_;

    SV_Script::config cng;

    lua_State* luaState_ = nullptr; 
    
    struct scriptState{
        bool isChange = true;
        QString name;       
        QString text;
        int tabInx = 0;

        scriptState(QString name_ = "", QString text_ = "", int tabInx_ = -1, bool isChange_ = false) :
            name(name_), text(text_), tabInx(tabInx_), isChange(isChange_){}
    };

    QVector<scriptState> scrState_;
    
    QString exlName(QString);

private slots:
    void addScript(QString name = "");
    void saveScript();
    void nameScriptChange(int row, int col);

};


