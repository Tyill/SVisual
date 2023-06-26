cmake_minimum_required(VERSION 3.0.2)

project(SVServer)

if(WIN32)
  add_definitions(-DSVSERVERDLL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_Serv "*.cpp" "*.h")

add_library(SVServer SHARED ${SRC_Serv})

target_link_libraries(SVServer SVBase SVMisc)

if(WIN32)
   target_link_libraries(SVServer zlib clickhouse-cpp-lib-static lz4-lib cityhash-lib absl-lib)
else()
   target_link_libraries(SVServer z clickhouse-cpp-lib)
endif(WIN32)