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
#include "forms/triggerPanel.h"
#include "SVTriggerPanel/SVTriggerPanel.h"

namespace SV_Trigger {

    QDialog *createTriggerPanel(QWidget *parent, config cng) {

        return new triggerPanel(parent, cng);
	}
   	
	void setGetCopySignalRef(QDialog *stp, pf_getCopySignalRef f) {

		if (stp)
            ((triggerPanel *)stp)->pfGetCopySignalRef = f;
	}

    void setGetModuleData(QDialog* stp, pf_getModuleData f){

        if (stp)
            ((triggerPanel *)stp)->pfGetModuleData = f;
    }

	void setGetSignalData(QDialog *stp, pf_getSignalData f) {

		if (stp)
            ((triggerPanel *)stp)->pfGetSignalData = f;
	}

	void setLoadSignalData(QDialog *stp, pf_loadSignalData f) {

		if (stp)
            ((triggerPanel *)stp)->pfLoadSignalData = f;
	}
    
    // сработал триггер callBack
    void setOnTriggerCBack(onTriggerCBack cback){

		serv.pfTriggerCBack = cback;
	}
	
    // вернуть все триггеры
	std::map<std::string, SV_Cng::triggerData*> getCopyTriggerRef(){

		return serv.getCopyTriggerRef();
    }

    // вернуть данные триггера
    SV_Cng::triggerData *getTriggerData(const string& name){

		return serv.getTriggerData(name);
    }

    // добавить триггер
    bool addTrigger(const string& name, SV_Cng::triggerData* td){

		return serv.addTrigger(name, td);
    }

    // удалить триггер
    bool delTrigger(const string& name){

		return serv.delTrigger(name);
    }
}