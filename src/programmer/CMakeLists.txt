set(QFM1000_PROGRAMMER_SOURCES
        arduino.cpp)

set(QFM1000_PROGRAMMER_HEADERS
        arduino.hpp)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        SerialPort
        Concurrent)

add_library(programmer
        ${QFM1000_PROGRAMMER_SOURCES}
        ${QFM1000_PROGRAMMER_HEADERS})

target_link_libraries(programmer
        Qt5::Core
        Qt5::SerialPort
        Qt5::Concurrent)

target_include_directories(programmer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})