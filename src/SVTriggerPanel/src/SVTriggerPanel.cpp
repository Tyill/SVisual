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

    void startUpdateThread(QDialog* stp){

        if (stp)
            ((triggerPanel*)stp)->startUpdateThread();
    }
   	
	void setGetCopySignalRef(QDialog *stp, pf_getCopySignalRef f) {

		if (stp)
            ((triggerPanel*)stp)->pfGetCopySignalRef = f;
	}

    void setGetCopyModuleRef(QDialog* stp, pf_getCopyModuleRef f){

        if (stp)
            ((triggerPanel*)stp)->pfGetCopyModuleRef = f;
    }

    void setGetModuleData(QDialog* stp, pf_getModuleData f){

        if (stp)
            ((triggerPanel*)stp)->pfGetModuleData = f;
    }

	void setGetSignalData(QDialog *stp, pf_getSignalData f) {

		if (stp)
            ((triggerPanel*)stp)->pfGetSignalData = f;
	}

	void setLoadSignalData(QDialog *stp, pf_loadSignalData f) {

		if (stp)
            ((triggerPanel*)stp)->pfLoadSignalData = f;
	}
        	
    // вернуть все триггеры
    QMap<QString, triggerData*> getCopyTriggerRef(QDialog *stp){

        if (stp)
            return ((triggerPanel*)stp)->getCopyTriggerRef();
        else
            return QMap<QString, triggerData*>();
    }

    // вернуть данные триггера
    triggerData* getTriggerData(QDialog *stp, const QString& name){

        if (stp)
            return ((triggerPanel*)stp)->getTriggerData(name);
        else
            return nullptr;
    }

    // добавить триггер
    bool addTrigger(QDialog *stp, const QString& name, triggerData* td){

        if (stp)
            return ((triggerPanel*)stp)->addTrigger(name, td);
        else
            return false;
    }

    void setOnTriggerCBack(QDialog* stp, pf_onTriggerCBack f){

        if (stp)
            ((triggerPanel*)stp)->pfOnTriggerCBack = f;
    }

    /// вернуть тип события как строку
    /// \param type
    /// \return
    QString getEventTypeStr(eventType type) {

        QString res = "none";

        switch (type) {
        case eventType::connectModule:
            res = "connectModule";
            break;
        case eventType::disconnectModule:
            res = "disconnectModule";
            break;
        case eventType::less:
            res = "less";
            break;
        case eventType::equals:
            res = "equals";
            break;
        case eventType::more:
            res = "more";
            break;
        case eventType::posFront:
            res = "posFront";
            break;
        case eventType::negFront:
            res = "negFront";
            break;
        default:
            res = "none";
            break;
        }

        return res;
    }
}