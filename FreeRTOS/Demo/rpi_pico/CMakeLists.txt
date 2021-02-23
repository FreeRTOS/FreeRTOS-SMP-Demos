# What CMake to start at
cmake_minimum_required(VERSION 3.12)

# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)
 
# Set the name and version of the project
project(PicoTest VERSION 1.0.0)
 
# Link the Project to a source file (step 4.6)
add_executable(PicoTest source.c)
 
# Link the Project to an extra library (pico_stdlib)
target_link_libraries(PicoTest pico_stdlib FreeRTOS FreeRTOS_port)

set(KERNEL_PATH ../../Source)
include_directories(AFTER ${KERNEL_PATH}/include)
include_directories(AFTER ${KERNEL_PATH}/portable/GCC/ARM_CM0)
include_directories(AFTER .)
# Get the FreeRTOS Kernel
add_subdirectory(${KERNEL_PATH} kernel)
# Get the FreeRTOS Kernel Port for Cortex M0+
add_subdirectory(${KERNEL_PATH}/portable/GCC/ARM_CM0 kernel_port)

# Initalise the SDK
pico_sdk_init()
 
# Enable USB, UART output
pico_enable_stdio_usb(PicoTest 1)
pico_enable_stdio_uart(PicoTest 0)
 
# Enable extra outputs (SWD?)
pico_add_extra_outputs(PicoTest)
