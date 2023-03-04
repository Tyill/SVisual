cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

project(SVZabbix)

FILE(GLOB_RECURSE SRC_Zbx "*.cpp" "*.c" "*.h" "*.ui" "*.qrc")

add_library(SVZabbix STATIC ${SRC_Zbx})

target_link_libraries(SVZabbix SVBase SVMisc)

target_link_libraries(SVZabbix Qt5::Core Qt5::Network)