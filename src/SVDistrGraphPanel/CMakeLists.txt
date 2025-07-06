cmake_minimum_required(VERSION 3.0.2)

project(SVDistrGraphPanel)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

FILE(GLOB_RECURSE SRC_GrWdg "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVDistrGraphPanel STATIC ${SRC_GrWdg})

target_link_libraries(SVDistrGraphPanel SVBase)

target_link_libraries(SVDistrGraphPanel Qt5::Core Qt5::Widgets Qt5::Gui )
