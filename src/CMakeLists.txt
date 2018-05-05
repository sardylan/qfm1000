add_subdirectory(config)
add_subdirectory(windows)
add_subdirectory(eeprom)
add_subdirectory(programmer)

set(QFM1000_SOURCES
        qfm1000.cpp)

set(QFM1000_HEADERS
        version.hpp
        qfm1000.hpp)

set(QFM1000_RESOURCES
        qfm1000.rc
        resources.qrc)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Svg
        Widgets
        SerialPort
        OpenGL)

configure_file(version.hpp.in version.hpp @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(QFM1000_MAC_ICON_NAME icon.icns)
set(QFM1000_MAC_ICON_PATH ../icons/macos/icon.icns)

set_source_files_properties(${QFM1000_MAC_ICON_PATH}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION
        Resources)

set(MACOSX_BUNDLE_EXECUTABLE_NAME qfm1000)
set(MACOSX_BUNDLE_BUNDLE_NAME qFM1000)
set(MACOSX_BUNDLE_ICON_FILE ${QFM1000_MAC_ICON_NAME})

add_executable(qfm1000
        WIN32 MACOSX_BUNDLE
        ${QFM1000_SOURCES}
        ${QFM1000_HEADERS}
        ${QFM1000_RESOURCES}
        ${QFM1000_MAC_ICON_PATH})

target_link_libraries(qfm1000
        config
        windows
        eeprom
        programmer
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::SerialPort
        Qt5::OpenGL)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(QFM1000_DEPLOYQT_TYPE "--debug")
else ()
    set(QFM1000_DEPLOYQT_TYPE "--release")
endif ()

if (WIN32)
    add_custom_command(TARGET qfm1000
            POST_BUILD
            COMMAND windeployqt $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR}/windeployqt)
elseif (APPLE)
    set_target_properties(qfm1000
            PROPERTIES
            MACOSX_BUNDLE_ICON_FILE ${QFM1000_MAC_ICON_NAME})
    add_custom_command(TARGET qfm1000
            POST_BUILD
            COMMAND macdeployqt $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR}/windeployqt)
endif ()

if (UNIX)
    set(QFM1000_TARGET_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
else ()
    set(QFM1000_TARGET_INSTALL_DIR ".")
endif ()

install(TARGETS qfm1000
        RUNTIME DESTINATION ${QFM1000_TARGET_INSTALL_DIR}
        BUNDLE DESTINATION ${QFM1000_TARGET_INSTALL_DIR}
        COMPONENT qfm1000)

if (WIN32 OR APPLE)
    install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
            DESTINATION ${QFM1000_TARGET_INSTALL_DIR}
            COMPONENT qtlibs)
endif ()
