cmake_minimum_required(VERSION 3.0.2)

project(SVBase)

FILE(GLOB_RECURSE SRC_Base "*.cpp" "*.h")

if (UNIX)
    add_compile_options(-fPIC)
endif(UNIX)

add_library(SVBase STATIC ${SRC_Base})

