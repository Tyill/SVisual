cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVStatPanel)

if(WIN32)
    add_definitions(-DSVSTATPANEL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_StPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVStatPanel SHARED ${SRC_StPn})

target_link_libraries(SVStatPanel SVConfig)

qt5_use_modules(SVStatPanel Core Widgets Gui )
