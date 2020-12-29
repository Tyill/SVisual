cmake_minimum_required(VERSION 3.0.2)

project(SVServer)

FILE(GLOB_RECURSE SRC_Serv "*.cpp" "*.h")

add_library(SVServer STATIC ${SRC_Serv})

if(MSVC) 
   set_target_properties (SVServer PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMT") 
endif()

target_link_libraries(SVServer SVConfig SVAuxFunc)

if(WIN32)
   target_link_libraries(SVServer zlibwapi)
else()
   target_link_libraries(SVServer z)
endif(WIN32)