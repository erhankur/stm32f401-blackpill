cmake_minimum_required (VERSION 2.8.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# set the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message("Debug build.")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  message("Release build.")
endif()

# set search path for CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# include OpenOCD module, but make it not required
find_package(OpenOCD)

# Project specific settings
set(application_name "blackpill")

cmake_policy(SET CMP0048 NEW)
project (${application_name} VERSION 1.0.0 LANGUAGES C CXX ASM)

configure_file(${CMAKE_SOURCE_DIR}/src/app/inc/app/version.h.in
  ${CMAKE_SOURCE_DIR}/src/app/inc/app/version.h)

# platform-specific configurations
include(CMake/platform.cmake)

# STM32-specific settings
set(DEVICE_FAMILY STM32F401xx)

# Speed of external resonator
set(HSE_VALUE 16000000)

# Speed of internal resonator
set(HSI_VALUE 16000000)

# Used linker file (just for testing)
get_filename_component(LINKER_SCRIPT linker/stm32f401rbt6.ld ABSOLUTE)

# Use LTO? WARNING: This might break the code
#set(ADDITIONAL_CORE_FLAGS -flto)

# OpenOCD config file
#get_filename_component(OPENOCD_CONFIG tools/stm32f4discovery.cfg ABSOLUTE)
get_filename_component(OPENOCD_CONFIG tools/stm32f4x-jlink.cfg ABSOLUTE)

# add libraries
add_subdirectory(lib-hal)
add_subdirectory(ThirdParty)

# add main project
add_subdirectory(src)
