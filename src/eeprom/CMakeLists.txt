add_library(eeprom
        eeprom.cpp eeprom.hpp
        filemanager.cpp filemanager.hpp
        values.cpp values.hpp
        model.cpp model.cpp)

target_link_libraries(eeprom
        Qt::Core)

target_include_directories(eeprom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(eeprom-eeprom-test
        test/eeprom.cpp test/eeprom.hpp)

target_link_libraries(eeprom-eeprom-test
        eeprom Qt::Core Qt::Test)

add_test(eeprom-eeprom-test
        eeprom-eeprom-test -o test-result-eeprom-eeprom.xml,junitxml)

add_executable(eeprom-values-test
        test/values.cpp test/values.hpp)

target_link_libraries(eeprom-values-test
        eeprom Qt::Core Qt::Test)

add_test(eeprom-values-test
        eeprom-values-test -o test-result-eeprom-values.xml,junitxml)
