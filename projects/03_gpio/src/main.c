#include <stdint.h>
#include "stm32h753xx.h"

#define LED_Count 3

typedef struct {
    GPIO_TypeDef *port;
    uint8_t     pin;
    uint32_t    rcc_bit;
} LED;

LED leds[] = {
    {GPIOB, 0, RCC_AHB4ENR_GPIOBEN},
    {GPIOE, 1, RCC_AHB4ENR_GPIOEEN},
    {GPIOB, 14, RCC_AHB4ENR_GPIOBEN},
};


void delay(volatile uint32_t count){
    while(count --);
}

void GPIO_init(void){
    // Setting Clock sources for the ports
    for (uint8_t i = 0; i < LED_Count; i++){
        RCC->AHB4ENR |= leds[i].rcc_bit;
    }

    (void)RCC->AHB4ENR; // waiting for the clock for propagate
    // Setting Default LED Pins to Output

    for (uint8_t i = 0; i < LED_Count; i++){
        /* code */
        uint8_t pin = leds[i].pin;
        GPIO_TypeDef *port = leds[i].port;
        port->MODER &= ~(0x3U << (pin*2)); // Clear Moder
        port->MODER |= (0x1U << (pin*2));   // Seting output
        port->OTYPER &= ~(0x1U << pin); // push-pull
        port->PUPDR &= ~(0x3U << (pin * 2)); // no pull
    }

}


void LED_On(void){
    for(uint8_t i = 0; i < LED_Count; i++){
        leds[i].port->BSRR = (0x1U << leds[i].pin);
    }
}

void LED_Off(void){
    for (uint8_t i = 0; i < LED_Count; i++){
        /* code */
        leds[i].port->BSRR = (0x1U << (leds[i].pin + 16));
    }
}
void LED_Sequence(void){
    for (int8_t i = LED_Count - 1; i >= 0; i--){
        /* code */
        leds[i].port -> BSRR = (0x1U << leds[i].pin);
        delay(5000000);
        leds[i].port -> BSRR = (0x1U << (leds[i].pin + 16));
        delay(5000000);
    }
}
int main(void) {
    GPIO_init();
    while (1) {
        LED_Sequence();

    }
}
