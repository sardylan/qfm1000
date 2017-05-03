set(QFM1000_EEPROM_SOURCES
        eeprom.cpp
        channel.cpp
        filemanager.cpp)

set(QFM1000_EEPROM_HEADERS
        eeprom.hpp
        channel.hpp
        filemanager.hpp)

find_package(Qt5 REQUIRED COMPONENTS
        Core)

add_library(eeprom
        ${QFM1000_EEPROM_SOURCES}
        ${QFM1000_EEPROM_HEADERS})

target_link_libraries(eeprom
        Qt5::Core)

target_include_directories(eeprom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
