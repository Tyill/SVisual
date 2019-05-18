cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVTriggerPanel)

if(WIN32)
    add_definitions(-DSVSCRIPTPANEL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_TrgPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVTriggerPanel SHARED ${SRC_TrgPn})

target_link_libraries(SVTriggerPanel SVConfig SVAuxFunc)

qt5_use_modules(SVTriggerPanel Core Widgets Gui)
