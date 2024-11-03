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
project(pico_bwp C CXX ASM)

# Selecting c and cpp version
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# add src to executable
add_executable(pico_bwp)

target_sources(pico_bwp PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(pico_bwp PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(pico_bwp PUBLIC pico_stdlib hardware_adc hardware_dma)

# uart
pico_enable_stdio_uart(pico_bwp 1)
pico_enable_stdio_usb(pico_bwp 0)

# Generate uf2 file 
pico_add_extra_outputs(pico_bwp)