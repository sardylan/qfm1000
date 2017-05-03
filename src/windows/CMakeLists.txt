set(QFM1000_WINDOWS_WIDGET_SOURCES
        widgets/statusbar.cpp)

set(QFM1000_WINDOWS_WIDGET_HEADERS
        widgets/statusbar.hpp)

set(QFM1000_WINDOWS_MODEL_SOURCES
        model/tablemodel.cpp)

set(QFM1000_WINDOWS_MODEL_HEADERS
        model/tablemodel.hpp)

set(QFM1000_WINDOWS_MAIN
        mainwindow.cpp
        mainwindow.hpp
        mainwindow.ui)

set(QFM1000_WINDOWS_CONFIG
        configwindow.cpp
        configwindow.hpp
        configwindow.ui)

set(QFM1000_WINDOWS_ABOUT
        aboutwindow.cpp
        aboutwindow.hpp
        aboutwindow.ui)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Widgets
        Gui
        SerialPort
        OpenGL)

add_library(windows
        ${QFM1000_WINDOWS_WIDGET_SOURCES}
        ${QFM1000_WINDOWS_WIDGET_HEADERS}
        ${QFM1000_WINDOWS_MODEL_SOURCES}
        ${QFM1000_WINDOWS_MODEL_HEADERS}
        ${QFM1000_WINDOWS_MAIN}
        ${QFM1000_WINDOWS_CONFIG}
        ${QFM1000_WINDOWS_ABOUT})

target_link_libraries(windows
        config
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::SerialPort
        Qt5::OpenGL)

target_include_directories(windows PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
