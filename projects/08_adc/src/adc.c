#include "adc.h"
#include "systick.h"
#include "stm32h753xx.h"

void adc_init(uint8_t channel) {

    // Ensure D3 domain is in RUN mode as without this RCC_AHB1ENR_ADC12EN; cannot be enabled
    PWR->CPUCR &= ~PWR_CPUCR_PDDS_D3;
    while (!(PWR->D3CR & PWR_D3CR_VOSRDY));
    // ── 1. Bus clocks ─────────────────────────────────────────
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
    (void)RCC->AHB4ENR;                      // clock propagation barrier

    RCC->AHB1ENR |= RCC_AHB1ENR_ADC12EN;
    (void)RCC->AHB1ENR;                      // clock propagation barrier

    // ── 2. PA3 → analog mode, no pull ────────────────────────
    GPIOA->MODER  |=  (0x3U << (3 * 2));     // bits [7:6] = 11 → analog
    GPIOA->PUPDR  &= ~(0x3U << (3 * 2));     // bits [7:6] = 00 → no pull

    // ── 3. ADC kernel clock via per_ck (HSI = 64 MHz) ────────
    RCC->D1CCIPR  &= ~RCC_D1CCIPR_CKPERSEL;  // CKPERSEL = 00 → HSI
    RCC->D3CCIPR  &= ~RCC_D3CCIPR_ADCSEL;
    RCC->D3CCIPR  |=  (0x2U << RCC_D3CCIPR_ADCSEL_Pos); // ADCSEL = 10 → per_ck

    // ── 4. ADC prescaler /2 in common register ────────────────
    ADC12_COMMON->CCR &= ~ADC_CCR_PRESC;
    ADC12_COMMON->CCR |=  (0x2U << ADC_CCR_PRESC_Pos);  // /2 → 32 MHz

    // ── 5. Exit deep power down ───────────────────────────────
    ADC1->CR &= ~ADC_CR_DEEPPWD;

    // ── 6. Enable voltage regulator, wait 10us ────────────────
    ADC1->CR |= ADC_CR_ADVREGEN;
    delay_us(10);

    // ── 7. Calibration (single-ended) ─────────────────────────
    ADC1->CR &= ~ADC_CR_ADCALDIF;
    ADC1->CR |=  ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    // ── 8. Enable ADC, wait until ready ──────────────────────
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->ISR |= ADC_ISR_ADRDY;

    // ── 9. Channel 15 (PA3), sampling time 64.5 cycles ───────
    ADC1->SQR1  = ((uint32_t)channel << 6); // SQ1 = channel
    if(channel<=9){
        ADC1->SMPR1 = (0b011U << (3 * channel));
    }
    else{
        ADC1->SMPR2 = (0b011U << (3 * (channel - 10)));
    }
    // H7 specific: enable channel 15 in PCSEL register without this the ADC read does not start
    ADC1->PCSEL |= (1U << channel);
}

uint32_t adc_read(void) {
    ADC1->CR |= ADC_CR_ADSTART;
    while (!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}