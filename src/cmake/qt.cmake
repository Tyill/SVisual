set(QT_PATH_DIR "c:/QtVS64/Qt5.5.1/5.5/msvc2013_64/lib/cmake" CACHE STRING "Select path for Qt")
set(CMAKE_PREFIX_PATH ${QT_PATH_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package( Qt5Core REQUIRED)
find_package( Qt5Widgets REQUIRED)
find_package( Qt5Gui REQUIRED )