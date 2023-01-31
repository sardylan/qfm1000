add_executable(windows-model-utilities-test
        utilities.cpp utilities.hpp
        ../../../src/windows/model/utilities.cpp ../../../src/windows/model/utilities.hpp)

target_link_libraries(windows-model-utilities-test
        eeprom Qt5::Core Qt5::Test)

add_test(windows-model-utilities-test
        windows-model-utilities-test -o test-result-windows-model-utilities.xml,${QT_TEST_OUTPUT_FORMAT})
