cmake_minimum_required(VERSION 3.0.2)

project(SVServer)

if(WIN32)
    add_definitions(-DSVSRVDLL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_Serv "*.cpp" "*.h")

add_library(SVServer SHARED ${SRC_Serv})

if(MSVC) 
   set_target_properties (SVServer PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMT") 
endif()

target_link_libraries(SVServer SVConfig SVAuxFunc)

if(WIN32)
   target_link_libraries(SVServer zlibwapi)
else()
   target_link_libraries(SVServer z)
endif(WIN32)