cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVWebServer)

if(WIN32)
    add_definitions(-DSVWEBSERVER_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_Web"*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVWebServer SHARED ${SRC_Web})

target_link_libraries(SVWebServer SVConfig SVAuxFunc)

qt5_use_modules(SVWebServer Core Network)
