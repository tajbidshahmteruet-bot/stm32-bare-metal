MEMORY
{
    FLASH   (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    DTCMRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    RAM     (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    SRAM1   (rw)  : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2   (rw)  : ORIGIN = 0x30020000, LENGTH = 128K
    SRAM4   (rw)  : ORIGIN = 0x38000000, LENGTH = 64K
}

ENTRY(Reset_Handler)

_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);

SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    .text :
    {
        *(.text*)
        *(.rodata*)
    } > FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM AT > FLASH

    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM
}
