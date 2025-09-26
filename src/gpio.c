/* 
Author: Gabriel Rodgers <gabrielrodgers4i@gmail.com>
Date: 6/5/2025
*/

#include <stdint.h>

#include "stm32h723xx.h"
#include "gpio.h"
#include "tcnt.h"
#include "exti.h"

/*
 * gpio_button_init() - enable PC13 to be used as a button
*/
void gpio_button_init(void){
    /* enable clocks */
    RCC->AHB4ENR |= (1 << 2); //enable GPIOC clk
    
    GPIOC->MODER &= ~(0x3 << 26); //input mode for PC13
	GPIOC->PUPDR |= (1 << 27); //pull down R for PC13

    /* set up EXTI  for interrupt on button */
    exti_user_button_init();
}

/*
 * gpio_led_init() - set up 3 leds for use as on/off or PWM
*/
void gpio_led_init(GPIO_mode_t MODE){
    /* enable clocks */
    RCC->AHB4ENR |= (1 << 1); //enable GPIOB clk
    RCC->AHB4ENR |= (1 << 4); //enable GPIOE clk

    switch(MODE) {
        case(OUTPUT):
            GPIOB->MODER &= ~(1 << 1); //output mode for PB0
            GPIOB->MODER &= ~(1 << 29); //output mode for PB14
            GPIOE->MODER &= ~(1 << 3); //output mode for PE1
            break;
        case(PWM):
            GPIOB->MODER &= ~(1 << 0); //alt func for PB0
            GPIOB->MODER &= ~(1 << 28); //alt func for PB14

            GPIOB->AFR[0] |= GPIO_AFRL_AFSEL0_1; //TIM3_CH3, AF2
            GPIOB->AFR[1] |= GPIO_AFRH_AFSEL14_1; //TIM12_CH2, AF2  

            GPIOE->MODER &= ~(1 << 3); //output mode for PE1
            break;
        default:
            break;
    }
}

/*
 * gpio_pwm_set_duty() - A wrapper for pwm_set_duty()
*/
void gpio_pwm_set_duty(char port, uint8_t pin, uint8_t duty) {
    switch(port) {
        case('B'):
            switch(pin) {
                case(0):
                    pwm_set_duty(TIMER3, duty);
                    break;
                case(1): case(2): case(3): case(4):
                case(5): case(6): case(7): case(8):
                case(9): case(10): case(11): case(12):
                case(13):
                    break;
                case(14):
                    pwm_set_duty(TIMER12, duty);
                    break;
                case(15):
                default:
                    break;
            }
            break;
        case('C'):
        case('E'):
        default:
            break;
    }
}

/*
 * gpio_toggle() - toggle a GPIO pin's state
*/
void gpio_toggle(char port, uint8_t pin){
    switch(port) {
        case('B'):
            GPIOB->ODR ^= (1 << pin);
            break;
        case('C'):
            GPIOC->ODR ^= (1 << pin); 
            break;
        case('E'):
            GPIOE->ODR ^= (1 << pin); 
            break;
        default:
            break;
    }
}

/* 
 * gpio_on() - turn a GPIO pin on
*/
void gpio_on(char port, uint8_t pin) {
    switch(port) {
        case('B'):
            GPIOB->BSRR |= (1 << pin);
            break;
        case('C'):
            GPIOC->BSRR |= (1 << pin); 
            break;
        case('E'):
            GPIOE->BSRR |= (1 << pin); 
            break;
        default:
            break;
    }
}

/* 
 * gpio_on() - turn a GPIO pin off 
*/
void gpio_off(char port, uint8_t pin) {
    switch(port) {
        case('B'):
            GPIOB->BSRR |= (1 << (pin + 16));
            break;
        case('C'):
            GPIOC->BSRR |= (1 << (pin + 16)); 
            break;
        case('E'):
            GPIOE->BSRR |= (1 << (pin + 16)); 
            break;
        default:
            break;
    }
}

