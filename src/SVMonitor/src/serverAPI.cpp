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
#include "SVServer/SVServer.h"

QMap<QString, SV_Cng::moduleData *> getCopyModuleRefSrv(){

	QMap<QString, SV_Cng::moduleData *> cmref;

	auto mref = SV_Srv::getCopyModuleRef();
	for (auto& s : mref)
		cmref.insert(QString::fromStdString(s.first), s.second);

	return cmref;
};

QMap<QString, SV_Cng::signalData *> getCopySignalRefSrv(){

	QMap<QString, SV_Cng::signalData*> csref;

	auto sref = SV_Srv::getCopySignalRef();
	for (auto& s : sref)
		csref.insert(QString::fromStdString(s.first), s.second);

	return csref;
};

SV_Cng::moduleData* getModuleDataSrv(const QString& module){

    return SV_Srv::getModuleData(module.toUtf8().data());
};

QVector<QString> getModuleSignalsSrv(const QString& module){

    auto msign = SV_Srv::getModuleSignals(module.toUtf8().data());

    QVector<QString> res;
    for (auto& s : msign){
        res.push_back(QString::fromStdString(s));
    }
    return res;
};

SV_Cng::signalData* getSignalDataSrv(const QString& sing){

	return SV_Srv::getSignalData(sing.toUtf8().data());
};

bool loadSignalDataSrv(const QString& name){

    return SV_Srv::signalBufferEna(name.toUtf8().data());
}

bool addSignalSrv(SV_Cng::signalData* sd){

    return SV_Srv::addSignal(sd);
}

bool addModuleSrv(SV_Cng::moduleData* md){

    return SV_Srv::addModule(md);
}