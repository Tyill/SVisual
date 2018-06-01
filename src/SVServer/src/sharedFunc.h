
#pragma once


#define FUNC_BEGIN try {

#define FUNC_END } catch(std::exception& e) { \
statusMess(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " Exception: " + e.what()); return false; } return true;
