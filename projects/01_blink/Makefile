# ── Change these per project ──────────────────────────────────
TARGET = blink
SRCS   = src/main.c

# ── Toolchain ─────────────────────────────────────────────────
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# ── Paths ─────────────────────────────────────────────────────
WORKSPACE = ../..
CMSIS_DIR = $(WORKSPACE)/cmsis

STARTUP = startup/startup_stm32h753zi.s
LINKER  = linker/STM32H753ZI.ld

# ── Flags ─────────────────────────────────────────────────────
CFLAGS  = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS += -DSTM32H753xx -DHSE_VALUE=8000000UL
CFLAGS += -I$(CMSIS_DIR)
CFLAGS += -Iinclude
CFLAGS += -O0 -g3
CFLAGS += -Wall -Wextra -Wno-unused-parameter
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -std=c11

LDFLAGS  = -T$(LINKER)
LDFLAGS += -nostartfiles
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(TARGET).map
LDFLAGS += -Wl,--print-memory-usage

# ── Build ─────────────────────────────────────────────────────
ALL_SRCS = $(STARTUP) $(SRCS)

all: $(TARGET).bin size

$(TARGET).elf: $(ALL_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	

size: $(TARGET).elf
	$(SIZE) $<

# ── Flash ─────────────────────────────────────────────────────
flash: $(TARGET).bin
	openocd -f interface/stlink.cfg \
	        -f target/stm32h7x.cfg \
	        -c "program $(TARGET).bin 0x08000000 verify reset exit"

# ── Debug ─────────────────────────────────────────────────────
debug: $(TARGET).elf
	openocd -f interface/stlink.cfg \
	        -f target/stm32h7x.cfg \
	        -c "program $(TARGET).elf verify reset halt"

gdb: $(TARGET).elf
	gdb-multiarch $< \
	    -ex "target remote :3333" \
	    -ex "monitor reset halt"

# ── Clean ─────────────────────────────────────────────────────
clean:
	rm -f $(TARGET).elf $(TARGET).bin $(TARGET).map

.PHONY: all flash debug gdb clean size
