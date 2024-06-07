# CMake minimum version 3.13
cmake_minimum_required(VERSION 3.13)

# Setting pico sdk path for github
#set(PICO_SDK_PATH /workspaces/pico-sdk)

# Setting pico sdk path for gitpod
set(PICO_SDK_PATH /workspace/pico-sdk)

# Selecting wshare rp2040 zero
set(PICO_BOARD "waveshare_rp2040_zero")

# Including predefined configaration for detecting pico sdk
include(pico_sdk_import.cmake)

# Enabling C, CPP, ASM for this projects
project(wozmon-rp2040 C CXX ASM)

# Selecting c and cpp version
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# init pico sdk
pico_sdk_init()

# including src dir
include_directories(${CMAKE_SOURCE_DIR})

# add src to executable
add_executable(wozmon-rp2040
  main.c
)

# Disabling uart and enabling usb serial
pico_enable_stdio_uart(wozmon-rp2040 0)
pico_enable_stdio_usb(wozmon-rp2040 1)

# Generate uf2 file 
pico_add_extra_outputs(wozmon-rp2040)

# Specify the path to your custom linker script
set(WOZMON_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/wozmon_linker.ld)

# Override the default linker script
pico_set_linker_script(wozmon-rp2040 ${WOZMON_LINKER_SCRIPT})

# Including pico stdlib
target_link_libraries(wozmon-rp2040 pico_stdlib)