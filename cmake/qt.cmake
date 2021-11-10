find_program(QT_QMAKE_BIN
        qmake
        PATHS ENV Qt6_DIR ENV Qt5_DIR
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH)

if (NOT QT_QMAKE_BIN)
    find_program(QT_QMAKE_BIN
            qmake)
endif ()

if (NOT QT_QMAKE_BIN)
    message(FATAL_ERROR "QMake binary not found")
endif ()

message(STATUS "QMake binary: ${QT_QMAKE_BIN}")

execute_process(COMMAND ${QT_QMAKE_BIN} --version
        OUTPUT_VARIABLE QT_QMAKE_OUT)

string(REGEX REPLACE "^.*\nUsing Qt version ([0-9.]+) in (.+)\n$" "\\1" QT_QMAKE_VERSION ${QT_QMAKE_OUT})
string(REGEX REPLACE "^.*\nUsing Qt version ([0-9.]+) in (.+)\n$" "\\2" QT_QMAKE_PREFIX ${QT_QMAKE_OUT})

if (DEFINED ENV{Qt6_DIR})
    set(QT_PREFIX $ENV{Qt6_DIR})
elseif (DEFINED ENV{Qt5_DIR})
    set(QT_PREFIX $ENV{Qt5_DIR})
else ()
    set(QT_PREFIX ${QT_QMAKE_PREFIX})
endif ()

set(QT_VERSION ${QT_QMAKE_VERSION})

message(STATUS "Qt version: ${QT_VERSION}")
message(STATUS "Qt prefix: ${QT_PREFIX}")

set(CMAKE_PREFIX_PATH "${QT_PREFIX}/lib/cmake")

if (${QT_VERSION} VERSION_LESS "5.15.0")
    set(QT_LIB_NAME "Qt5")
else ()
    set(QT_LIB_NAME "Qt")
endif ()

message(STATUS "Qt lib name: ${QT_LIB_NAME}")

find_package(Qt6 COMPONENTS Core Gui Svg Widgets SerialPort OpenGL Concurrent Test QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.12 COMPONENTS Core Gui Svg Widgets SerialPort OpenGL Concurrent Test REQUIRED)
endif ()
