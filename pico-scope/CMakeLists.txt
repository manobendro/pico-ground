# CMake minimum version 3.13
cmake_minimum_required(VERSION 3.13)

# Setting build type
set(CMAKE_BUILD_TYPE Debug)

# Setting pico sdk path for gitpod
set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/../lib/pico-sdk)

# Selecting wshare rp2040 zero
set(PICO_BOARD "waveshare_rp2040_zero")

# Tiny USB Configaration
set(FAMILY rp2040)
set(BOARD pico_sdk)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

# Enabled C,CPP,ASM
project(pico_scope C CXX ASM)

# Selecting c and cpp version
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# add src to executable
add_executable(pico_scope)

target_sources(pico_scope PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(pico_scope PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(pico_scope PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)

# Disabling USB and enabling UART serial
pico_enable_stdio_uart(pico_scope 1)
pico_enable_stdio_usb(pico_scope 0)

# Generate uf2 file 
pico_add_extra_outputs(pico_scope)