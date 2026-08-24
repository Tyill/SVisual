cmake_minimum_required(VERSION 3.0.2)

project(SVDocker)

find_package(Qt5 COMPONENTS Network REQUIRED)

FILE(GLOB_RECURSE SRC_SVDock "*.cpp" "*.h")

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(UNIX)

add_executable(SVDocker ${SRC_SVDock})

if(MSVC) 
   set_target_properties (SVDocker PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMT") 
endif(MSVC)

target_link_libraries(SVDocker SVBase SVMisc SVServer SVWebServer)

target_link_libraries(SVDocker Qt5::Core)

set(WEB_UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SVWebServer/website/web)
if(EXISTS ${WEB_UI_DIR}/index.html)
  add_custom_command(TARGET SVDocker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE_DIR:SVDocker>/web
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${WEB_UI_DIR}
      $<TARGET_FILE_DIR:SVDocker>/web
    COMMENT "Deploying web UI to $<TARGET_FILE_DIR:SVDocker>/web"
  )
endif()