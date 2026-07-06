#include <stdint.h>
#include "stm32h753xx.h"
#include <stdio.h>
#include "systick.h"
#include "usart3.h"
#include "adc.h"

void EXTI15_10_IRQHandler(void) { while(1); }

int main(void) {
    systick_init();
    usart3_init();
    adc_init();

    while (1) {
        uint32_t min = 65535, max = 0;

        // collect 100 samples over ~100ms
        for (int i = 0; i < 100; i++) {
            uint32_t s = adc_read();
            if (s < min) min = s;
            if (s > max) max = s;
            delay_ms(1);
        }

        printf("min=%lu max=%lu range=%lu\r\n", min, max, max - min);
    }
}