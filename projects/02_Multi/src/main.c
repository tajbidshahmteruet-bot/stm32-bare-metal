#include <stdint.h>
#include "stm32h753xx.h"
#include <stdio.h>
#include "systick.h"
#include "usart3.h"
#include "adc.h"

int main(void) {
    systick_init();
    usart3_init();
    adc_init();

    while (1) {
        uint32_t sample = adc_read();
        printf("ADC: %lu\r\n", sample);
        delay_ms(100);
    }
}
