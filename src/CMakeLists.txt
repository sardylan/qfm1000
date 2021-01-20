add_compile_options(-Wall -Wextra -pedantic -Werror)

include_directories(.)

add_subdirectory(utilities)
add_subdirectory(eeprom)
add_subdirectory(inoprog)

add_subdirectory(app)
