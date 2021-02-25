
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "pico/stdlib.h"

void blinkTask(void *pvParameters);
void serialTask(void *pvParameters);
void helloTask(void *pvParameters);
void pingTask(void *pvParameters);
void pongTask(void *pvParameters);

/* Static Queue Memory Allocation */

#define QUEUE_LENGTH    2
#define ITEM_SIZE       sizeof( bool )

static StaticQueue_t ledStaticQueue;
static StaticQueue_t pingStaticQueue;
static StaticQueue_t pongStaticQueue;

uint8_t ledQueueStorage[QUEUE_LENGTH * ITEM_SIZE];
uint8_t pingQueueStorage[QUEUE_LENGTH * ITEM_SIZE];
uint8_t pongQueueStorage[QUEUE_LENGTH * ITEM_SIZE];

QueueHandle_t ping;
QueueHandle_t pong;
QueueHandle_t ledQ;

/* Static Task Memory Allocation */

#define HELLO_STACK_SIZE 1024
#define BLINK_STACK_SIZE 60
#define PING_STACK_SIZE 128
#define PONG_STACK_SIZE 128

StaticTask_t xHelloTaskBuffer;
StaticTask_t xBlinkTaskBuffer;
StaticTask_t xPingTaskBuffer;
StaticTask_t xPongTaskBuffer;

StackType_t xHelloStack[ HELLO_STACK_SIZE ];
StackType_t xBlinkStack[ BLINK_STACK_SIZE ];
StackType_t xPingStack[ PING_STACK_SIZE ];
StackType_t xPongStack[ PONG_STACK_SIZE ];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static – otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task’s state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task’s stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static – otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task’s stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void main()
{
    ledQ = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE, ledQueueStorage, &ledStaticQueue);
    ping = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE, pingQueueStorage, &pingStaticQueue);
    pong = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE, pongQueueStorage, &pongStaticQueue);

    bool data = true;
    xQueueGenericSend(pong,&data,1,queueSEND_TO_BACK);

    xTaskCreateStatic( helloTask, "Hello", HELLO_STACK_SIZE, NULL, 5, xHelloStack, &xHelloTaskBuffer);
    xTaskCreateStatic( blinkTask, "blink", BLINK_STACK_SIZE, ledQ, 5, xBlinkStack, &xBlinkTaskBuffer );
    xTaskCreateStatic( pingTask, "Pinger", PING_STACK_SIZE, NULL, 5, xPingStack, &xPingTaskBuffer);
    xTaskCreateStatic( pongTask, "Ponger", PONG_STACK_SIZE, NULL , 5, xPongStack, &xPongTaskBuffer );

    vTaskStartScheduler();

}

void pingTask( void *pvParameters )
{
  bool data = true;
  bool ledCmd = true;
  while(1)
  {
    xQueueReceive(pong, &data, portMAX_DELAY);
    xQueueGenericSend(ledQ,&ledCmd,1,queueSEND_TO_BACK);
    vTaskDelay(pdMS_TO_TICKS(500));
    data = true;
    xQueueGenericSend(ping,&data,1,queueSEND_TO_BACK);
  }
}

void pongTask(void *pvParameters)
{
  bool data = false;
  bool ledCmd = false;
  while(1)
  {
    xQueueReceive(ping, &data, portMAX_DELAY);
    xQueueGenericSend(ledQ,&ledCmd,1,queueSEND_TO_BACK);
    vTaskDelay(pdMS_TO_TICKS(500));
    data = true;
    xQueueGenericSend(pong,&data,1,queueSEND_TO_BACK);
  }
}

void blinkTask(void *pvParameters)
{
    QueueHandle_t ledCommands = (QueueHandle_t) pvParameters;
    
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while(true)
    {
        bool command;
        xQueueReceive(ledCommands, &command, portMAX_DELAY);
        if(command)
        {
            gpio_put(LED_PIN, 1);
        }
        else
        {
            gpio_put(LED_PIN, 0);
        }
    }
}

void helloTask(void *pvParameters)
{  
  unsigned long counter = 0;
  while(true)
  {
    printf("Hello_World : %d",counter++);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

