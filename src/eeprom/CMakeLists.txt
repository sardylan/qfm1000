find_package(Qt5 REQUIRED COMPONENTS
        Core Test)

add_library(eeprom
        eeprom.cpp eeprom.hpp
        filemanager.cpp filemanager.hpp
        values.hpp)

target_link_libraries(eeprom
        Qt5::Core)

target_include_directories(eeprom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(testeeprom
        test/eeprom.hpp
        test/eeprom.cpp)

target_link_libraries(testeeprom
        eeprom Qt5::Core Qt5::Test)

add_test(TestEEPROM
        testeeprom -o testresult-testeeprom.xml,xunitxml)

add_executable(testeeprombyte
        test/eeprombyte.hpp
        test/eeprombyte.cpp)

target_link_libraries(testeeprombyte
        eeprom
        Qt5::Core
        Qt5::Test)

add_test(TestEEPROMByte
        testeeprombyte -o testresult-testeeprombyte.xml,xunitxml)