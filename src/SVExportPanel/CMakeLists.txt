cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVExportPanel)

if(WIN32)
    add_definitions(-DSVEXPPANEL_EXPORTS)
endif(WIN32)

FILE(GLOB_RECURSE SRC_ExPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVExportPanel SHARED ${SRC_ExPn})

target_link_libraries(SVExportPanel SVConfig QtXlsxWriter)

qt5_use_modules(SVExportPanel Core Widgets Gui )
