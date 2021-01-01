cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVStatDialog)

FILE(GLOB_RECURSE SRC_StPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVStatDialog STATIC ${SRC_StPn})

target_link_libraries(SVStatDialog SVConfig)

qt5_use_modules(SVStatDialog Core Widgets Gui )
