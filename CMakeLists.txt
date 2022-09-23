cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_logicemu)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(pico_logicemu
        logicemu.c
        )

target_include_directories(pico_logicemu PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        )

# pull in common dependencies
target_link_libraries(pico_logicemu pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_logicemu)