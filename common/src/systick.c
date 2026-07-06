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
void delay_us(uint32_t us) {
    uint32_t counts = us * 64;           // 64 counts per microsecond at 64MHz

    uint32_t start = SysTick->VAL;       // current counter value (counts down)
    uint32_t elapsed = 0;

    while (elapsed < counts) {
        uint32_t current = SysTick->VAL;

        // Handle counter wraparound (VAL reloads from LOAD when it hits 0)
        if (current <= start) {
            elapsed += start - current;
        } else {
            elapsed += start + (SysTick->LOAD - current) + 1;
        }

        start = current;
    }
}