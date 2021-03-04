find_package(Qt5 REQUIRED COMPONENTS
        Core Gui Widgets)

add_library(delegators
        readonly.cpp readonly.hpp
        flag.cpp flag.hpp
        ctcss.cpp ctcss.hpp
        power.cpp power.hpp
        squelch.cpp squelch.hpp)

target_link_libraries(delegators
        eeprom
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)

target_include_directories(delegators
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
