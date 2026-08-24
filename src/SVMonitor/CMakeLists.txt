cmake_minimum_required(VERSION 3.0.2)

project(SVMonitor)

find_package(Qt5 COMPONENTS Core Widgets Gui Network SerialPort PrintSupport REQUIRED)

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

set(WEB_UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SVWebServer/website/web)
if(EXISTS ${WEB_UI_DIR}/index.html)
  add_custom_command(TARGET SVMonitor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:SVMonitor>/web
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${WEB_UI_DIR}
      $<TARGET_FILE_DIR:SVMonitor>/web
    COMMENT "Deploying web UI to $<TARGET_FILE_DIR:SVMonitor>/web"
  )
endif()
