cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

project(SVStatDialog)

FILE(GLOB_RECURSE SRC_StPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVStatDialog STATIC ${SRC_StPn})

target_link_libraries(SVStatDialog SVBase)

target_link_libraries(SVStatDialog Qt5::Core Qt5::Widgets Qt5::Gui )
