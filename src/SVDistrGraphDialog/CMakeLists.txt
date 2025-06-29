cmake_minimum_required(VERSION 3.0.2)

project(SVDistributionGraphDialog)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

FILE(GLOB_RECURSE SRC_GrWdg "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVDistributionGraphDialog STATIC ${SRC_GrWdg})

target_link_libraries(SVDistributionGraphDialog SVBase)

target_link_libraries(SVDistributionGraphDialog Qt5::Core Qt5::Widgets Qt5::Gui )
