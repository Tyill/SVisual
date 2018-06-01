cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVGraphPanel)

if(WIN32)
    add_definitions(-DSVGRAPHPANEL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_GrPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVGraphPanel SHARED ${SRC_GrPn})

target_link_libraries(SVGraphPanel SVConfig)

qt5_use_modules(SVGraphPanel Core Widgets Gui )
