.syntax unified
.cpu cortex-m7
.fpu softvfp
.thumb

.global Reset_Handler
.global Default_Handler
.global SysTick_Handler
.global EXTI15_10_IRQHandler
.global TIM6_DAC_IRQHandler 

/* ── Interrupt Vector Table ─────────────────────────────────── */
.section .isr_vector, "a", %progbits
.word   _estack                     /* Stack top */
.word   Reset_Handler               /* Reset */
.word   Default_Handler             /* NMI */
.word   Default_Handler             /* HardFault */
.word   Default_Handler             /* MemManage */
.word   Default_Handler             /* BusFault */
.word   Default_Handler             /* UsageFault */
.word   0                           /* Reserved */
.word   0                           /* Reserved */
.word   0                           /* Reserved */
.word   0                           /* Reserved */
.word   Default_Handler             /* SVC */
.word   Default_Handler             /* DebugMon */
.word   0                           /* Reserved */
.word   Default_Handler             /* PendSV */
.word   SysTick_Handler             /* SysTick */
/* ── STM32H753 peripheral interrupts ────────────────────────── */
.word   Default_Handler             /* 0  WWDG */
.word   Default_Handler             /* 1  PVD */
.word   Default_Handler             /* 2  TAMP_STAMP */
.word   Default_Handler             /* 3  RTC_WKUP */
.word   Default_Handler             /* 4  FLASH */
.word   Default_Handler             /* 5  RCC */
.word   Default_Handler             /* 6  EXTI0 */
.word   Default_Handler             /* 7  EXTI1 */
.word   Default_Handler             /* 8  EXTI2 */
.word   Default_Handler             /* 9  EXTI3 */
.word   Default_Handler             /* 10 EXTI4 */
.word   Default_Handler             /* 11 DMA1_Stream0 */
.word   Default_Handler             /* 12 DMA1_Stream1 */
.word   Default_Handler             /* 13 DMA1_Stream2 */
.word   Default_Handler             /* 14 DMA1_Stream3 */
.word   Default_Handler             /* 15 DMA1_Stream4 */
.word   Default_Handler             /* 16 DMA1_Stream5 */
.word   Default_Handler             /* 17 DMA1_Stream6 */
.word   Default_Handler             /* 18 ADC1_2 */
.word   Default_Handler             /* 19 FDCAN1_IT0 */
.word   Default_Handler             /* 20 FDCAN2_IT0 */
.word   Default_Handler             /* 21 FDCAN1_IT1 */
.word   Default_Handler             /* 22 FDCAN2_IT1 */
.word   Default_Handler             /* 23 EXTI9_5 */
.word   Default_Handler             /* 24 TIM1_BRK */
.word   Default_Handler             /* 25 TIM1_UP */
.word   Default_Handler             /* 26 TIM1_TRG_COM */
.word   Default_Handler             /* 27 TIM1_CC */
.word   Default_Handler             /* 28 TIM2 */
.word   Default_Handler             /* 29 TIM3 */
.word   Default_Handler             /* 30 TIM4 */
.word   Default_Handler             /* 31 I2C1_EV */
.word   Default_Handler             /* 32 I2C1_ER */
.word   Default_Handler             /* 33 I2C2_EV */
.word   Default_Handler             /* 34 I2C2_ER */
.word   Default_Handler             /* 35 SPI1 */
.word   Default_Handler             /* 36 SPI2 */
.word   Default_Handler             /* 37 USART1 */
.word   Default_Handler             /* 38 USART2 */
.word   Default_Handler             /* 39 USART3 */
.word   Default_Handler        /* 40 EXTI15_10 */
.word   Default_Handler             /* 41 */
.word   Default_Handler             /* 42 */
.word   Default_Handler             /* 43 */
.word   Default_Handler             /* 44 */
.word   Default_Handler             /* 45 */
.word   Default_Handler             /* 46 */
.word   Default_Handler             /* 47 */
.word   Default_Handler             /* 48 */
.word   Default_Handler             /* 49 */
.word   Default_Handler             /* 50 */
.word   Default_Handler             /* 51 */
.word   Default_Handler             /* 52 */
.word   Default_Handler             /* 53 */
.word   TIM6_DAC_IRQHandler         /* 54 TIM6_DAC */

/* ── Reset Handler ──────────────────────────────────────────── */
.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:

    /* Copy .data section from Flash to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    b   copy_check
copy_loop:
    ldr r3, [r2], #4
    str r3, [r0], #4
copy_check:
    cmp r0, r1
    bcc copy_loop

    /* Zero out .bss section */
    ldr r0, =_sbss
    ldr r1, =_ebss
    mov r2, #0
    b   bss_check
bss_loop:
    str r2, [r0], #4
bss_check:
    cmp r0, r1
    bcc bss_loop

    /* Call main */
    bl  main

    /* If main returns, loop forever */
infinite_loop:
    b infinite_loop

/* ── Default Handler ────────────────────────────────────────── */
.section .text.Default_Handler
.type Default_Handler, %function
Default_Handler:
    b Default_Handler
