cmake_minimum_required(VERSION 3.0.2)

include(../cmake/qt.cmake)

find_package(Qt5 COMPONENTS Core Widgets Gui Network SerialPort PrintSupport REQUIRED)

project(SVMonitor)

FILE(GLOB_RECURSE SRC_Mon "*.cpp" "*.h" "*.ui" "*.qrc" ".rc")

if (UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(UNIX)

add_executable(SVMonitor ${SRC_Mon})

if(MSVC) 
  set_target_properties (SVMonitor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /NODEFAULTLIB:LIBCMT")
  target_link_libraries (SVMonitor debug qtmaind)
  target_link_libraries (SVMonitor optimized qtmain)
endif(MSVC)

target_link_libraries(SVMonitor sqlite3 SVBase SVMisc SVGraphPanel SVTriggerDialog
                      SVExportDialog SVScriptDialog SVServer SVWebServer SVZabbix)

target_link_libraries(SVMonitor Qt5::Core Qt5::Widgets Qt5::Network Qt5::Gui Qt5::SerialPort Qt5::PrintSupport)
