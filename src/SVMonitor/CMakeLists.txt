cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)
include(../cmake/mswin.cmake)

project(SVMonitor)

FILE(GLOB_RECURSE SRC_Mon "*.cpp" "*.h" "*.ui" "*.qrc" ".rc")

if (UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(UNIX)

add_executable(SVMonitor ${SRC_Mon} src/structurs.h)

if(MSVC) 
   set_target_properties (SVMonitor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /NODEFAULTLIB:LIBCMT")
   target_link_libraries (SVMonitor debug qtmaind)
   target_link_libraries (SVMonitor optimized qtmain)
endif(MSVC)

target_link_libraries (SVMonitor sqlite3)

# copy mswin dependent
if(WIN32)
  
   mswin_getDependDir(DEPEND_WIN_DIR)
   
   add_custom_command(TARGET SVMonitor PRE_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_directory ${DEPEND_WIN_DIR}/plugins ${CMAKE_CURRENT_BINARY_DIR}/plugins)

endif()

target_link_libraries(SVMonitor SVConfig SVAuxFunc SVGraphPanel SVExportPanel SVScriptPanel SVServer)

qt5_use_modules(SVMonitor Core Widgets Gui SerialPort PrintSupport)
