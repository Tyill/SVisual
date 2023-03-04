cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

project(SVWebServer)

FILE(GLOB_RECURSE SRC_Web "*.cpp" "*.c" "*.h" "*.ui" "*.qrc")

add_library(SVWebServer STATIC ${SRC_Web})

target_link_libraries(SVWebServer SVBase SVMisc)

target_link_libraries(SVWebServer Qt5::Core Qt5::Network)
