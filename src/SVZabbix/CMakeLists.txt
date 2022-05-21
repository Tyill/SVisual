cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVZabbix)

FILE(GLOB_RECURSE SRC_Zbx "*.cpp" "*.c" "*.h" "*.ui" "*.qrc")

add_library(SVZabbix STATIC ${SRC_Zbx})

target_link_libraries(SVZabbix SVBase SVAuxFunc)

qt5_use_modules(SVZabbix Core Network)