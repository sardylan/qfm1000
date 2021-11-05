find_package(Qt6 REQUIRED COMPONENTS
        Core Gui Widgets)

add_library(widgets
        hexeditor.cpp hexeditor.hpp)

target_link_libraries(widgets
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets)

target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(widgets_sample sample/main.cpp)
target_link_libraries(widgets_sample widgets Qt6::Core)