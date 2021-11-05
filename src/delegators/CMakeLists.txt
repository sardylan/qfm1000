find_package(Qt6 REQUIRED COMPONENTS
        Core Gui Widgets)

add_library(delegators
        readonly.cpp readonly.hpp
        flag.cpp flag.hpp
        ctcss.cpp ctcss.hpp
        power.cpp power.hpp
        squelch.cpp squelch.hpp)

target_link_libraries(delegators
        eeprom
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets)

target_include_directories(delegators
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
