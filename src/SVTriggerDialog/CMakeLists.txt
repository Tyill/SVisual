cmake_minimum_required(VERSION 3.0.2)

project(SVTriggerDialog)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

FILE(GLOB_RECURSE SRC_TrgPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVTriggerDialog STATIC ${SRC_TrgPn})

target_link_libraries(SVTriggerDialog SVBase SVMisc)

target_link_libraries(SVTriggerDialog Qt5::Core Qt5::Widgets Qt5::Gui)
