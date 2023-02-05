cmake_minimum_required(VERSION 3.1)
project(qfm1000)

if (UNIX)
    include(GNUInstallDirs)
endif ()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT5_PREFIX "$ENV{Qt5_Dir}" CACHE PATH "Qt5 prefix path")

find_program(QT5_QMAKE_BIN
        qmake
        PATHS "${QT5_PREFIX}/bin"
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH)

if (NOT QT5_QMAKE_BIN)
    message(FATAL_ERROR "Wrong Qt5 directory: ${QT5_PREFIX}")
endif ()

set(CMAKE_PREFIX_PATH "${QT5_PREFIX}/lib/cmake")

if (WIN32 OR APPLE)
    if (WIN32)
        find_program(QT5_DEPLOYQT_BIN
                windeployqt
                PATHS "${QT5_PREFIX}/bin"
                NO_CMAKE_PATH
                NO_CMAKE_ENVIRONMENT_PATH
                NO_SYSTEM_ENVIRONMENT_PATH
                NO_CMAKE_SYSTEM_PATH)
    elseif (APPLE)
        find_program(QT5_DEPLOYQT_BIN
                macdeployqt
                PATHS "${QT5_PREFIX}/bin"
                NO_CMAKE_PATH
                NO_CMAKE_ENVIRONMENT_PATH
                NO_SYSTEM_ENVIRONMENT_PATH
                NO_CMAKE_SYSTEM_PATH)
    endif ()

    if (NOT QT5_DEPLOYQT_BIN)
        message(FATAL_ERROR "No Qt Deploy command found in : ${QT5_PREFIX}/bin")
    endif ()
endif ()

set(QT_TEST_OUTPUT_FORMAT "xunitxml")
message(STATUS "Qt tests output format: ${QT_TEST_OUTPUT_FORMAT}")

set(QFM1000_APPLICATION_NAME "qFM1000")
set(QFM1000_APPLICATION_VERSION_MAJOR "0")
set(QFM1000_APPLICATION_VERSION_MINOR "7")
set(QFM1000_APPLICATION_VERSION_PATCH "2")

set(QFM1000_ORGANIZATION_NAME "The HellNet.org")
set(QFM1000_ORGANIZATION_DOMAIN "thehellnet.org")

set(QFM1000_APPLICATION_VERSION "${QFM1000_APPLICATION_VERSION_MAJOR}.${QFM1000_APPLICATION_VERSION_MINOR}.${QFM1000_APPLICATION_VERSION_PATCH}")

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

add_subdirectory(desktop)

include(cmake/packaging.cmake)
