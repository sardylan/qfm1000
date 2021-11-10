cmake_minimum_required(VERSION 3.16.3)
project(qfm1000)

if (UNIX)
    include(GNUInstallDirs)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.20.0")
    cmake_policy(SET CMP0115 NEW)
endif ()

include(CTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(cmake/qt.cmake)

if (WIN32 OR APPLE)
    if (WIN32)
        find_program(QT_DEPLOYQT_BIN
                windeployqt
                PATHS ENV Qt6_DIR ENV Qt5_DIR)
    elseif (APPLE)
        find_program(QT_DEPLOYQT_BIN
                macdeployqt
                PATHS ENV Qt6_DIR ENV Qt5_DIR)
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

include(cmake/flags.cmake)

add_subdirectory(src)
add_subdirectory(desktop)

include(cmake/packaging.cmake)
