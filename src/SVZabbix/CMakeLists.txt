cmake_minimum_required(VERSION 3.0.2)

project(SVZabbix)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

FILE(GLOB_RECURSE SRC_Zbx "*.cpp" "*.c" "*.h" "*.ui" "*.qrc")

add_library(SVZabbix STATIC ${SRC_Zbx})

target_link_libraries(SVZabbix SVBase SVMisc Qt5::Core Qt5::Network)