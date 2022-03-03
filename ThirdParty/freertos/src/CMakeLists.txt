set(sources
    list.c
    queue.c
    tasks.c
    timers.c
    portable/GCC/ARM_CM4F/port.c
    portable/MemMang/heap_4.c
    FreeRTOS-openocd.c
)

add_library(freertos ${sources})

# add include directories
target_include_directories(freertos PUBLIC include)
target_include_directories(freertos PRIVATE include/freertos)
target_include_directories(freertos PUBLIC portable/GCC/ARM_CM4F)

# openocd awareness
target_link_libraries(freertos PRIVATE "-Wl,--undefined=uxTopUsedPriority")

# set up some defines 

# additional compiler options: use size-optimized version of library in release build, use -O0 in debug build
if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(additional_flags -O0)
else()
  set(additional_flags -Os)
endif()

target_compile_options(freertos PRIVATE ${additional_flags})
