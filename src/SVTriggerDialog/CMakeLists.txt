cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVTriggerDialog)

FILE(GLOB_RECURSE SRC_TrgPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVTriggerDialog STATIC ${SRC_TrgPn})

target_link_libraries(SVTriggerDialog SVBase SVAuxFunc)

qt5_use_modules(SVTriggerDialog Core Widgets Gui)
