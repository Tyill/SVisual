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
#include <string>
#include <iostream>
#include "SVClient/SVClient.h"
#include "SVAuxFunc/auxFunc.h"

int main(int argc, char *argv[]){

	std::string nm = argc >= 2 ? argv[1] : "client";

	int cyc = argc >= 3 ? atoi(argv[2]) : 100;
	int packSz = argc >= 4 ? atoi(argv[3]) : 10;

	SV_Cln::svSetParam(cyc, packSz);

	if (SV_Cln::svConnect(nm.c_str(), "127.0.0.1", 2144)){

		std::cout << "connect " << nm.c_str() << " ok" << std::endl;

		for (int i = 0; i < 100; ++i){

			std::string val = "абрвал" + std::to_string(i);
			SV_Cln::svAddIntValue(val.c_str(), 0);

			val = "front" + std::to_string(i);
			SV_Cln::svAddBoolValue(val.c_str(), true);
		}
	}
	else std::cout << "connect no" << std::endl;

	int cp = 0;
	while (true){

		for (int i = 0; i < 100; i++){
				
			std::string val = "dde" + std::to_string(i);
			SV_Cln::svAddIntValue(val.c_str(), cp);

			val = "front" + std::to_string(i);
			SV_Cln::svAddBoolValue(val.c_str(), cp%2);
		}
		
		cp += 1; if (cp > 100) cp = -100;
			
		SV_Aux::SleepMs(cyc);
	}
	return 0;
}