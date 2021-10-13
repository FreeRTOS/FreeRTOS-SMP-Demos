# Introduction to Symmetric Multiprocessing (SMP) with FreeRTOS

[SMP support in FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/smp)
enables one instance of the FreeRTOS kernel to schedule tasks across multiple
identical processor cores.  The core architectures must be identical and share the
same memory.

# Getting Started with FreeRTOS and SMP

The simplest way to get started is to use one of the following pre-configured
example projects:

* [XCORE AI](FreeRTOS/Demo/XCORE.AI_xClang/README.md)
* [Raspberry Pi Pico](FreeRTOS/Demo/CORTEX_M0+_RP2040/README.md)

# SMP Specific APIs

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

# SMP Specific Hook Functions

## Minimal Idle Hook Function
The FreeRTOS SMP kernel has two type of Idle tasks:
1. Idle Task - There is the standard Idle task used in single core FreeRTOS applications.
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

# SMP Specific Configuration Options

These additional configuration options are available to the FreeRTOS-SMP
Kernel:
* [configNUM_CORES](#confignumcores)
* [configRUN_MULTIPLE_PRIORITIES](#configrunmultiplepriorities)
* [configUSE_CORE_AFFINITY](#configusecoreaffinity)
* [configUSE_TASK_PREEMPTION_DISABLE](#configusetaskpreemptiondisable)

## configNUM_CORES

Sets the number of available processor cores.

## configRUN_MULTIPLE_PRIORITIES

In a single core FreeRTOS application, a lower priority task will never run if
there is a higher priority task that is able to run.  In an SMP FreeRTOS application
the RTOS kernel will run as many tasks as there are cores available - so it is possible
that a lower priority task will run on one core at the same time as a higher priority task
runs on another core.  That can cause a problem if you application or library was
written for a single core environment, so makes assumptions about the order in
which tasks execute.  Therefore configRUN_MULTIPLE_PRIORITIES is provided to
control the behaviour.


If `configRUN_MULTIPLE_PRIORITIES` is defined as `0`, multiple tasks may run
simultaneously only if they have equal priority - maintaining the paradigm of
a lower priority task never running if there is a higher priority task that is
able to run.  If
`configRUN_MULTIPLE_PRIORITIES` is defined as `1`, multiple tasks with different
priorities may run simultaneously - so a higher and lower priority task may run
on different cores at the same time.

## configUSE_CORE_AFFINITY

Allows the application writer to control which cores a task can run on.
If `configUSE_CORE_AFFINITY` is defined as `1`, `vTaskCoreAffinitySet` can be
used to control which cores a task can run on, and `vTaskCoreAffinityGet` can be
used to query which cores a task can run on.  If `configUSE_CORE_AFFINITY` is 0
then the FreeRTOS scheduler is free to execute any task on any available core.

## configUSE_TASK_PREEMPTION_DISABLE

In a single core FreeRTOS application the FreeRTOS scheduler can be configured to
be either pre-emptive or co-operative.  See the definition of configUSE_PREEMPTION.
In SMP FreeRTOS application, if `configUSE_TASK_PREEMPTION_DISABLE` is defined as `1`, 
then individual tasks can be set to either pre-emptive or co-operative mode using the `vTaskPreemptionDisable`
and `vTaskPreemptionEnable` API functions.


