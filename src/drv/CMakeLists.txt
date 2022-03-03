set(sources
    src/uart.c
    src/wdts.c
    src/io.c
    src/log.c
)

add_library(drv ${sources})

set_source_files_properties(
    ${sources}
    PROPERTIES COMPILE_FLAGS
    "-pedantic -Werror -Wextra"
)

target_link_libraries(drv PUBLIC stm32f4xx)
target_link_libraries(drv PUBLIC freertos)

target_include_directories(drv PUBLIC inc)
target_include_directories(drv PRIVATE inc/drv)
