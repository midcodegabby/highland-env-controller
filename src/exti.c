/* 
Author: Gabriel Rodgers <gabrielrodgers4i@gmail.com>
Date: 6/5/2025
*/

#include <stdint.h>

#include "stm32h723xx.h"
#include "exti.h"
#include "gpio.h"
#include "nvic.h"
#include "main.h"

extern const GPIO_mode_t led_mode;
extern volatile button_state_t button; 

/* 
 * exti_init() - function to initialize exti 
*/
void exti_init(void) {
    /* enable syscfg clock */
    RCC->APB4ENR |= (1 << 1); //enable SYSCFG clk

    SYSCFG->EXTICR[3] |= (0x2 << 4); //set EXTI line 13 to PC13
	EXTI->EMR1 |= (1 << 13); //unmask event line 13
	EXTI->RTSR1 |= (1 << 13); //enable rising edge trigger for line 13
	EXTI->IMR1 |= (1 << 13); //unmask interrupt line 13
}

/* 
 * EXTI_15_10_IRQHandler() - IRQ to handle exti lines [15-10] interrupts
*/
void EXTI15_10_IRQHandler(void) {
    EXTI->IMR1 &= ~(1 << 13); //mask interrupts and events from line 13
	EXTI->EMR1 &= ~(1 << 13);

	button = PRESSED;

	NVIC->ICPR[1] |= (1 << 8); //clear pending interrupt 40
	EXTI->PR1 |= (1 << 13); //clear pending interrupts on line 13
	EXTI->IMR1 |= (1 << 13); //unmask interrupt line 13
	EXTI->EMR1 |= (1 << 13); //unmask event line 13
}
