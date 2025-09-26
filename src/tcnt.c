/* 
Author: Gabriel Rodgers <gabrielrodgers4i@gmail.com>
Date: 6/9/2025
*/

#include <stdint.h>

#include "stm32h723xx.h"
#include "tcnt.h"
#include "main.h"

/*
 * timer3_pwm_init() - function to initialize timer3 for pwm
*/
void timer3_pwm_init(void) {
    RCC->APB1LENR |= RCC_APB1LENR_TIM3EN; //enable TIM3 clk

    TIM3->CR1 |= TIM_CR1_ARPE; //auto-reload preload

    TIM3->CCER |= (TIM_CCER_CC3E); //enable capture/compare 3 output
    TIM3->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2); //pwm mode 1 for ch3
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE; //enable preload for ch3

    TIM3->ARR = MAX_PWM_ONE_BYTE; //CNT reloads once it reaches this value
    TIM3->CCR3 = MIN_PWM_ONE_BYTE;  //ch3 is set to 0/1 depending on if CNT is less than or greater than this value

    TIM3->EGR |= TIM_EGR_UG;    //update the counter registers
    TIM3->CR1 |= TIM_CR1_CEN;   //enable counter
}

/*
 * timer12_pwm_init() - function to initialize timer12 for pwm
*/
void timer12_pwm_init(void) {
    RCC->APB1LENR |= RCC_APB1LENR_TIM12EN; //enable TIM12 clk

    TIM12->CR1 |= TIM_CR1_ARPE; //auto-reload preload
    TIM12->CCER |= (TIM_CCER_CC1E); //enable capture/compare 1 output
    TIM12->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2); //pwm mode 1 for ch1
    TIM12->CCMR1 |= TIM_CCMR1_OC1PE; //enable preload for ch1

    TIM12->ARR = MAX_PWM_ONE_BYTE; //CNT reloads once it reaches this value
    TIM12->CCR1 = MIN_PWM_ONE_BYTE;  //ch3 is set to 0/1 depending on if CNT is less than or greater than this value

    TIM12->EGR |= TIM_EGR_UG;    //update the counter registers
    TIM12->CR1 |= TIM_CR1_CEN;   //enable counter
}

/*
 * timer3_upcount_init() - function to initialize timer3 for up-counting
*/
void timer3_upcount_init(void) {
    RCC->APB1LENR |= RCC_APB1LENR_TIM3EN; //enable TIM3 clk

    TIM3->CR1 |= TIM_CR1_ARPE; //auto-reload preload
    TIM3->CR1 |= TIM_CR1_URS; //only allow under/overflow interrupts
    
    TIM3->ARR = 0xFFFF; //ONE_SECOND_10KHZ; //CNT reloads once it reaches this value
    TIM3->PSC = 63999; //counter runs at 10KHz

    TIM3->EGR |= TIM_EGR_UG;    //update the counter registers
    TIM3->CR1 |= TIM_CR1_CEN;   //enable counter
}

/*
 * timer3_blocking_delay() - function to create a blocking delay
*/
void timer3_blocking_delay(const uint32_t delay) {
    TIM3->ARR = delay; //CNT reloads once it reaches this value
    TIM3->EGR |= TIM_EGR_UG;    //update the counter registers
    TIM3->CNT = 0;
    while (TIM3->CNT < delay); 
}


/*
 * pwm_set_duty() - function to set the duty cycle of a timer's PWM signal
*/
void pwm_set_duty(uint8_t timer_num, uint8_t duty) {
    switch(timer_num) {
        case(3):
            TIM3->CCR3 = duty;
            break;
        case(12):
            TIM12->CCR1 = duty;
            break;
        default:
            break;
    }
}

