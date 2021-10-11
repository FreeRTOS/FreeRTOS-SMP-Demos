The FreeRTOS API remains mostly same between single core and SMP version except
[these additions](SMP.md). Therefore, an application written for FreeRTOS single
core version should compile with SMP version with minimal to no effort. However,
there may be some functional issues as some assumptions which used to be true
for single core may no longer be true for multi-cores.

One such common assumption is that a lower priority task cannot be running while
a higher priority task is running. While this used to be true for single core,
it is no longer true for multi-cores as multiple tasks can be running
simultaneously. If the application relies on relative task priorities to provide
mutual exclusion, it may observe unexpected results in multi-core environment.
The application writer has couple of options to address it:

1. The best option is to update the application to not rely on task priorities
   and use synchronization primitives instead.
2. Another option is to pin all the tasks which must not be running
   simultaneously to one core using `vTaskCoreAffinitySet` API.
3. Another option is to define `configRUN_MULTIPLE_PRIORITIES` to `0` which
   ensures that multiple tasks will run simultaneously only if they have same
   priority. Note that it may result in under utilization and put some cores to
   idle while they could be used to run other low priority tasks.
