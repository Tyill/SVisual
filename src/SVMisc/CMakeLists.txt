cmake_minimum_required(VERSION 3.0.2)

project(SVMisc)

FILE(GLOB_RECURSE SRC_SVAux "*.cpp" "*.h")

if (UNIX)
   add_compile_options(-fPIC )
endif(UNIX)

add_library(SVMisc STATIC ${SRC_SVAux})

if(WIN32)
   target_link_libraries(SVMisc libuv ws2_32 Iphlpapi psapi Userenv)
else()
   target_link_libraries(SVMisc uv pthread)
endif()