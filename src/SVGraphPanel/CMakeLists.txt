cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVGraphPanel)

FILE(GLOB_RECURSE SRC_GrWdg "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVGraphPanel STATIC ${SRC_GrWdg})

target_link_libraries(SVGraphPanel SVConfig)

qt5_use_modules(SVGraphPanel Core Widgets Gui )
