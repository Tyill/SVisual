cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVScriptPanel)

if(WIN32)
    add_definitions(-DSVSCRIPTPANEL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_ScrPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVScriptPanel SHARED ${SRC_ScrPn})

target_link_libraries(SVScriptPanel SVConfig SVAuxFunc lua53)

qt5_use_modules(SVScriptPanel Core Widgets Gui )
