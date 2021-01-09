set(QFM1000_CONFIG_SOURCES
        config.cpp
        manager.cpp
        status.cpp)

set(QFM1000_CONFIG_HEADERS
        default.hpp
        config.hpp
        manager.hpp
        status.hpp)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        SerialPort)

add_library(config
        ${QFM1000_CONFIG_SOURCES}
        ${QFM1000_CONFIG_HEADERS})

target_link_libraries(config
        eeprom
        Qt5::Core
        Qt5::SerialPort)

target_include_directories(config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
