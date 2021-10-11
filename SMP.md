# Introduction

[Symmetric Multiprocessing (SMP) support in FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/smp)
enables running multiple tasks simultaneously on multi-core microcontrollers.
Mutlti-core microcontrollers contain two or more identical processor cores which
share the same memory. FreeRTOS-SMP kernel utilizes all of those cores to
schedule multiple ready tasks simultaneously.

# New APIs

The following new APIs have been added to the FreeRTOS-SMP Kernel:
* [vTaskCoreAffinitySet](#vtaskcoreaffinityset)
* [vTaskCoreAffinityGet](#vtaskcoreaffinityget)
* [vTaskPreemptionDisable](#vtaskpreemptiondisable)
* [vTaskPreemptionEnable](#vtaskpreemptionenable)

## vTaskCoreAffinitySet

```c
void vTaskCoreAffinitySet( const TaskHandle_t xTask, UBaseType_t uxCoreAffinityMask );
```

`configUSE_CORE_AFFINITY` must be defined as `1` for this function to be available.

Sets the core affinity mask for a task i.e. the cores on which a task can run.

**Parameters:**

`xTask` The handle of the task to set the core affinity mask for. Passing `NULL`
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

Gets the core affinity mask for a task i.e. the cores on which a task can run.

**Parameters:**

`xTask` The handle of the task to get the core affinity mask for. Passing `NULL`
        will get the core affinity mask for the calling task.

**Returns:**

The core affinity mask which is a bitwise value that indicates the cores on
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
     * the core different than the networking task to ensure that the
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

Disables preemption for a task.

**Parameters:**

`xTask` The handle of the task to disable preemption. Passing `NULL` disables
        preemption for the calling task.

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

Enables preemption for a task.

**Parameters:**

`xTask` The handle of the task to enable preemption. Passing `NULL` enables
        preemption for the calling task.

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

# New Configuration Options

The following new configuration options have been added to the FreeRTOS-SMP
Kernel:
* [configNUM_CORES](#confignumcores)
* [configRUN_MULTIPLE_PRIORITIES](#configrunmultiplepriorities)
* [configUSE_CORE_AFFINITY](#configusecoreaffinity)

## configNUM_CORES

Sets the number of cores.

## configRUN_MULTIPLE_PRIORITIES

Configures if multiple priorities task can run simultaneously. If
`configRUN_MULTIPLE_PRIORITIES` is defined as `0`, multiple tasks may run
simultaneously only if they have equal priority. If
`configRUN_MULTIPLE_PRIORITIES` is defined as `1`, multiple tasks with different
priorities may run simultaneously.

## configUSE_CORE_AFFINITY

Enables the application writer to control which cores a task can run on.
If `configUSE_CORE_AFFINITY` is defined as `1`, `vTaskCoreAffinitySet` can be
used to control which cores a task can run on. `vTaskCoreAffinityGet` can be
used to query which cores a task can run on.
