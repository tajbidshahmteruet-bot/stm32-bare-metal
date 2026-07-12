#include <stdint.h>
#include "stm32h753xx.h"
#include <stdio.h>
#include "systick.h"
#include "usart3.h"
#include "adc.h"
#include "lr_model.h"

#define WINDOW_SIZE     100
#define WINDOW_DELAY_MS 1

void EXTI15_10_IRQHandler(void) { while(1); }

void compute_features(uint32_t *out_range, uint32_t *out_std) {
    static uint32_t samples[WINDOW_SIZE];
    uint32_t min = 65535, max = 0;
    uint64_t sum = 0;

    for (int i = 0; i < WINDOW_SIZE; i++) {
        samples[i] = adc_read();
        if (samples[i] < min) min = samples[i];
        if (samples[i] > max) max = samples[i];
        sum += samples[i];
        delay_ms(WINDOW_DELAY_MS);
    }

    uint32_t mean = (uint32_t)(sum / WINDOW_SIZE);

    uint64_t variance = 0;
    for (int i = 0; i < WINDOW_SIZE; i++) {
        int64_t diff = (int64_t)samples[i] - (int64_t)mean;
        variance += (uint64_t)(diff * diff);
    }
    variance /= WINDOW_SIZE;

    // Integer square root
    uint32_t std = 0;
    if (variance > 0) {
        uint64_t s = variance;
        uint64_t t = (s + 1) / 2;
        while (t < s) { s = t; t = (s + variance / s) / 2; }
        std = (uint32_t)s;
    }

    *out_range = max - min;
    *out_std   = std;
}

uint8_t lr_predict(uint32_t range, uint32_t std) {
    float f0 = ((float)range - LR_MEAN0) / LR_STD0;
    float f2 = ((float)std   - LR_MEAN2) / LR_STD2;
    float z  = LR_W0 * f0 + LR_W2 * f2 + LR_BIAS;
    return (z >= 0.0f) ? 1 : 0;
}
void HardFault_Handler(void) {
    // Read fault status registers
    uint32_t hfsr = SCB->HFSR;
    uint32_t cfsr = SCB->CFSR;
    uint32_t mmfar = SCB->MMFAR;
    uint32_t bfar  = SCB->BFAR;
    printf("HARDFAULT!\r\n");
    printf("HFSR: 0x%08lx\r\n", hfsr);
    printf("CFSR: 0x%08lx\r\n", cfsr);
    printf("MMFAR: 0x%08lx\r\n", mmfar);
    printf("BFAR:  0x%08lx\r\n", bfar);
    while(1);
}

int main(void) {
    // Enable FPU — set CP10 and CP11 to full access
    SCB->CPACR |= (0xFU << 20);
    systick_init();
    usart3_init();
    printf("Boot OK\r\n");
    adc_init(15);
    printf("=== LR Anomaly Detector ===\r\n");

    while (1) {
        uint32_t range, std;
        compute_features(&range, &std);
        uint8_t result = lr_predict(range, std);

        if (result == 1) {
            printf("ANOMALY  range=%lu std=%lu\r\n", range, std);
        } else {
            printf("normal   range=%lu std=%lu\r\n", range, std);
        }
    }
}