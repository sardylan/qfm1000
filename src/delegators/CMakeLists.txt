add_library(delegators
        readonly.cpp readonly.hpp
        flag.cpp flag.hpp
        ctcss.cpp ctcss.hpp
        power.cpp power.hpp
        squelch.cpp squelch.hpp)

target_link_libraries(delegators
        eeprom
        Qt::Core
        Qt::Gui
        Qt::Widgets)

target_include_directories(delegators
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
