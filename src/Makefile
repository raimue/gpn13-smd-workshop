.PHONY: all clean flash gdbstub

# Target file name without file extension
TARGET  = ledtest

# Compiler toolchain
CC      = avr-gcc
SIZE    = avr-size
OBJCOPY = avr-objcopy
GDB     = avr-gdb

# AVR Tools for flashing and debugging
AVARICE = avarice
AVRDUDE = avrdude

# Port for the GDB stub
GDBPORT = 4242

# MCU settings
#MCU = atmega8
#MCU = atmega32
#MCU = attiny13
#MCU = attiny2313
#MCU = attiny85
MCU = attiny9

# ISP device type and flags
#ISPDEV = -c usbtiny -P usb
ISPDEV = -c avrispmkII -P usb
#ISPBCLK = -B 8

# JTAG debugger device type and flags
# JTAG ICE mkI
#JTAGDEV = -1 -j /dev/avrjtag
# JTAG ICE mkII
JTAGDEV = -2 -j usb
# AVR Dragon
#JTAGDEV = -g -j usb
# JTAG frequency
JTAGFREQ = -B '125 kHz'

# CFLAGS for MCU
CFLAGS += -mmcu=$(MCU)
CFLAGS += -DF_CPU=8000000UL
# CFLAGS += -DF_CPU=1000000UL

# CFLAGS for language and optimization tuning
CFLAGS += -std=c99
CFLAGS += -ffreestanding
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# Warning: this might break linkage of additional files
#CFLAGS += -fwhole-program
CFLAGS += -Wall -Werror

# Use appropriate flags for debugging or for optimized code
#OPT = -g -O0
OPT = -Os
CFLAGS += $(OPT)

all: $(TARGET).hex

flash: $(TARGET).hex.flash

# Target for flashing an Intel hex image
%.hex.flash: %.hex
	$(AVRDUDE) $(ISPDEV) $(ISPBCLK) -p $(MCU) -U flash:w:$<

# Target for flashing an ELF image
%.elf.flash: %.elf
	$(AVARICE) $(JTAGDEV) $(JTAGFREQ) -P $(MCU) -f $< --erase -p

# Start debugging stub for gdb
gdbstub:
	$(AVARICE) $(JTAGDEV) $(JTAGFREQ) -P $(MCU) localhost:$(PORT)

# Auto-detect connected device and use appropriate command,
# only works on Linux due to use of lsusb.
#%.hex.flash: %.hex
#	@if [ -n "`lsusb | grep 03eb:2103`" ]; then \
#		$(AVARICE) $(JTAGDEV) -B $(JTAGFREQ) -P $(MCU) -f $< --erase -p ; \
#	elif [ -n "`lsusb | grep 03eb:2104`" ]; then \
#		$(AVRDUDE) $(ISPDEV) $(ISPBCLK) -p $(MCU) -U flash:w:$< ; \
#	else \
#		echo '***********************************' ; \
#		echo '* No ISP or JTAG device detected! *' ; \
#		echo '***********************************' ; \
#		exit 1 ; \
#	fi

%.flash: %.hex.flash

%.elf.debug: %.elf
	$(GDB) $<

%.debug: %.elf.debug

# Not all versions of avrdude can handle ELF images, convert them to Intel hex format
%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.elf: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(SIZE) $@

clean:
	$(RM) $(TARGET).hex
	$(RM) $(TARGET).eep
	$(RM) $(TARGET).obj
	$(RM) $(TARGET).cof
	$(RM) $(TARGET).elf
	$(RM) $(TARGET).map
	$(RM) $(TARGET).obj
	$(RM) $(TARGET).a90
	$(RM) $(TARGET).sym
	$(RM) $(TARGET).lnk
	$(RM) $(TARGET).lss
	$(RM) $(TARGET).o
