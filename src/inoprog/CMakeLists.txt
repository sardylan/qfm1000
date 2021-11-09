add_library(inoprog
        inoprog.cpp inoprog.hpp)

target_link_libraries(inoprog
        utilities
        Qt::Core
        Qt::SerialPort
        Qt::Concurrent)

target_include_directories(inoprog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
