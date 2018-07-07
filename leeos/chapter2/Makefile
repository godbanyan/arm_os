TOOLCHAIN_PREFIX=$(HOME)/arm_os/leeos_tools/bin
OBJCOPY = $(TOOLCHAIN_PREFIX)/arm-elf-objcopy 
LD = $(TOOLCHAIN_PREFIX)/arm-elf-ld
AS = $(TOOLCHAIN_PREFIX)/arm-elf-as 
CC = $(TOOLCHAIN_PREFIX)/arm-elf-gcc
C_FLAG = -c -nostdlib -nostartfiles -lgcc

TARGET=helloworld

skyeye: $(TARGET).bin
	skyeye

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	@if [ ! -f $(TARGET).bin ]; then \
		mv $@ $(TARGET).bin; \
	fi

%.elf: %.o
	$(LD) -e $(TARGET) -Ttext 0x0 $< -o $@

%.o: %.s
	$(AS) $< -o $@
%.o: %.c
	$(CC) $(C_FLAG) $< -o $@

.PHONY: clean
clean:
	rm -rf *.o *.elf *.bin
