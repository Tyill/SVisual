cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVExportDialog)

FILE(GLOB_RECURSE SRC_ExPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVExportDialog STATIC ${SRC_ExPn})

if(USE_QtXlsxWriter)
   add_definitions(-DUSE_QtXlsxWriter)
   target_link_libraries(SVExportDialog QtXlsxWriter)
endif()

target_link_libraries(SVExportDialog SVBase)

qt5_use_modules(SVExportDialog Core Widgets Gui )
