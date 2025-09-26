/*
Author: Gabriel Rodgers
Date: 8/1/2024
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "tcnt.h"
#include "exti.h"
#include "nvic.h"
#include "uart.h"
#include "misc.h"

#include "stm32h723xx.h"

#include <FreeRTOS.h>
#include <task.h>


/* Macros */
#define STACK_SIZE (200)
#define NVIC_PriorityGroup_4 (~(1 << 10))

/* Globals */
extern volatile char uart2_ch;
volatile button_state_t button = UNPRESSED;

BaseType_t status;

/* Task Handles */
TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;
TaskHandle_t xHandle3 = NULL;

/* Task Prototypes */
void task1_handler(void *args);
void task2_handler(void *args);
void task3_handler(void *args);


static void hardware_init(void) {
	//clock_peripherals_init();
	gpio_button_init();
	gpio_led_init(PWM);
	nvic_enable();
	exti_init(); 

	timer3_pwm_init();
	timer12_pwm_init();
	//timer3_upcount_init(); 	
	uart_init(921600);

	SCB->AIRCR |= (VECTKEY);	//use the VECTKEY to gain write access to the AIRCR register
    SCB->AIRCR &= (NVIC_PriorityGroup_4); //clear bit 10 in AIRCR, resulting in no subpriorities
}

int main(void) {

	hardware_init();

	status = xTaskCreate( (TaskFunction_t) task1_handler, "Initialization Routine", STACK_SIZE, NULL, 1, &xHandle1);
	configASSERT(status == pdPASS);

	vTaskStartScheduler();

	//if the program gets to here, then that means that the heap was overrun - which is not good.
	while (1) {
	}

	return 0;
}

/*---------------------------TASKS---------------------------*/
/*-----------------------------------------------------------*/
void task1_handler(void *args) {
    uart2_ch = '\0';
   
    while (1) {
        gpio_pwm_set_duty('B', 0, MAX_PWM_ONE_BYTE);
        gpio_pwm_set_duty('B', 14, MAX_PWM_ONE_BYTE);
        
        printf("------------------------------------------------------------\r\n"
               "------- Highland Environment Controller Setup Wizard -------\r\n"
               "------------------------------------------------------------\r\n"
               "Enter Year:\r\n");

        while (uart2_ch == '\0'); 

        if (uart2_ch == 'q') {
            break;
        }
    }

    /* After initialization routine is finished, spawn tasks and delete current one. */

	status = xTaskCreate( (TaskFunction_t) task2_handler, "task1", STACK_SIZE, NULL, 1, &xHandle1);
	configASSERT(status == pdPASS);

	status = xTaskCreate( (TaskFunction_t) task3_handler, "task2", STACK_SIZE, NULL, 1, NULL);
	configASSERT(status == pdPASS);
    
    vTaskDelete(NULL);    
}



void task2_handler(void *args) {
    const TickType_t xDelay = pdMS_TO_TICKS(10000);  /* 10 second */
    int size = 27;
    char* pTimeStamp = (char*) pvPortMalloc(size*sizeof(char));
    int timeList[12] = {0};     /* init to 0 */

    status_t writeStatus = OK; 

    /* Digits for time/date */
    /* 0H:0M:0S  0D/0M/0Y */

    snprintf(pTimeStamp, size, "Time: %d%d:%d%d:%d%d  %d%d/%d%d/%d%d\r\n", timeList[0], timeList[1], 
             timeList[2], timeList[3], timeList[4], timeList[5], timeList[6], timeList[7],         
             timeList[8], timeList[9], timeList[10], timeList[11]);

	while(1) {
        gpio_on('E', 1);
        
        printf("%s", pTimeStamp);

        vTaskDelay(xDelay);

	}
    vPortFree(pTimeStamp);
}

void task3_handler (void *args) {
	while(1) {
        gpio_off('E', 1);
	}
}
/*-----------------------------------------------------------*/

/*----------------------------HOOKS--------------------------*/
/*-----------------------------------------------------------*/
//allows for debugging in the case of stack overflow.
void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
     * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();


	//do stuff in here to debug
    for( ; ; ) {
        gpio_toggle('B', 0);
        gpio_toggle('E', 1);
        gpio_toggle('B', 14);
        for (volatile int i = 0; i < 10000000; i++);
	}
    
}

/*-----------------------------------------------------------*/

