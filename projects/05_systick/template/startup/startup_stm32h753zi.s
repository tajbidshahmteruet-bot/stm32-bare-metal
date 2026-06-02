.syntax unified
.cpu cortex-m7
.fpu softvfp
.thumb

.global Reset_Handler
.global Default_Handler

/* ── Interrupt Vector Table ─────────────────────────────────── */
.section .isr_vector, "a", %progbits
.word   _estack                 /* Stack top */
.word   Reset_Handler           /* Reset */
.word   Default_Handler         /* NMI */
.word   Default_Handler         /* HardFault */
.word   Default_Handler         /* MemManage */
.word   Default_Handler         /* BusFault */
.word   Default_Handler         /* UsageFault */
.word   0                       /* Reserved */
.word   0                       /* Reserved */
.word   0                       /* Reserved */
.word   0                       /* Reserved */
.word   Default_Handler         /* SVC */
.word   Default_Handler         /* DebugMon */
.word   0                       /* Reserved */
.word   Default_Handler         /* PendSV */
.word   Default_Handler         /* SysTick */

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
