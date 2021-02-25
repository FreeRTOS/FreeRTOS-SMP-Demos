# Introduction
Basic SMP demo for the Raspberry Pi Pico.  The intent is to create 4 tasks, 1 will toggle a pin, 2 will "ping" and "pong" to each other while the last tasks prints the "ping"/"pong" messages.

# Building the project
There is a CMakeLists.txt file that includes the FreeRTOS kernel and the CM0 port.  CMakeLists.txt has been added to the kernel & the port to make this process easier.  NOTE: CMakeLists.txt are not the normal way to build FreeRTOS in the primary repo.  This project is built with VisualStudio Code and the CMAKE Extension simply by running the "build" task and choosing GCC for arm-none-eabi.  Improvements are welcome.

The PICO SDK is NOT installed with this project.  You must install the SDK in a suitable directory on your machine and set the PICO_SDK_PATH environment variable.

# TODO
The project currently builds but it does not actually run.  Left to do are implementing the following MACRO's in portmacro.h.  NOTE: Documentation on these macro's is still in development.  Some comments are in the code.  All locations needing work are tagged TODO SMP in the code.

1. portGET_CORE_ID
1. portYIELD_CORE(a)
1. portRESTORE_INTERRUPTS(interrupt state)
1. portCHECK_IF_IN_ISR()
1. portGET_ISR_LOCK
1. portRELEASE_ISR_LOCK
1. portGET_TASK_LOCK
1. portRELEASE_TASK_LOCK

The TCB structure of the SMP port is currently an array of TCB's, one TCB for each core.  This causes significant work in the xPortPendSVHandler and vPortStartFirstTask functions.  There is 100% chance that the TCB architecture will change slighly requiring a few iterations of these functions.
