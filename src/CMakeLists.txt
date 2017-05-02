add_subdirectory(config)
add_subdirectory(windows)

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
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::SerialPort
        Qt5::OpenGL)

set_target_properties(qfm1000
        PROPERTIES
        MACOSX_BUNDLE_ICON_FILE
        ${QFM1000_MAC_ICON_NAME})
