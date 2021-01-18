find_package(Qt5 REQUIRED COMPONENTS
        Core SerialPort Concurrent)

add_library(inoprog
        inoprog.cpp inoprog.hpp)

target_link_libraries(inoprog
        utilities
        Qt5::Core
        Qt5::SerialPort
        Qt5::Concurrent)

target_include_directories(inoprog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
