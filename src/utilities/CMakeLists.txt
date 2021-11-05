find_package(Qt6 REQUIRED COMPONENTS
        Core
		SerialPort)

add_library(utilities
        service.cpp service.hpp
        serialport.cpp serialport.hpp)

target_link_libraries(utilities
        Qt6::Core
		Qt6::SerialPort)

target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
