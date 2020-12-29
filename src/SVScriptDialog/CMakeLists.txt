cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVScriptDialog)

FILE(GLOB_RECURSE SRC_ScrPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVScriptDialog STATIC ${SRC_ScrPn})

target_link_libraries(SVScriptDialog SVConfig SVAuxFunc lua53)

qt5_use_modules(SVScriptDialog Core Widgets Gui )
