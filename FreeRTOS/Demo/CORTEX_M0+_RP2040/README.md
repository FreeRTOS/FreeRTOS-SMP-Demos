# FreeRTOS SMP Demos for Raspberry Pi Pico Board

> **FreeRTOS-SMP Kernel is still being tested.**

This page documents demos that use the [FreeRTOS symmetric multiprocessing (SMP) version](../../../SMP.md).
The demos target the [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)
board, which uses the [RP2040](https://www.raspberrypi.com/documentation/microcontrollers/rp2040.html#welcome-to-rp2040)
microcontroller from [Raspberry Pi](https://www.raspberrypi.com/) featuring a
Dual-core ARM Cortex M0+ processor.

These demo applications use the [GNU ARM Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
to build the FreeRTOS Raspberry Pi Pico port. They demonstrate support for [Symmetric
Multiprocessing (SMP)](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/smp) in the FreeRTOS Kernel.

----

## IMPORTANT! Notes on using the FreeRTOS Raspberry Pi Pico demo applications

Please read all the following points before using this RTOS port.

1. [Source Code Organization](#source-code-organization)
1. [The Demo Applications](#the-demo-applications)
1. [Building and Running the RTOS Demo Applications](#building-and-running-the-rtos-demo-applications)
1. [RTOS Configuration and Usage Details](#rtos-configuration-and-usage-details)

Also see the FAQ [My application does not run, what could be wrong](https://www.freertos.org/FAQHelp.html)?

----

## Source Code Organization
The project files for this demo are located in the `FreeRTOS/Demo/CORTEX_M0+_RP2040`
directory of the [FreeRTOS SMP Demo Git repository](https://github.com/FreeRTOS/FreeRTOS-SMP-Demos).
FreeRTOS Port files compiled in the project are in the
`FreeRTOS/Source/portable/ThirdParty/GCC/RP2040` directory.

----

## The Demo Applications
The project includes the following demos:
1. Blinky Demo.
1. Comprehensive Demo.
1. Multicore Demo.

### Blinky Demo
The blinky demo uses two tasks and one queue.

* The Queue Send Task:

  The queue send task is implemented by the `prvQueueSendTask()` function. The
  task sits in a loop and sends the value 100 to the queue every 1000 milliseconds
  (1 second).

* The Queue Receive Task:

  The queue receive task is implemented by the `prvQueueReceiveTask()` function.
  The task sits in a loop that blocks on attempts to read from the queue
  (no CPU cycles are consumed while the task is blocked), and toggles an LED each
  time the value 100 is received from the queue send task. As the queue send
  task writes to the queue every 1000 milliseconds, the queue receive task
  unblocks and toggles the LED every 1000 milliseconds.

### Comprehensive Demo
The comprehensive demo implements a comprehensive test and demo application that,
among other things, demonstrates and/or tests:

* [Message buffers](https://www.freertos.org/RTOS-stream-message-buffers.html)
* [Stream buffers](https://www.freertos.org/RTOS-stream-message-buffers.html)
* [Task notifications](https://www.freertos.org/RTOS-task-notifications.html)
* [Queues](https://www.freertos.org/Embedded-RTOS-Queues.html)
* [Semaphores](https://www.freertos.org/Embedded-RTOS-Binary-Semaphores.html)
* [Mutexes](https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html)
* [Event groups](https://www.freertos.org/FreeRTOS-Event-Groups.html)
* [Software timers](https://www.freertos.org/RTOS-software-timer.html)

The created tasks are from the set of [standard demo](https://www.freertos.org/a00102.html)
tasks that are used by all FreeRTOS port demo applications. They have no
specific functionality, but are created just to demonstrate how to use the
FreeRTOS API, and test the RTOS port.

A "check" task is created that periodically inspects the standard demo tasks
(which contain self monitoring code) to ensure all the tasks are functioning
as expected. The check task toggles the LED each time it executes. This
gives visual feedback of the system health. **If the LED is toggling every 3
seconds, then the check task has not discovered any problems. If the LED is
toggling every 200 milliseconds, then the check task has discovered a problem
in one or more tasks.**

### Multicore Demo
The multicore demo application runs FreeRTOS tasks on one core which interacts
with the code running on the other core using Raspberry Pico SDK synchronization
primitives. There are two versions of the same demo - One version runs FreeRTOS
on core 0, the other runs FreeRTOS on core 1.

## Building and Running the RTOS Demo Applications

### Building
1. Setup the Raspberry Pi Pico SDK build environment by following the instructions for
[Getting Started With Pico](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf).
Ensure that `PICO_SDK_PATH` is set in your environment, or pass it via
`-DPICO_SDK_PATH=xxx` on the CMake command line.
2. Run the following commands:
```sh
$ cd FreeRTOS/Demo/CORTEX_M0+_RP2040
$ mkdir build
$ cd build
$ cmake ..
$ make
```
This will generate `.uf2` files for each demo application:
* Blinky Demo - `FreeRTOS/Demo/CORTEX_M0+_RP2040/build/Standard/main_blinky.uf2`.
* Comprehensive Demo - `FreeRTOS/Demo/CORTEX_M0+_RP2040/build/Standard/main_full.uf2`
* Multicore Demo
  * `FreeRTOS/Demo/CORTEX_M0+_RP2040/build/OnEitherCore/on_core_zero.uf2`
  * `FreeRTOS/Demo/CORTEX_M0+_RP2040/build/OnEitherCore/on_core_one.uf2`

### Running
1. Connect the Raspberry Pi Pico to your computer while holding the `BOOTSEL`
button. This will force the board into USB Mass Storage Mode.
2. Drag-and-drop the `.uf2` file for the demo you want to run onto the Mass
Storage Device.
----

## RTOS Configuration and Usage Details
* Configuration items specific to the blinky and comprehensive demos are contained in
`FreeRTOS/Demo/CORTEX_M0+_RP2040/Standard/FreeRTOSConfig.h` and those specific
to the multicore demo are contained in `FreeRTOS/Demo/CORTEX_M0+_RP2040/OnEitherCore/FreeRTOSConfig.h`.
The [constants defined in these files](https://www.freertos.org/a00110.html) can be
edited to suit your application. The following configuration options are
specific to the SMP support in the FreeRTOS Kernel:
  * `configNUM_CORES` - Set the number of cores.
  * `configRUN_MULTIPLE_PRIORITIES` - Enable/Disable simultaneously running tasks with multiple priorities.
  * `configUSE_CORE_AFFINITY` - Enable/Disable setting a task's affinity to certain cores.
* `Source/Portable/MemMang/heap_4.c` is included in the project to provide the
memory allocation required by the RTOS kernel. Please refer to the
[Memory Management](https://www.freertos.org/a00111.html) section of the API
documentation for complete information.
* vPortEndScheduler() has not been implemented.
