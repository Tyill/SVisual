cmake_minimum_required(VERSION 3.0.2)

project(SVConfig)

FILE(GLOB_RECURSE SRC_Cng "*.cpp" "*.h")

if (UNIX)
    add_compile_options(-fPIC)
endif(UNIX)

add_library(SVConfig STATIC ${SRC_Cng})

