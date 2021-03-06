
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "pico.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "pico_support.h"


void main2()
{
  sleep_ms(5000);
  while(1)
  {
    printf("Other Core\n");
    printf("Running on Core %d\n", portSIE_CPUID );

    portGET_ISR_LOCK();
    printf("ISR LOCK ACHIEVED\n");
    sleep_ms(500);
    portGET_TASK_LOCK();
    printf("TASK LOCK ACHIEVED\n");
    portRELEASE_ISR_LOCK();
    sleep_ms(500);
    portRELEASE_TASK_LOCK();
    sleep_ms(500);
  }
}

int main()
{
  stdio_init_all();

  sleep_ms(5000);
  multicore_launch_core1(main2);

  while(1)
  {
    printf("PICO Testing Startup\n");
    printf("Running on Core %d\n", portSIE_CPUID );

    printf("SPINLOCKS are : %X\n", portSIE_SPINLOCK_ST);

    portGET_ISR_LOCK();
    portGET_TASK_LOCK();
    printf("main thread got both locks\n");
    sleep_ms(2000);
    portRELEASE_TASK_LOCK();
    portRELEASE_ISR_LOCK();

    sleep_ms(1000);
  }
  return 0;
}

