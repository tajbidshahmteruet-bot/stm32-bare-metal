#include <stdint.h>
#include "stm32h753xx.h"

void delay(volatile uint32_t count) {
    while(count--);
}

int main(void) {
    /* Enable clocks */
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
    (void)RCC->AHB4ENR;

    /* PB0 as output (LED) */
    GPIOB->MODER &= ~(0x3U << 0);
    GPIOB->MODER |=  (0x1U << 0);
    GPIOB->OTYPER &= ~(1U << 0);
    GPIOB->PUPDR  &= ~(0x3U << 0);

    /* PC13 as input, no pull needed (external pull-up on board) */
    GPIOC->MODER &= ~(0x3U << 26);

    uint8_t led_state = 0;
    uint8_t last_button = 1;   /* 1 = not pressed (active LOW) */

    while (1) {
        uint8_t current_button = (GPIOC->IDR & (1U << 13)) ? 1 : 0;

        /* Detect falling edge — button just pressed */
        if (last_button == 1 && current_button == 0) {
            delay(20000);   /* debounce — wait for bouncing to settle */

            /* Read again to confirm it was a real press */
            if (!(GPIOC->IDR & (1U << 13))) {
                led_state ^= 1;   /* toggle */
                if (led_state) {
                    GPIOB->BSRR = (1U << 0);    /* LED ON */
                } else {
                    GPIOB->BSRR = (1U << 16);   /* LED OFF */
                }
            }
        }

        last_button = current_button;
    }
}