# CMake minimum version 3.13
cmake_minimum_required(VERSION 3.13)

# Setting pico sdk path
set(PICO_SDK_PATH /workspaces/pico-sdk)

# Including predefined configaration for detecting pico sdk
include(pico_sdk_import.cmake)

# Enabling C, CPP, ASM for this projects
project(hello-asm C CXX ASM)

# Selecting c and cpp version
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# init pico sdk
pico_sdk_init()

# including src dir
include_directories(${CMAKE_SOURCE_DIR})

# add src to executable
add_executable(hello-asm
  hello.S
)

# Disabling uart and enabling usb serial
pico_enable_stdio_uart(hello-asm 0)
pico_enable_stdio_usb(hello-asm 1)

# Generate uf2 file 
pico_add_extra_outputs(hello-asm)

# Including pico stdlib
target_link_libraries(hello-asm pico_stdlib)