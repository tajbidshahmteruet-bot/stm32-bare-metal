#include <stdint.h>
#include "stm32h753xx.h"
#include <stdio.h>
#include "systick.h"
#include "usart3.h"
#include "adc.h"
#include "nn_model.h"

#define WINDOW_SIZE     100
#define WINDOW_DELAY_MS 1

void EXTI15_10_IRQHandler(void) { while(1); }
void HardFault_Handler(void) {
    printf("HARDFAULT!\r\n");
    while(1);
}

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

float relu(float x) {
    return x > 0.0f ? x : 0.0f;
}

float sigmoid(float x) {
    // Clamp to avoid overflow
    if (x >  6.0f) return 1.0f;
    if (x < -6.0f) return 0.0f;
    // Approximation using Taylor series
    float ex = 1.0f + x + x*x*0.5f + x*x*x*0.1667f +
               x*x*x*x*0.0417f + x*x*x*x*x*0.0083f;
    return ex / (1.0f + ex);
}

uint8_t nn_predict(uint32_t range, uint32_t std) {
    // Normalize inputs
    float x0 = ((float)range - NN_MEAN0) / NN_STD0;
    float x1 = ((float)std   - NN_MEAN1) / NN_STD1;

    // Hidden layer: 8 neurons with ReLU
    float h[8];
    for (int j = 0; j < 8; j++) {
        h[j] = relu(NN_W1[0][j] * x0 + NN_W1[1][j] * x1 + NN_B1[j]);
    }

    // Output layer: 1 neuron with sigmoid
    float z = NN_B2;
    for (int j = 0; j < 8; j++) {
        z += NN_W2[j] * h[j];
    }

    float prob = sigmoid(z);
    return (prob >= 0.5f) ? 1 : 0;
}

int main(void) {
    // Enable FPU
    SCB->CPACR |= (0xFU << 20);

    systick_init();
    usart3_init();
    printf("Boot OK\r\n");
    adc_init(15);
    printf("=== NN Anomaly Detector ===\r\n");

    while (1) {
        uint32_t range, std;
        compute_features(&range, &std);
        uint8_t result = nn_predict(range, std);

        if (result == 1) {
            printf("ANOMALY  range=%lu std=%lu\r\n", range, std);
        } else {
            printf("normal   range=%lu std=%lu\r\n", range, std);
        }
    }
}

