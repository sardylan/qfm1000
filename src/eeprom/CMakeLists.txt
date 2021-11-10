add_library(eeprom
        eeprom.cpp eeprom.hpp
        filemanager.cpp filemanager.hpp
        values.cpp values.hpp
        model.cpp model.cpp)

target_link_libraries(eeprom
        ${QT_LIB_NAME}::Core)

target_include_directories(eeprom PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

