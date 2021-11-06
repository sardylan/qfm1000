cmake_minimum_required(VERSION 3.18)
project(qfm1000)

if (UNIX)
    include(GNUInstallDirs)
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_policy(SET CMP0115 NEW)

include(CTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_PREFIX "$ENV{QTDIR}" CACHE PATH "Qt prefix path")

find_program(QT_QMAKE_BIN
        qmake
        PATHS "${QT_PREFIX}/bin")

if (NOT QT_QMAKE_BIN)
    message(FATAL_ERROR "qmake not found. set QT_PREFIX.")
endif ()

set(CMAKE_PREFIX_PATH "${QT_PREFIX}/lib/cmake")

if (WIN32 OR APPLE)
    if (WIN32)
        find_program(QT_DEPLOYQT_BIN
                windeployqt
                PATHS "${QT_PREFIX}/bin")
    elseif (APPLE)
        find_program(QT_DEPLOYQT_BIN
                macdeployqt
                PATHS "${QT_PREFIX}/bin")
    endif ()

    if (NOT QT_DEPLOYQT_BIN)
        message(FATAL_ERROR "No Qt Deploy command found.")
    endif ()
endif ()

set(APPLICATION_NAME "qFM1000")
set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "7")
set(APPLICATION_VERSION_PATCH "100")

set(ORGANIZATION_NAME "The HellNet.org")
set(ORGANIZATION_DOMAIN "thehellnet.org")

set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

enable_testing()

add_subdirectory(src)

#add_subdirectory(desktop)

#include(cmake/packaging.cmake)
