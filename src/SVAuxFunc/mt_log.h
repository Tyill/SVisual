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

#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "auxFunc.h"

// Лог многопоточный

/* Пользоваться так:
 -создать объект: 'auto log = new Logger(путь, имя);'
 -записать сообщ: 'log.WriteLine(mess);'
 
 -освободить ресурсы в конце destroy()
*/

namespace SV_Aux {
	class Logger {

	public:

		// -nameFile - имя файла лога
		// -path - путь
		Logger(const std::string &nameFile, const std::string &pathFile) {

			nameFile_ = nameFile;
			pathSave_ = pathFile;

			Init();
		}

		Logger() {
			nameFile_ = "";
			pathSave_ = "";

			Init();
		}

		~Logger() {
			destroy();
		}

		//
		void destroy() {

			fStop_ = true;
			cval_.notify_one();
			if (thrWriteMess_.joinable()) thrWriteMess_.join();
		}

		void SetName(const std::string &nameFile) {
			nameFile_ = nameFile;
		}

		void SetPath(const std::string &pathFile) {
			pathSave_ = pathFile;
		}

		bool WriteLine(const std::string &mess) {

			mtxWr_.lock();

			deqMess_[writeMessCnt_] = Message(true, CurrDateTimeMs(), mess);
			++writeMessCnt_;
			if (writeMessCnt_ >= MAX_CNT_MESS) writeMessCnt_ = 0;

			mtxWr_.unlock();

			cval_.notify_one();

			return true;
		}

	private:

		const int MAX_CNT_MESS = 100;

		std::string nameFile_ = " ", pathSave_;

		int readMessCnt_, writeMessCnt_;

		struct Message {
			bool activ;
			std::string cTime;
			std::string mess;

			Message():activ(false){}
			Message(bool activ_, const std::string& cTime_, const std::string& mess_):
					activ(activ_), cTime(cTime_), mess(mess_){}
		};
		std::vector<Message> deqMess_;

		std::mutex mtxWr_, mtxRd_;
		std::thread thrWriteMess_;
		std::condition_variable cval_;
		bool fStop_ = false;


		void WriteCyc() {

			while (!fStop_) {

				std::unique_lock<std::mutex> lck(mtxRd_);
				cval_.wait(lck);

				CreateSubDirectory(pathSave_);

				std::string logPath = pathSave_ + nameFile_ + ".log";

				std::ofstream slg(logPath.c_str(), std::ios::app);

				while (deqMess_[readMessCnt_].activ) {

					slg << "[" << deqMess_[readMessCnt_].cTime << "] " << deqMess_[readMessCnt_].mess << std::endl;

					deqMess_[readMessCnt_].activ = false;
					readMessCnt_++;
					if (readMessCnt_ >= MAX_CNT_MESS) readMessCnt_ = 0;
				}
				slg.close();
			}
		}

		void Init() {

			deqMess_.resize(MAX_CNT_MESS);

			readMessCnt_ = 0;
			writeMessCnt_ = 0;
            			
			thrWriteMess_ = std::thread([](Logger *lg) { lg->WriteCyc(); }, this);
		}
	};
}