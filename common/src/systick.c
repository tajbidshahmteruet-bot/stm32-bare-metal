#include "systick.h"
#include "stm32h753xx.h"

volatile uint32_t ticks = 0;

void SysTick_Handler(void) {
    ticks++;
}

void systick_init(void) {
    SysTick->LOAD = 64000 - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
                  | SysTick_CTRL_TICKINT_Msk
                  | SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(uint32_t ms) {
    uint32_t start = ticks;
    while ((ticks - start) < ms);
}

uint32_t millis(void) {
    return ticks;
}