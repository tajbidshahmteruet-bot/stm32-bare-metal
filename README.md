# STM32H753ZI Bare-Metal Programming

Learning embedded systems programming on the STM32H753ZI (Nucleo-144 board)
without HAL — directly accessing hardware registers from scratch.

## Hardware
- Board: NUCLEO-H753ZI
- MCU: STM32H753ZI (ARM Cortex-M7, 480MHz)
- Programmer: Onboard ST-Link V3

## Toolchain (Linux Ubuntu)
- Compiler: arm-none-eabi-gcc
- Debugger: gdb-multiarch
- Flasher: OpenOCD
- Editor: Sublime Text

## Projects

| Project | Description |
|---------|-------------|
| 01_blink | Bare metal LED blink using GPIO registers and BSRR |
| 02_button | User button input with debounce and edge detection |
| 03_gpio | Multiple LEDs with struct array and for loop init |

## Reference Documents
- RM0433 — STM32H753ZI Reference Manual
- UM2407 — Nucleo-144 User Manual
- ARM Cortex-M7 Generic User Guide (DUI0646)

## Key Concepts Learned
- Memory-mapped I/O
- RCC clock enable registers
- GPIO MODER, OTYPER, PUPDR, ODR, BSRR registers
- Active LOW button logic
- Struct arrays for hardware descriptors