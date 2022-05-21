cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

project(SVViewer)

FILE(GLOB_RECURSE SRC_View "*.cpp" "*.h" "*.ui" "*.qrc" ".rc")

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(UNIX)

add_executable(SVViewer ${SRC_View})

if(MSVC) 
   set_target_properties (SVViewer PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /NODEFAULTLIB:LIBCMT")
   target_link_libraries (SVViewer debug qtmaind)
   target_link_libraries (SVViewer optimized qtmain)
endif(MSVC)

target_link_libraries(SVViewer SVBase SVAuxFunc SVGraphPanel SVStatDialog SVExportDialog SVScriptDialog)

# copy mswin dependent
if(WIN32)
   target_link_libraries(SVViewer zlib)
else()
   target_link_libraries(SVViewer z)
endif(WIN32)

qt5_use_modules(SVViewer Core Widgets Gui PrintSupport)