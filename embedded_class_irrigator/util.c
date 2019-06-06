#include "common.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

uint8_t valve(uint8_t cmd) {
  if (cmd) {  // open the valve close is else

  } else {  // close the valve instead of opening

  }
  return 0;
}

uint64_t get_ms_ticks(){
  uint64_t temp;
  cli();
  temp = ms_ticks;
  sei();
  return temp;
}

void reset_us() {
  cli();
  us_ticks = 0;
  sei();
}
uint64_t get_us_ticks(){
  uint64_t temp;
  cli();
  temp = us_ticks;
  sei();
  return temp;
}

uint64_t get_s_ticks(){
  uint64_t temp;
  cli();
  temp = s_ticks;
  sei();
  return temp;
}

void sendChar(char c) {
	while((UCSR1A & (1<<UDRE1)) == 0);	// wait while data register is NOT empty
	UDR1 = c;
}

void sendString(char *s) {
	while(*s != 0x00) {
		sendChar(*s);
		s++;
	}
	sendChar('\r');
}

void setupUART(void) {
	UBRR1 = ((F_CPU/(16*38400)) - 1);

	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);		// 8 bit char size
	UCSR1B |= (1 << TXEN1);		// enable transmit

	UCSR1B |= (1 << RXEN1);     // enable receive
	UCSR1B |= (1 << RXCIE1);     // enable interrupt on data receipt
}

void handleInput() {
	// This function is called from main, but "released" in the ISR.
	// The ISR sets user_command_ready=1, main watches for the flag.

}

void printMenu() {
	sendString("(R)eference position set, (Z)ero out encoder, (V)iew values, (S)end saved data\n\r");
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
/*	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	uint8_t stateMask = (state ? bit : 0);
	unsigned long width = 0; // keep initialization out of time critical area

	// convert the timeout from microseconds to a number of times through
	// the initial loop; it takes 16 clock cycles per iteration.
	unsigned long numloops = 0;
	unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;

	// wait for any previous pulse to end
	while ((*portInputRegister(port) & bit) == stateMask)
		if (numloops++ == maxloops)
			return 0;

	// wait for the pulse to start
	while ((*portInputRegister(port) & bit) != stateMask)
		if (numloops++ == maxloops)
			return 0;

	// wait for the pulse to stop
	while ((*portInputRegister(port) & bit) == stateMask) {
		if (numloops++ == maxloops)
			return 0;
		width++;
	}

	return clockCyclesToMicroseconds(width * 21 + 16); */
}
