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
#include "sql.h"
#include <sstream>

using namespace std;
using namespace SV_Cng;
using namespace SV_Trigger;

void statusMess(QString);

bool sql::Init(){

		sqlite3_shutdown();
		sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
		sqlite3_initialize();

		if (!Open()){ return false; }

		string req = "PRAGMA journal_mode = WAL;";
	    vector<vector<string>> results;
		if (!Query(req, results)) return false;

		req = "PRAGMA synchronous = NORMAL;";
		if (!Query(req, results)) return false;
				
		sqlite3_busy_timeout(db_, 5000); 

		return true;
	}

bool sql::Open(){
	return (sqlite3_open(qPrintable(pathDB_), &db_) == SQLITE_OK);
}
	
bool sql::Query(const string& query, vector<vector<string>>& results){
	
	mtx_.lock();

	bool res = true;
	try{
		sqlite3_stmt* statement;
		if (sqlite3_prepare_v2(db_, query.c_str(), -1, &statement, 0) == SQLITE_OK)
		{
			int cols = sqlite3_column_count(statement);
			int result = 0;
			while (true){

				result = sqlite3_step(statement);

				if (result == SQLITE_ROW){
					vector<string> values;
					for (int col = 0; col < cols; col++){
						char* rv = (char*)sqlite3_column_text(statement, col);
						if (rv) values.push_back(rv);
					}
					if (!values.empty()) results.push_back(values);
				}
				else break;
			}

			sqlite3_finalize(statement);

		}
		else res = false;

	}
	catch (std::exception ex){ statusMess(QString("SQL::Query Exception ") + ex.what()); res = false; }

	mtx_.unlock();

	return res;
}

void sql::Close(){
		
	sqlite3_close(db_);
}

sql::sql(QString filename, bool& err){
		
	pathDB_ = filename;

	if (!Init()) { err = true; return; }

	stringstream ss;

	vector<vector<string>> results;
	ss << "CREATE TABLE IF NOT EXISTS 'Signal' ("
		"'id' INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"'name' TEXT,"
		"'module' TEXT,"
		"'grp' TEXT,"
		"'comment' TEXT,"
		"'type' INTEGER);";
	
	if (!Query(ss.str(), results)){ err = true; return; }

	ss.str(""); ss.clear();
	ss << "CREATE TABLE IF NOT EXISTS 'Trigger' ("
		"'id' INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"'name' TEXT,"
		"'signal' TEXT,"
		"'module' TEXT,"
		"'isActive' INTEGER,"		
		"'condType' INTEGER,"
		"'condValue' INTEGER,"
		"'condTOut' INTEGER);";
		
	if (!Query(ss.str(), results)){ err = true; return; }

	ss.str(""); ss.clear();
	ss << "CREATE TABLE IF NOT EXISTS 'UserEventData' ("
		"'id' INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"'trigger' TEXT,"
		"'userProcPath' TEXT,"
		"'userProcArgs' TEXT);";

	if (!Query(ss.str(), results)){ err = true; return; }

	
	ss.str(""); ss.clear();
	ss << "CREATE TABLE IF NOT EXISTS 'Events' ("
		"'id' INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"'trigger' TEXT,"
		"'sendDateTime' DATETIME);";
	
	if (!Query(ss.str(), results)){ err = true; return; }


	err = false;
}

sql::~sql(){

	Close();

}

bool sql::saveTriggers(const QMap<QString, SV_Trigger::triggerData*>& trgData){

	stringstream ss;vector<vector<string>> res;
		
	for (auto& tr : trgData){
	
		ss << "INSERT INTO Trigger ('name','signal','module','isActive','condType','condValue','condTOut') VALUES(" <<
            "'" << tr->name.toUtf8().constData() << "',"
            "'" << tr->signal.toUtf8().constData() << "',"
            "'" << tr->module.toUtf8().constData() << "',"
			"'" << (tr->isActive ? "1" : "0") << "',"
			"'" << (int)tr->condType << "',"
			"'" << tr->condValue << "',"
			"'" << tr->condTOut << "');";
		
		if (!Query(ss.str(), res)) return false;
		ss.str(""); ss.clear();

        /////////////////////////////////////////////////

        ss << "INSERT INTO UserEventData ('trigger','userProcPath','userProcArgs') VALUES(" <<
            "'" << tr->name.toUtf8().constData() << "',"
            "'" << tr->userProcPath.toUtf8().constData() << "',"
            "'" << tr->userProcArgs.toUtf8().constData() << "');";

        if (!Query(ss.str(), res)) return false;
        ss.str(""); ss.clear();
	}
	
	return true;
}

QVector<triggerData*> sql::getTrigger(const QString& signal, const QString& module){

	stringstream ss;

	ss << "SELECT * FROM Trigger WHERE signal = '" << signal.toUtf8().data() <<
		"' AND module = '" << module.toUtf8().data() << "'; ";

	vector<vector<string>> trg; QVector<triggerData*> res;
	if (!Query(ss.str(), trg)) return res;
        
	int sz = trg.size();
	for (int i = 0; i < sz; ++i){

		triggerData* td = new triggerData();

		td->name = trg[i][1].c_str();
		td->signal = trg[i][2].c_str();
		td->module = trg[i][3].c_str();
		td->isActive = trg[i][4] == "1";
		td->condType = (eventType)atoi(trg[i][5].c_str());
		td->condValue = atoi(trg[i][6].c_str());
		td->condTOut = atoi(trg[i][7].c_str());

        ss.str(""); ss.clear();
        ss << "SELECT * FROM UserEventData WHERE trigger = '" << td->name.toUtf8().data() << "';";

        vector<vector<string>> evData;
        if (!Query(ss.str(), evData)) return res;

        td->userProcPath = evData[i][1].c_str();
        td->userProcArgs = evData[i][2].c_str();

		res.push_back(td);
	}

	return res;
}

triggerData* sql::getTrigger(const QString& trname){

	stringstream ss;

	ss << "SELECT * FROM Trigger WHERE name = '" << trname.toUtf8().data() << "';";

	vector<vector<string>> trg; triggerData* td = nullptr;
	if (!Query(ss.str(), trg)) return td;

	if (!trg.empty()){

		td = new triggerData();

		td->name = trg[0][1].c_str();
        td->signal = trg[0][2].c_str();
        td->module = trg[0][3].c_str();
		td->isActive = trg[0][4] == "1";
		td->condType = (eventType)atoi(trg[0][5].c_str());
		td->condValue = atoi(trg[0][6].c_str());
		td->condTOut = atoi(trg[0][7].c_str());

        ss.str(""); ss.clear();
        ss << "SELECT * FROM UserEventData WHERE trigger = '" << td->name.toUtf8().data() << "';";

        vector<vector<string>> evData;
        if (!Query(ss.str(), evData)) return td;

        td->userProcPath = evData[0][2].c_str();
        td->userProcArgs = evData[0][3].c_str();
	}

	return td;
}

bool sql::saveSignals(const std::map<std::string, signalData*>& signs){

	stringstream ss; vector<vector<string>> res;

	for (auto& s : signs){

		ss << "SELECT * FROM Signal WHERE name = '" << s.second->name << "' AND module = '" << s.second->module << "';";

		res.clear();
		if (!Query(ss.str(), res)) return false;
        ss.str(""); ss.clear();
		
		if (res.empty()){
			ss << "INSERT INTO Signal ('name','module','grp','comment','type') VALUES(" <<
				"'" << s.second->name << "',"
				"'" << s.second->module << "',"
				"'" << s.second->group << "',"
				"'" << s.second->comment << "',"
				"'" << (int)s.second->type << "');";
		}
		else{
			ss << "UPDATE Signal SET "
				"grp = '" << s.second->group << "',"
				"comment = '" << s.second->comment << "' "
				"WHERE name = '" << s.second->name << "' AND module = '" << s.second->module << "';";
		}
		if (!Query(ss.str(), res)) return false;
		ss.str(""); ss.clear();
	}

	return true;
}

signalData sql::getSignal(const QString& signal, const QString& module){

	stringstream ss;

	ss << "SELECT * FROM Signal WHERE name = '" << signal.toUtf8().data() << "' AND module = '" << module.toUtf8().data() << "';";

	vector<vector<string>> signs; signalData sd;
	if (!Query(ss.str(), signs)) return sd;

	if (!signs.empty()){

		sd.name = signs[0][1].c_str();
		sd.module = signs[0][2].c_str();
		sd.group = signs[0][3].c_str();
		sd.comment = signs[0][4].c_str();
		sd.type = (valueType)atoi(signs[0][5].c_str());
	}

	return sd;

}

void sql::saveEvent(QString trg, QDateTime dt){

	stringstream ss;
		
	ss << "INSERT INTO Events ('trigger', 'sendDateTime') VALUES("
		"'" << trg.toUtf8().constData() << "',"
		"'" << qPrintable(dt.toString("yyyy-MM-dd HH:mm:ss")) << "');";

	vector<vector<string>> ret;
	Query(ss.str(), ret);
}

QVector<uEvent> sql::getEvents(QDateTime beginDT, QDateTime endDT){

	QString cmd = "SELECT * FROM Events WHERE sendDateTime BETWEEN '" + beginDT.toString("yyyy-MM-dd HH:mm:ss") + "' AND '" + endDT.toString("yyyy-MM-dd HH:mm:ss") + "';";

	vector<vector<string>> evt; QVector<uEvent> res;
	if (!Query(cmd.toStdString(), evt)) return res;

	int sz = evt.size(); res.reserve(sz); QSet<QString> trgs;
	for (int i = 0; i < sz; ++i){

		uEvent ed;

		ed.triggName = evt[i][1].c_str();
		ed.sendDateTime = evt[i][2].c_str();

		if (!trgs.contains(ed.triggName)) trgs.insert(ed.triggName);
		res.push_back(ed);
	}

	for (auto& t : trgs){

		QString cmd = "SELECT * FROM Trigger WHERE name = '" + t + "';";

		vector<vector<string>> tr;
		if (!Query(cmd.toStdString(), tr)) return res;
				
		if (!tr.empty()){
			for (auto& r : res){
				if (r.triggName == t){
					r.signal = tr[0][2].c_str();
					r.module = tr[0][3].c_str();
					r.condType = (SV_Trigger::eventType)stoi(tr[0][5]);
					r.condValue = stoi(tr[0][6]);
					r.condTOut = stoi(tr[0][7]);
					break;
				}
			}
		}
	}

	return res;
}

