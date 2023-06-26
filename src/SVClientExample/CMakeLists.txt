cmake_minimum_required(VERSION 3.0.2)

project(SVClientExample)

FILE(GLOB_RECURSE SRC_ClEx "*.cpp" "*.h")

add_executable(SVClientExample ${SRC_ClEx})

target_link_libraries(SVClientExample SVClient SVMisc)