cmake_minimum_required(VERSION 3.0.2)

PROJECT("SVisual")
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Builds/$<CONFIG>/${CMAKE_VS_PLATFORM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Builds/$<CONFIG>/${CMAKE_VS_PLATFORM_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Builds/$<CONFIG>/${CMAKE_VS_PLATFORM_NAME})

option(USE_QtXlsxWriter   "QtXlsxWriter library support"  OFF)

if(MSVC)   
   add_definitions(-DWIN32_LEAN_AND_MEAN)
   add_definitions(-D_SCL_SECURE_NO_WARNINGS)
   add_definitions(-D_CRT_SECURE_NO_WARNINGS)
   link_directories(";$(OutDir)")
endif(MSVC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/)

add_subdirectory(SVAuxFunc)
add_subdirectory(SVConfig)
add_subdirectory(SVClient)
add_subdirectory(SVServer)
add_subdirectory(SVWebServer)
add_subdirectory(SVWriterExample)
add_subdirectory(SVDocker)
add_subdirectory(SVZabbix)
add_subdirectory(SVClientExample)

# gui
add_subdirectory(SVGraphPanel)
add_subdirectory(SVStatPanel)
add_subdirectory(SVTriggerPanel)
add_subdirectory(SVExportPanel)
add_subdirectory(SVScriptPanel)
add_subdirectory(SVMonitor)
add_subdirectory(SVViewer)

