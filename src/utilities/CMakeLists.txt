add_library(utilities
        service.cpp service.hpp
        serialport.cpp serialport.hpp)

target_link_libraries(utilities
        Qt::Core
		Qt::SerialPort)

target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
