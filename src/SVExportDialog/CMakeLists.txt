cmake_minimum_required(VERSION 3.0.2)

project(SVExportDialog)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

FILE(GLOB_RECURSE SRC_ExPn "*.cpp" "*.h" "*.ui" "*.qrc")

add_library(SVExportDialog STATIC ${SRC_ExPn})

if(USE_QtXlsxWriter)
   add_definitions(-DUSE_QtXlsxWriter)
   target_link_libraries(SVExportDialog QtXlsxWriter)
endif()

target_link_libraries(SVExportDialog SVBase)

target_link_libraries(SVExportDialog Qt5::Core Qt5::Widgets Qt5::Gui )
