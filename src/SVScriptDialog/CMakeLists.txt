cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

project(SVScriptDialog)

FILE(GLOB_RECURSE SRC_ScrPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVScriptDialog STATIC ${SRC_ScrPn})

target_link_libraries(SVScriptDialog SVBase SVMisc lua53)

target_link_libraries(SVScriptDialog Qt5::Core Qt5::Widgets Qt5::Gui )
