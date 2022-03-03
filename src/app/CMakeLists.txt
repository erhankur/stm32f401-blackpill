set(sources
    src/main.c
)

add_library(app ${sources})

set_source_files_properties(
    ${sources}
    PROPERTIES COMPILE_FLAGS
    "-pedantic -Werror -Wextra"
)

target_link_libraries(app PUBLIC stm32f4xx)
target_link_libraries(app PUBLIC freertos)
target_link_libraries(app PUBLIC drv)

target_include_directories(app PUBLIC inc)
target_include_directories(app PRIVATE inc/app)
