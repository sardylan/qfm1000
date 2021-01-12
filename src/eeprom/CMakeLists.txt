find_package(Qt5 REQUIRED COMPONENTS
        Core Test)

add_library(eeprom
        eeprom.cpp eeprom.hpp
        filemanager.cpp filemanager.hpp
        values.cpp values.hpp)

target_link_libraries(eeprom
        Qt5::Core)

target_include_directories(eeprom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(eeprom-test
        test/eeprom.cpp test/eeprom.hpp)

target_link_libraries(eeprom-test
        eeprom Qt5::Core Qt5::Test)

add_test(eeprom-test
        eeprom-test -o test-result-eeprom.xml,xunitxml)
