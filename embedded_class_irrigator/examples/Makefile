PORT=COM4

ifneq ($(filter program,$(MAKECMDGOALS)),$())
ifndef PORT
$(error PORT must be set to program device)
endif
endif

MCU=atmega32u4
CFLAGS+= -g -w -mcall-prologues -mmcu=$(MCU) -Os -std=c99
LDFLAGS+= -Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=main
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
LIBS = -lm

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex
