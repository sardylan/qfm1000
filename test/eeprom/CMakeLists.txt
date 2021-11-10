add_executable(eeprom-eeprom-test
        eeprom.cpp eeprom.hpp)

target_link_libraries(eeprom-eeprom-test
        eeprom ${QT_LIB_NAME}::Core ${QT_LIB_NAME}::Test)

add_test(eeprom-eeprom-test
        eeprom-eeprom-test -o test-result-eeprom-eeprom.xml,${QT_TEST_OUTPUT_FORMAT})

add_executable(eeprom-values-test
        values.cpp values.hpp)

target_link_libraries(eeprom-values-test
        eeprom ${QT_LIB_NAME}::Core ${QT_LIB_NAME}::Test)

add_test(eeprom-values-test
        eeprom-values-test -o test-result-eeprom-values.xml,${QT_TEST_OUTPUT_FORMAT})
