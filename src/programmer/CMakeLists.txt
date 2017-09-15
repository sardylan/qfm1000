set(QFM1000_PROGRAMMER_SOURCES
        programmer.cpp)

set(QFM1000_PROGRAMMER_HEADERS
        programmer.hpp)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        SerialPort
        Test)

add_library(programmer
        ${QFM1000_PROGRAMMER_SOURCES}
        ${QFM1000_PROGRAMMER_HEADERS})

target_link_libraries(programmer
        Qt5::Core
        Qt5::SerialPort)

target_include_directories(programmer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testprogrammer
        test/programmer_test.hpp
        test/programmer_test.cpp)

target_link_libraries(testprogrammer
        Qt5::Core
        Qt5::SerialPort
        Qt5::Test
        programmer)
