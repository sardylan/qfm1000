configure_file(qfm1000.desktop.in qfm1000.desktop @ONLY)

if (UNIX AND NOT APPLE)
    install(FILES ${CMAKE_BINARY_DIR}/desktop/qfm1000.desktop
            DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
            COMPONENT qfm1000)
    install(FILES ../icons/application.svg
            DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons"
            RENAME "qfm1000.svg"
            COMPONENT qfm1000)
endif ()