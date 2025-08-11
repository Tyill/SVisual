cmake_minimum_required(VERSION 3.0.2)

project(SVServer)

if(WIN32)
  add_definitions(-DSVSERVERDLL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_Serv "*.cpp" "*.h")

add_library(SVServer SHARED ${SRC_Serv})

target_link_libraries(SVServer SVBase SVMisc)

if(WIN32)
   target_link_libraries(SVServer zlib)
else()
   target_link_libraries(SVServer z)
endif(WIN32)

if (USE_ClickHouseDB)
   add_definitions(-DUSE_ClickHouseDB)
   if(WIN32)
      target_link_libraries(SVServer clickhouse-cpp-lib-static lz4-lib cityhash-lib absl-lib)
   else()
      target_link_libraries(SVServer clickhouse-cpp-lib)
   endif(WIN32)
endif()