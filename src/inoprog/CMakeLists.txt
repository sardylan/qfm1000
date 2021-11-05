find_package(Qt6 REQUIRED COMPONENTS
        Core SerialPort Concurrent)

add_library(inoprog
        inoprog.cpp inoprog.hpp)

target_link_libraries(inoprog
        utilities
        Qt6::Core
        Qt6::SerialPort
        Qt6::Concurrent)

target_include_directories(inoprog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
