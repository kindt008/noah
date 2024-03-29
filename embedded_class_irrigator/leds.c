#include "leds.h"

void initialize_leds() {
  INIT_YELLOW;
  INIT_GREEN;
  INIT_RED;
}

// Syntax for using struct to access registers inspired by
// http://www.avrfreaks.net/forum/io-ports-mapped-struct
void flash_led(IO_struct * color, int inverted) {
  if (!inverted) {
    SET_BIT(*color->port, color->pin);
  } else {
    CLEAR_BIT(*color->port, color->pin);
  }
  _delay_ms(200);
  TOGGLE_BIT(*color->port, color->pin);
  _delay_ms(200);
}

void light_show() {
  // Flash them all to ensure they are working.
  int i;
  for (i = 0; i < 2; i++) {
		flash_led(&_yellow, 0);
		flash_led(&_red, 1);
		flash_led(&_green, 1);
  }
}

void GPIOshow() {
  // First GPIO is purely for error checking
  PORTB |= (1<<PB5);
  _delay_ms(1000);
  PORTB |= ~(1<<PB5);
  PORTB |= (1<<PB6);
  _delay_ms(1000);
  PORTB &= ~(1<<PB6);
  PORTC |= (1<<PC6);
  _delay_ms(1000);
  PORTC &= ~(1<<PC6);
  PORTD |= (1<<PD6);
  _delay_ms(1000);
  PORTD &= ~(1<<PD6);

}
