cmake_minimum_required(VERSION 3.0)
project(qfm1000)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT5_PREFIX "$ENV{QTDIR}" CACHE PATH "Qt5 prefix path")

set(CMAKE_PREFIX_PATH "${QT5_PREFIX}/lib/cmake")

enable_testing()

add_subdirectory(src)
