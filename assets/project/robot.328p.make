PROJECT=robot.328p
SOURCES=$(PROJECT).c
MMCU= atmega328p
F_CPU = 20000000

CFLAGS=-mmcu=$(MMCU) -Wall -Os -DF_CPU=$(F_CPU)

$(PROJECT).hex: $(PROJECT).out
	avr-objcopy -O ihex $(PROJECT).out $(PROJECT).c.hex;\
	avr-size --mcu=$(MMCU) --format=avr $(PROJECT).out

$(PROJECT).out: $(SOURCES)
	avr-gcc $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)
    
program-usbtiny: $(PROJECT).hex
	avrdude -p atmega328p -P usb -c usbtiny -U flash:w:$(PROJECT).c.hex
    
program-usbtiny-fuses: $(PROJECT).hex
	avrdude -p atmega328p -P usb -c usbtiny -U lfuse:w:0xd6:m 
