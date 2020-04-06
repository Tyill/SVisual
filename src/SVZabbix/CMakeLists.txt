cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVZabbix)

if(WIN32)
    add_definitions(-DSVZABBIX_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_Zbx "*.cpp" "*.c" "*.h" "*.ui" "*.qrc")

add_library(SVZabbix SHARED ${SRC_Zbx})

target_link_libraries(SVZabbix SVConfig SVAuxFunc)