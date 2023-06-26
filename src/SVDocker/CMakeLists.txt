cmake_minimum_required(VERSION 3.0.2)

project(SVDocker)

find_package(Qt5 COMPONENTS Network REQUIRED)

FILE(GLOB_RECURSE SRC_SVDock "*.cpp" "*.h")

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(UNIX)

add_executable(SVDocker ${SRC_SVDock})

if(MSVC) 
   set_target_properties (SVDocker PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMT") 
endif(MSVC)

target_link_libraries(SVDocker SVBase SVMisc SVServer SVWebServer)

target_link_libraries(SVDocker Qt5::Core)