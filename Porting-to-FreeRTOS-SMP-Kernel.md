The FreeRTOS API remains substantially the same between single core and SMP
versions except for [these additions](SMP.md). Therefore, an application written
for the FreeRTOS single core version should compile with the SMP version with
minimal to no effort. However, there may be some functional issues, as some
assumptions which were true for single core applications may no longer be true
for those on multi-cores.

One such common assumption is that a lower priority task cannot run while a
higher priority task is running. While this was true on a single core, it is no
longer true for multi-cores, as multiple tasks can be running simultaneously. If
the application relies on relative task priorities to provide mutual exclusion,
it may observe unexpected results in a multi-core environment. The application
writer has couple of options to address this:

1. The best option is to update the application so that it does not rely on task
   priorities and uses synchronization primitives instead.
2. Another option is to pin all the tasks which must not be running
   simultaneously to one core using the `vTaskCoreAffinitySet` API.
3. Another option is to define `configRUN_MULTIPLE_PRIORITIES` to `0` which
   ensures that multiple tasks will run simultaneously only if they have the
   same priority. Note that this may result in under utilization and put some
   cores to idle when they could be used to run other low priority tasks.

One other common assumption is that ISRs cannot run simultaneously with each
other or with other tasks. This is no longer true in a multi-core environment
and the application writer needs to ensure proper mutual exclusion while
accessing shared data between tasks and ISRs. The macros `taskENTER_CRITICAL_FROM_ISR()`
and `taskEXIT_CRITICAL_FROM_ISR()` can be used in ISRs and the macros
`taskENTER_CRITICAL()` and `taskEXIT_CRITICAL()` can be used in tasks to provide
such mutual exclusion.
