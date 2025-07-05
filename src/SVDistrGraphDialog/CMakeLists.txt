cmake_minimum_required(VERSION 3.0.2)

project(SVDistrGraphDialog)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

FILE(GLOB_RECURSE SRC_GrWdg "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVDistrGraphDialog STATIC ${SRC_GrWdg})

target_link_libraries(SVDistrGraphDialog SVBase)

target_link_libraries(SVDistrGraphDialog Qt5::Core Qt5::Widgets Qt5::Gui )
