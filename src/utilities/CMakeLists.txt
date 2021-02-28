find_package(Qt5 REQUIRED COMPONENTS
        Core
		SerialPort)

add_library(utilities
        service.cpp service.hpp
        serialport.cpp serialport.hpp)

target_link_libraries(utilities
        Qt5::Core
		Qt5::SerialPort)

target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
