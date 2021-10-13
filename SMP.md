# Introduction

[Symmetric Multiprocessing (SMP) support in FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/smp)
enables you to run multiple tasks simultaneously on multi-core microcontrollers.
Mutlti-core microcontrollers contain two or more identical processor cores which
share the same memory. The FreeRTOS-SMP kernel utilizes all of those cores to
schedule multiple ready tasks simultaneously.

# Getting Started

The simplest way to get started is to use one of the following pre-configured
example projects:

* [XCORE AI](FreeRTOS/Demo/XCORE.AI_xClang/README.md)
* [Raspberry Pi Pico](FreeRTOS/Demo/CORTEX_M0+_RP2040/README.md)

# New APIs

These additional APIs are available to the FreeRTOS-SMP Kernel:
* [vTaskCoreAffinitySet](#vtaskcoreaffinityset)
* [vTaskCoreAffinityGet](#vtaskcoreaffinityget)
* [vTaskPreemptionDisable](#vtaskpreemptiondisable)
* [vTaskPreemptionEnable](#vtaskpreemptionenable)

## vTaskCoreAffinitySet

```c
void vTaskCoreAffinitySet( const TaskHandle_t xTask, UBaseType_t uxCoreAffinityMask );
```

`configUSE_CORE_AFFINITY` must be defined as `1` for this function to be available.

Sets the core affinity mask for a task, i.e. the cores on which a task can run.

**Parameters:**

`xTask` The handle of the task that the core affinity mask is for. Passing `NULL`
        will set the core affinity mask for the calling task.

`uxCoreAffinityMask` A bitwise value that indicates the cores on which the task
                     can run. Cores are numbered from `0` to
                     `configNUM_CORES - 1`. For example, to ensure that a task
                     can run on core `0` and core `1`, set `uxCoreAffinityMask`
                     to `0x03`.

**Example Usage:**

```c
/* The function that creates task. */
void vAFunction( void )
{
TaskHandle_t xHandle;
UBaseType_t uxCoreAffinityMask;

    /* Create a task, storing the handle. */
    xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &( xHandle ) );

    /* Define the core affinity mask such that this task can only run on core 0
     * and core 2. */
    uxCoreAffinityMask = ( ( 1 << 0 ) | ( 1 << 2 ) );

    /* Set the core affinity mask for the task. */
    vTaskCoreAffinitySet( xHandle, uxCoreAffinityMask );
}
```

## vTaskCoreAffinityGet

```c
UBaseType_t vTaskCoreAffinityGet( const TaskHandle_t xTask );
```

`configUSE_CORE_AFFINITY` must be defined as `1` for this function to be available.

Gets the core affinity mask for a task, i.e. the cores on which a task can run.

**Parameters:**

`xTask` The handle of the task that the core affinity mask is for. Passing `NULL`
        will get the core affinity mask for the calling task.

**Returns:**

The core affinity mask, which is a bitwise value that indicates the cores on
which a task can run. Cores are numbered from `0` to `configNUM_CORES - 1`.
For example, if a task can run on core `0` and core `1`, the core affinity mask
is `0x03`.

**Example Usage:**

```c
/* Task handle of the networking task - it is populated elsewhere. */
TaskHandle_t xNetworkingTaskHandle;

void vAFunction( void )
{
TaskHandle_t xHandle;
UBaseType_t uxNetworkingCoreAffinityMask;

    /* Create a task, storing the handle. */
    xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &( xHandle ) );

    /* Get the core affinity mask for the networking task. */
    uxNetworkingCoreAffinityMask = vTaskCoreAffinityGet( xNetworkingTaskHandle );

    /* Here is a hypothetical scenario, just for the example. Assume that we
     * have 2 cores - Core 0 and core 1. We want to pin the application task to
     * the core that is not the networking task core to ensure that the
     * application task does not interfere with networking. */
    if( ( uxNetworkingCoreAffinityMask & ( 1 << 0 ) ) != 0 )
    {
        /* The networking task can run on core 0, pin our task to core 1. */
        vTaskCoreAffinitySet( xHandle, ( 1 << 1 ) );
    }
    else
    {
        /* Otherwise, pin our task to core 0. */
        vTaskCoreAffinitySet( xHandle, ( 1 << 0 ) );
    }
}
```

## vTaskPreemptionDisable

```c
void vTaskPreemptionDisable( const TaskHandle_t xTask );
```

`configUSE_TASK_PREEMPTION_DISABLE` must be defined as `1` for this function to be available.

Disables preemption for a task.

**Parameters:**

`xTask` The handle of the task for which preemption will be disabled. Passing
        `NULL` disables preemption for the calling task.

**Example Usage:**

```c
void vTaskCode( void *pvParameters )
{
    /* Silence warnings about unused parameters. */
    ( void ) pvParameters;

    for( ;; )
    {
        /* ... Perform some function here. */

        /* Disable preemption for this task. */
        vTaskPreemptionDisable( NULL );

        /* The task will not be preempted when it is executing in this portion ... */

        /* ... until the preemption is enabled again. */
        vTaskPreemptionEnable( NULL );

        /* The task can be preempted when it is executing in this portion. */
    }
}
```

## vTaskPreemptionEnable

```c
void vTaskPreemptionEnable( const TaskHandle_t xTask );
```

`configUSE_TASK_PREEMPTION_DISABLE` must be defined as `1` for this function to be available.

Enables preemption for a task.

**Parameters:**

`xTask` The handle of the task for which preemption will be enabled. Passing
        `NULL` enables preemption for the calling task.

**Example Usage:**

```c
void vTaskCode( void *pvParameters )
{
    /* Silence warnings about unused parameters. */
    ( void ) pvParameters;

    for( ;; )
    {
        /* ... Perform some function here. */

        /* Disable preemption for this task. */
        vTaskPreemptionDisable( NULL );

        /* The task will not be preempted when it is executing in this portion ... */

        /* ... until the preemption is enabled again. */
        vTaskPreemptionEnable( NULL );

        /* The task can be preempted when it is executing in this portion. */
    }
}
```

# New Hook Functions

## Minimal Idle Hook Function
The FreeRTOS-SMP kernel has two type of Idle tasks:
1. Idle Task - There is the one usual Idle task which does all the garbage collection.
2. Minimal Idle Tasks - There are `configNUM_CORES - 1` Minimal Idle tasks which
   are run on idle cores and which do nothing.

The minimal idle tasks can optionally call an application-defined hook
(or callback) function - the minimal idle hook. The minimal idle tasks run at
the very lowest priority, so such an idle hook function will only run
when there are no tasks of higher priority that are able to run.

The minimal idle hook will only get called if `configUSE_MINIMAL_IDLE_HOOK` is
set to `1` within `FreeRTOSConfig.h`. When this is set, the application must
provide the hook function with the following prototype:

```c
void vApplicationMinimalIdleHook( void );
```

The minimal idle hook is called repeatedly by the minimal idle tasks as
long as any one of them is running. **It is paramount that the minimal idle hook
function does not call any API functions that could cause it to block.**

# New Configuration Options

These additional configuration options are available to the FreeRTOS-SMP
Kernel:
* [configNUM_CORES](#confignumcores)
* [configRUN_MULTIPLE_PRIORITIES](#configrunmultiplepriorities)
* [configUSE_CORE_AFFINITY](#configusecoreaffinity)
* [configUSE_TASK_PREEMPTION_DISABLE](#configusetaskpreemptiondisable)

## configNUM_CORES

Sets the number of cores.

## configRUN_MULTIPLE_PRIORITIES

Configures whether tasks with multiple priorities can run simultaneously. If
`configRUN_MULTIPLE_PRIORITIES` is defined as `0`, multiple tasks may run
simultaneously only if they have equal priority. If
`configRUN_MULTIPLE_PRIORITIES` is defined as `1`, multiple tasks with different
priorities may run simultaneously.

## configUSE_CORE_AFFINITY

Allows the application writer to control which cores a task can run on.
If `configUSE_CORE_AFFINITY` is defined as `1`, `vTaskCoreAffinitySet` can be
used to control which cores a task can run on, and `vTaskCoreAffinityGet` can be
used to query which cores a task can run on.

## configUSE_TASK_PREEMPTION_DISABLE

Allows the application writer to disable preemption on a per task basis. When
`configUSE_TASK_PREEMPTION_DISABLE` is defined as `1`, `vTaskPreemptionDisable`
and `vTaskPreemptionEnable` APIs can be used to disable and enable preemption on
a per task basis.
