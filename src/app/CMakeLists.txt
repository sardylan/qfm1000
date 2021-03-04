find_package(Qt5 REQUIRED COMPONENTS
        Core Gui Svg Widgets SerialPort OpenGL)

configure_file(version.hpp.in version.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set_source_files_properties(../../icons/macos/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

set(MACOSX_BUNDLE_EXECUTABLE_NAME qfm1000)
set(MACOSX_BUNDLE_BUNDLE_NAME qFM1000)
set(MACOSX_BUNDLE_ICON_FILE icon.icns)

add_executable(qfm1000
        WIN32 MACOSX_BUNDLE
        windows/main.cpp windows/main.hpp windows/main.ui
        windows/about.cpp windows/about.hpp windows/about.ui
        windows/license.cpp windows/license.hpp windows/license.ui
        windows/config.cpp windows/config.hpp windows/config.ui
        windows/instance.cpp windows/instance.hpp windows/instance.ui
        instance.cpp instance.hpp
        status.cpp status.hpp
        config.cpp config.hpp
        qfm1000.cpp qfm1000.hpp
        main.cpp main.hpp
        default.hpp
        version.hpp
        qfm1000.rc
        qfm1000.qrc
        ../../icons/macos/icon.icns)

target_link_libraries(qfm1000
        utilities
        eeprom
        inoprog
        widgets
        delegators
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::SerialPort
        Qt5::OpenGL)

if (WIN32)
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QFM1000_DEPLOYQT_TYPE "--debug")
    else ()
        set(QFM1000_DEPLOYQT_TYPE "--release")
    endif ()

    add_custom_command(TARGET qfm1000
            POST_BUILD
            COMMAND ${QT5_DEPLOYQT_BIN} $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR}/windeployqt)

    #install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
    #        DESTINATION ${QFM1000_TARGET_INSTALL_DIR}
    #        COMPONENT qtlibs)
elseif (APPLE)
    set_target_properties(qfm1000
            PROPERTIES
            MACOSX_BUNDLE_ICON_FILE ${QFM1000_MAC_ICON_NAME})
    add_custom_command(TARGET qfm1000
            POST_BUILD
            COMMAND ${QT5_DEPLOYQT_BIN} $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE})
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
