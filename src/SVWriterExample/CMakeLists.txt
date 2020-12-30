cmake_minimum_required(VERSION 3.0.2)

project(SVWriterExample)

FILE(GLOB_RECURSE SRC_SVWr "*.cpp" "*.h")

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(UNIX)

add_executable(SVWriterExample ${SRC_SVWr})

if(MSVC) 
   set_target_properties (SVWriterExample PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMT") 
endif(MSVC)

target_link_libraries(SVWriterExample SVAuxFunc SVServer)