#include <stdint.h>
#include "stm32h753xx.h"
void exti_init(void)
{
    // 1. Enable clocks -> GPIOC, SYSFG
    RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
    (void)RCC->AHB4ENR;
    // 2. PC13 as input -> MODER = 0
    GPIOC->MODER &= ~(0x3U << 26);
    GPIOC->PUPDR &= ~(0x3U << 26); // No pull-up, pull-down mode
    //PB0 as output
    GPIOB->MODER &= ~(0x3U << 0); // Clearing the bits
    GPIOB->MODER |= (0x1U << 0);    // 01 for GP output mode
    GPIOB->OTYPER &= ~(0x1U << 0); // Output push pull
    GPIOB->PUPDR &= ~(0x3U << 0); // No pull-up or No pull-down
    // 3. SYSCFG -> connect Port C to EXTI line 13
    SYSCFG->EXTICR[3] &= ~(0xFU << 4); // as EXTICR is defined as array in stm32h753xx.h file
    SYSCFG->EXTICR[3] |= (0x2U << 4);
    // 4 EXTI -> FTSR1 bit 13, CPUIMR1 bit 13 
    EXTI->FTSR1 |= EXTI_FTSR1_TR13_Msk; // Falling edge enabled
    EXTI->IMR1 |= EXTI_IMR1_IM13_Msk; // Unmasking the the PC13
    // 5. NVIC -> priority and enable EXTI15_10_IRQn
    NVIC_SetPriority(EXTI15_10_IRQn, 5);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    

}

int main(void) 
{
    exti_init();

    while (1) 
    {

    }
}
// 6. ISR -> Check PR1, clear PR1, toggle LED
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR1 & EXTI_PR1_PR13_Msk)
    {
        EXTI->PR1 |= EXTI_PR1_PR13_Msk;
        // Never use delay inside ISR routine.
        GPIOB->ODR ^= (0x1U << 0);

    }
        
    }