cmake_minimum_required(VERSION 3.0.2)

project(SVClient)

if(WIN32)
    add_definitions(-DSVCLDLL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_Cl "*.cpp" "*.h")

add_library(SVClient SHARED ${SRC_Cl})

target_link_libraries(SVClient SVConfig)
target_link_libraries(SVClient SVAuxFunc)
