This repository contains demo applications for the [Symmetric Multiprocessing (SMP)
support in the FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/smp).
See the following links for details of demo applications:
* [XCORE AI](FreeRTOS/Demo/XCORE.AI_xClang/README.md)
* [Raspberry Pi Pico](FreeRTOS/Demo/CORTEX_M0+_RP2040/README.md)

## Getting started
The [FreeRTOS.org](https://www.freertos.org) website contains contains a
[FreeRTOS Kernel Quick Start Guide](https://www.freertos.org/FreeRTOS-quick-start-guide.html),
a [list of supported devices and compilers](https://www.freertos.org/RTOS_ports.html),
the [API reference](https://www.freertos.org/a00106.html), and many other resources.

## Getting help
You can use your Github login to get support from both the FreeRTOS community
and directly from the primary FreeRTOS developers on our
[active support forum](https://forums.freertos.org).
The [FAQ](https://www.freertos.org/FAQ.html) provides another support resource.

## Cloning this repository
This repo uses [Git Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
to bring in dependent components.

**Note:** If you download the ZIP file provided by the GitHub UI, you will not get the contents of the submodules. (The ZIP file is also not a valid git repository)

To clone using HTTPS:
```
git clone https://github.com/FreeRTOS/FreeRTOS-SMP-Demos.git --recurse-submodules
```
Using SSH:
```
git clone git@github.com:FreeRTOS/FreeRTOS-SMP-Demos.git --recurse-submodules
```

If you have downloaded the repo without using the `--recurse-submodules` argument, you need to run:
```
git submodule update --init --recursive
```

## Repository structure
This repository contains the FreeRTOS Kernel and a comprehensive set of example
applications.  The FreeRTOS Kernel is included as Git submodule from its own
Git repository.

### Kernel source code and example projects
```FreeRTOS/Source``` contains the FreeRTOS kernel source code
(submoduled from https://github.com/FreeRTOS/FreeRTOS-Kernel SMP branch).

```FreeRTOS/Demo``` contains pre-configured example projects that demonstrate
the FreeRTOS kernel executing on different hardware platforms and using
different compilers.
