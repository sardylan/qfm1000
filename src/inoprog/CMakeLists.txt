add_library(inoprog
        inoprog.cpp inoprog.hpp)

target_link_libraries(inoprog
        utilities
        ${QT_LIB_NAME}::Core
        ${QT_LIB_NAME}::SerialPort
        ${QT_LIB_NAME}::Concurrent)

target_include_directories(inoprog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
