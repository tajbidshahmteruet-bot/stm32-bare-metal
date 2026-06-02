#include <stdint.h>
#include "stm32h753xx.h"
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

void usart3_init(void){ /* Usart Initialization */ 
    /* 1. Enabling the Clocks */
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIODEN; // as bit 3 is set, GPIOD clock enabled 
    RCC->APB1LENR |= RCC_APB1LENR_USART3EN; // as bit 18 is set, USART3 Clock enabled 
    (void)RCC->APB1LENR; // Waiting for clock propagation

    /* 2. Configure PD8 as AF7 (physically connects to (USART3_TX)) */
    GPIOD->MODER &= ~(0x3U << 16); // Cleared the 16th and 17th bit in GPIOD_moder register
    GPIOD->MODER |= (0x2U << 16); // Stting pin8 in alternate function mode writing 10 at bit 16 and 17
    GPIOD->AFR[1] &= ~(0xFU << 0); // Clearing the AFR8 in GPIOD_AFRH
    GPIOD->AFR[1] |= (0x7 << 0);//setting the Pin8 as Tx for USART3 by writing value AF7 at bits [3:0] 
    /* 3. Configure PD9 as AF7 (USART3_RX)*/
    GPIOD->MODER &= ~(0x3U << 18); // Cleared the 18th and 19th bit in GPIOD_moder register
    GPIOD->MODER |= (0x2U << 18);  // Stting pin9 in alternate function mode writing 10 at bit 18 and 19
    GPIOD->AFR[1] &= ~(0xFU << 4); // Clearing the AFR9 in GPIOD_AFRH
    GPIOD->AFR[1] |= (0x7U << 4); //setting the Pin9 as Rx for USART3 by writing value AF7
    /* 4. Set Baud Rate
     * BRR = kernel clock / baud rate
     * HSI = 64MHZ, baud rate = 115200
     * BRR = 64000000 / 115200
    */
    USART3->BRR = 555;

    /* 5. Enable FIFO, TX, RX, and USART */
    USART3->CR1 = 0; // Set all bits to 0
    USART3->CR1 |= USART_CR1_FIFOEN; // FIFO enable [Bit:19]
    USART3->CR1 |= USART_CR1_TE; // TX enable [Bit:3]
    USART3->CR1 |= USART_CR1_RE; // RX enable [Bit:2]
    USART3->CR1 |= USART_CR1_UE; // USART enable [Bit:0], always enable last
    
}
void usart3_send_char(char c){
    /* We have to wait until the TXE: Transfer Data Register is empty[TXE=1], 
    and TXFNF: TXFIFO not full [TXFNF=1]*/
    while(!(USART3->ISR & USART_ISR_TXE_TXFNF));
    USART3->TDR = c;
}

void usart3_send_string(const char *str){
    while(*str){
        usart3_send_char(*str++);
    }

}
int _write(int file, char *ptr, int len){
    (void)file;
    for (int i = 0; i < len; i++){
        usart3_send_char(ptr[i]);
    
    }
    return len;

}

int main(void) {
    usart3_init();
    //usart3_send_string("Hello from STM32H753ZI!...\r\n");
    printf("Hello from STM32H753ZI!\r\n");
    printf("Bare Metal printf() working!\r\n");

    uint32_t counter = 0;
    while (1) {
        //usart3_send_string("Bare metal UART running...\r\n");
        printf("Counter: %lu\r\n", counter++);
        for (volatile uint32_t i = 0; i < 1000000; ++i);

    }
}
