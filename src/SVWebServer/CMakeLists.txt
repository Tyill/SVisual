cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVWebServer)

FILE(GLOB_RECURSE SRC_Web "*.cpp" "*.c" "*.h" "*.ui" "*.qrc")

add_library(SVWebServer STATIC ${SRC_Web})

target_link_libraries(SVWebServer SVConfig SVAuxFunc)

qt5_use_modules(SVWebServer Core Network)
