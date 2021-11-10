add_library(widgets
        hexeditor.cpp hexeditor.hpp)

target_link_libraries(widgets
        ${QT_LIB_NAME}::Core
        ${QT_LIB_NAME}::Gui
        ${QT_LIB_NAME}::Widgets)

target_include_directories(utilities
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(widgets_sample sample/main.cpp)
target_link_libraries(widgets_sample widgets ${QT_LIB_NAME}::Core)