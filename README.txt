
        - Install linux package
        -- `sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib -y`

        - Install Pico sdk
        -   cd ..
            git clone https://github.com/raspberrypi/pico-sdk.git
        -   cd pico-sdk
            git submodule update --remote
        -   export PICO_SDK_PATH=`pwd`

        - Pico SDK dependecy
            --
        - Build command
            -- Create a build folder inside project folder `mkdir build`
            -- Go to build folder `cd build`
            -- Generate make build script `cmake ..`
            -- Then build `make` 

        -- AVR GCC
            -- https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/avr8-gnu-toolchain-3.7.0.1796-linux.any.x86_64.tar.gz
