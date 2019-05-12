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

#include <thread>
#include <mutex>
#include <map>

#include "src/stdafx.h"
#include "ui_scriptPanel.h"
#include "SVConfig/SVConfigData.h"
#include "SVScriptPanel.h"
#include "Lua/lua.hpp"

struct boolValue{
    bool value;
    uint64_t time;
    boolValue(bool value_ = false, uint64_t time_ = 0) : value(value_), time(time_){}
};
struct intValue{
    int value;
    uint64_t time;
    intValue(int value_ = 0, uint64_t time_ = 0) : value(value_), time(time_){}
};
struct floatValue{
    float value;
    uint64_t time;
    floatValue(float value_ = 0, uint64_t time_ = 0) : value(value_), time(time_){}
};

class scriptPanel : public QDialog
{
    Q_OBJECT

    friend boolValue getBoolValue(const std::string& module, const std::string& signal);
    friend intValue getIntValue(const std::string& module, const std::string& signal);
    friend floatValue getFloatValue(const std::string& module, const std::string& signal);
    friend void setBoolValue(const std::string& signal, boolValue value);
    friend void setIntValue(const std::string& signal, intValue value);
    friend void setFloatValue(const std::string& signal, floatValue value);

public:
    
    SV_Script::pf_updateSignalsCBack pfUpdateSignalsCBack = nullptr;
    SV_Script::pf_addSignalsCBack pfAddSignalsCBack = nullptr;
	SV_Script::pf_getCopySignalRef pfGetCopySignalRef = nullptr;
	SV_Script::pf_getSignalData pfGetSignalData = nullptr;
    SV_Script::pf_getModuleData pfGetModuleData = nullptr;
    SV_Script::pf_addSignal pfAddSignal = nullptr;
    SV_Script::pf_addModule pfAddModule = nullptr;
    SV_Script::pf_moduleConnectCBack pfModuleConnectCBack = nullptr;
	SV_Script::pf_loadSignalData pfLoadSignalData= nullptr;
	
    scriptPanel(QWidget *parent, SV_Script::config, SV_Script::modeGr mode);
    ~scriptPanel();

    void startUpdateThread();
	
private:
    Ui::ScriptPanelClass ui;
	
    QString selDir_;

    SV_Script::modeGr mode_;

    SV_Script::config cng;

    lua_State* luaState_ = nullptr; 
    
    std::thread workThr_;
    std::mutex mtx_;
    bool isStopWork_ = false;

    std::map<std::string, SV_Cng::signalData *> signBuff_;

    int iterValue_ = 0;
   
    struct scriptState{
        bool isChange = true;
        bool isActive = false;
        QString name;       
        QString text;
        int tabInx = 0;

        scriptState(QString name_ = "", QString text_ = "", int tabInx_ = -1, bool isChange_ = false) :
            name(name_), text(text_), tabInx(tabInx_), isChange(isChange_){}
    };

    QVector<scriptState> scrState_;
    
    QString exlName(QString);

    void updateSign(SV_Cng::signalData* sign, int beginPos, int valuePos);
    bool updateBuffValue(const std::string& module, const std::string& signal, SV_Cng::valueType stype);

    void setValue(const std::string& signal, SV_Cng::value value, uint64_t time);

    void workCycle();

private slots:
    void addScript(QString name = "");
    void saveScript();
    void nameScriptChange(int row, int col);

};


