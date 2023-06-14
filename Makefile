MAIN_FILE = src/main
DEVICE = attiny84
PROGRAMMER = USBasp
CC = avr-gcc
CFLAGS = -Wall -I inc -Os -mmcu=$(DEVICE)
FUSES = -U lfuse:w:0x5f:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m 

default: install size clean

install: $(MAIN_FILE).hex
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -B 10 $(FUSES) -U flash:w:$(MAIN_FILE).hex:i

$(MAIN_FILE).hex: $(MAIN_FILE).elf
	avr-objcopy -j .text -j .data -O ihex $(MAIN_FILE).elf $(MAIN_FILE).hex

$(MAIN_FILE).elf: *.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^

size:
	avr-size --format=avr --mcu=$(DEVICE) $(MAIN_FILE).elf

clean:
	rm *.o
	rm src/*.elf
	rm src/*.hex
