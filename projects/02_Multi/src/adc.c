#include "adc.h"
#include "systick.h"
#include "stm32h753xx.h"

void adc_init(void) {

    // Enable ADC1/2 clock on AHB1
    RCC->AHB1ENR |= RCC_AHB1ENR_ADC12EN;

    // Enable GPIOA clock (PA3 = ADC1_INP15)
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;

    // PA3 → analog mode (MODER = 11)
    GPIOA->MODER |= (0x3 << (3 * 2));

    // Disable pull-up/pull-down on PA3
    GPIOA->PUPDR &= ~(0x3 << (3 * 2));

    // Step 1 — Exit deep power down
    ADC1->CR &= ~ADC_CR_DEEPPWD;

    // Enable internal voltage regulator
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait TADCVREG_STUP = 10us
    delay_us(10);

    // Step 2 — Calibration (single-ended)
    ADC1->CR &= ~ADC_CR_ADCALDIF;
    ADC1->CR |=  ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    // Step 3 — Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->ISR |= ADC_ISR_ADRDY;

    // Step 4 — Configure channel 15, sampling time 64.5 cycles
    ADC1->SQR1  = (15 << 6);
    ADC1->SMPR2 = (0b011 << 15);
}
uint32_t adc_read(void) {

    // Start conversion
    ADC1->CR |= ADC_CR_ADSTART;

    // Wait for end of conversion
    while (!(ADC1->ISR & ADC_ISR_EOC));

    // Read result (clears EOC automatically)
    return ADC1->DR;
}