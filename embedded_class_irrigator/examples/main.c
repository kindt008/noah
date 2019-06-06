#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SET_BIT(port, pin) (port |= (1<<pin))
#define CLEAR_BIT(port, pin) (port &= ~(1<<pin))
#define TOGGLE_BIT(port, pin) (port ^= (1<<pin))

#define CONFIG_OUTPUT(port, pin) (SET_BIT(port,pin))
#define CONFIG_INPUT(port,pin) (CLEAR_BIT(port,pin))

int main(void) {
  USBCON = 0;
  SET_BIT(DDRD, DDD6);  // power to port 12

}
