cmake_minimum_required(VERSION 3.0.2)

project(SVServer)

FILE(GLOB_RECURSE SRC_Serv "*.cpp" "*.h")

add_library(SVServer STATIC ${SRC_Serv})

target_link_libraries(SVServer SVBase SVAuxFunc)

if(WIN32)
   target_link_libraries(SVServer zlib)
else()
   target_link_libraries(SVServer z)
endif(WIN32)