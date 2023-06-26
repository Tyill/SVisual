cmake_minimum_required(VERSION 3.0.2)

project(SVGraphPanel)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

FILE(GLOB_RECURSE SRC_GrWdg "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVGraphPanel STATIC ${SRC_GrWdg})

target_link_libraries(SVGraphPanel SVBase)

target_link_libraries(SVGraphPanel Qt5::Core Qt5::Widgets Qt5::Gui )
