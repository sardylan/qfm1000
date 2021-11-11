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
        windows/inoprog.cpp windows/inoprog.hpp windows/inoprog.ui
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
        ${QT_LIB_NAME}::Core
        ${QT_LIB_NAME}::Gui
        ${QT_LIB_NAME}::Svg
        ${QT_LIB_NAME}::Widgets
        ${QT_LIB_NAME}::SerialPort
        ${QT_LIB_NAME}::OpenGL)

if (UNIX)
    set(QFM1000_TARGET_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
else ()
    set(QFM1000_TARGET_INSTALL_DIR ".")
endif ()

if (WIN32)
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QFM1000_DEPLOYQT_TYPE "--debug")
    else ()
        set(QFM1000_DEPLOYQT_TYPE "--release")
    endif ()

    add_custom_command(TARGET qfm1000
            POST_BUILD
            COMMAND ${QT_DEPLOYQT_BIN} $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR}/windeployqt)

    install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
            DESTINATION ${QFM1000_TARGET_INSTALL_DIR} COMPONENT qt_libs)
elseif (APPLE)
    set_target_properties(qfm1000
            PROPERTIES
            MACOSX_BUNDLE_ICON_FILE ../../icons/macos/icon.icns)
    add_custom_command(TARGET qfm1000
            POST_BUILD
            COMMAND ${QT_DEPLOYQT_BIN} $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE})
endif ()

install(TARGETS qfm1000
        RUNTIME DESTINATION ${QFM1000_TARGET_INSTALL_DIR} COMPONENT app_qfm1000
        BUNDLE DESTINATION ${QFM1000_TARGET_INSTALL_DIR} COMPONENT app_qfm1000)
