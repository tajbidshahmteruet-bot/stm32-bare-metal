#include <stdint.h>
#include <stdio.h>
#include "stm32h753xx.h"
#include "usart3.h"
#include "systick.h"

void Timer_Init(void){
    // Enable Peripheral Clocks
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
    RCC->APB1LENR |= RCC_APB1LENR_TIM6EN;
    (void)RCC->AHB4ENR;
    // PB0 as output
    GPIOB->MODER &= ~(0x3U << 0);
    GPIOB->MODER |= (0x1U << 0);
    GPIOB->OTYPER &= ~(0x1U << 0);
    GPIOB->PUPDR  &= ~(0x3U << 0);
    // Prescaler and Auto-Reloader value setting for 1 sec
    TIM6->PSC = 6399;
    TIM6->ARR = 9999;
    // Enabling the timer interrup and counting
    TIM6->DIER |= (0x1U << 0); // UIE enabled
    TIM6->CR1  |= (0x1U << 0); // CEN bit set
    // NVIC enabling.
    NVIC_SetPriority(TIM6_DAC_IRQn, 5);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    
}
void TIM6_DAC_IRQHandler(void){
    TIM6->SR &= ~(0x1U << 0);
    static uint8_t led_state = 0;
    led_state ^= 1;
    if(led_state){
        GPIOB->BSRR = (0x1U << 0);
        printf("LED ON\r\n");
    }
    else{
        GPIOB->BSRR = (0x1U << 16);
        printf("LED OFF\r\n");
    }
    
}

int main(void) {
    Timer_Init();
    usart3_init();
    while (1) {

    }
}
