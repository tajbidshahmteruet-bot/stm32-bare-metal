#include <stdio.h>
#include <stdint.h>
#include "stm32h753xx.h"
#include "usart3.h" 

/*------- SysTick counter------------------------------------*/
volatile uint32_t ticks = 0; /* Increments every 1ms*/

/* ------- SysTick ISR--------------------------------------*/
void SysTick_Handler(void){
    ticks++;
}
/*------- Delay --------------------------------------------*/
void delay_ms(uint32_t ms){
    uint32_t start = ticks;
    while((ticks - start) < ms);
}

uint32_t millis(void){
    return ticks;
}

void systick_init(void){
    SysTick->LOAD = 64000-1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
                    | SysTick_CTRL_TICKINT_Msk
                    | SysTick_CTRL_ENABLE_Msk;
}

int main(void) {
    systick_init();
    usart3_init();

    printf("SysTick Initialized!\r\n");

    while (1) {
        printf("[%6u ms] Hello!\r\n", (unsigned int)millis());
        delay_ms(1000);

    }
}
