#set(QT_PATH_DIR "c:/Qt64/Qt5.5.1/5.5/msvc2013_64/lib/cmake" CACHE STRING "Select path for Qt")
set(QT_PATH_DIR "~/Qt/5.15.2/gcc_64/lib/cmake" CACHE STRING "Select path for Qt")

set(CMAKE_PREFIX_PATH ${QT_PATH_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)