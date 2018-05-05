set(QFM1000_EEPROM_SOURCES
        eeprom.cpp
        filemanager.cpp)

set(QFM1000_EEPROM_HEADERS
        eeprom.hpp
        filemanager.hpp)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Test)

add_library(eeprom
        ${QFM1000_EEPROM_SOURCES}
        ${QFM1000_EEPROM_HEADERS})

target_link_libraries(eeprom
        Qt5::Core)

target_include_directories(eeprom
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testeeprom
        test/eepromtest.hpp
        test/eepromtest.cpp)

target_link_libraries(testeeprom
        Qt5::Core
        Qt5::Test
        eeprom)

add_test(TestEEPROM
        testeeprom)

add_executable(testeeprombyte
        test/eeprombytetest.hpp
        test/eeprombytetest.cpp)

target_link_libraries(testeeprombyte
        Qt5::Core
        Qt5::Test
        eeprom)

add_test(TestEEPROMByte
        testeeprombyte)