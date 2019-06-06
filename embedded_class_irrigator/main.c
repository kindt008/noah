#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "seesaw/Adafruit_seesaw.h"
#include <Adafruit_seesaw_Library/Adafruit_seesaw.h>

#include "timers.h"
#include "buttons.h"

void initialize_system() {
  initialize_buttons();
  initialize_leds();
  SetUpTimerCTC(0, 64, 1);  // ms_ticks timer
  setUpTimer1PWM(MICROSEC);
  // step to ensure valve is close

  cur_distance_cm = 0;
  DDRD |= (1<<DDD6);  // power supply to 5v distance sensor, GPIO, and soil sensor
  pinging = 0;
  if(!ss.begin(0x36)) {  // setting up soil sensor
    exit(1);
  }
}

int main(void) {
  USBCON = 0;
  initialize_system();
  // sendString("STARTING\n\r");
  openvalve = 0; closevalve = 0; is_open = 0;
  PORTB = 0;
  DDRB |= (1<<DDB7) | (1<<DDB6);
  sendString("START\n\r");
  sei();
  while (1) {
    // check if open water spout flag has been set;
    if (openvalve && !is_open) {
      // function to communicate with ports set up with the valve
      // code for setting up valve pre while(1)
      // valve(1);

      is_open = 1;
      openvalve = 0;
    }
    // check if close water spout flag has been set

    uint64_t arr[5];
    // check the water level sensor, update related values
    for (int i=0; i<5; i++) {
      uint64_t distance_us;
      uint64_t start_ping = get_ms_ticks();
      uint64_t end_ping;
      if(start_ping%10 == 0) {
        end_ping = start_ping+2;
        while(!(PORTB & 1<<PB5)) {
          if(get_ms_ticks()==end_ping) {
            end_ping++;
            break;
          }
        }
      }
      CLEAR_BIT(DDRB, DDB4);
      usleep(2);
      SET_BIT(DDRB, DDB4);
      usleep(10);
      CLEAR_BIT(DDRB, DDB4);
//      sendString("REACHED\n\r");
      TIMSK1 |= (1<<OCIE1A);
      distance_us = 2;
//      reset_us();
      while(get_ms_ticks()!=end_ping) {
        distance_us+=3;
        if(PORTB & (1<<PB5)) { // echo received
//          distance_us = get_us_ticks();
          distance_us -= (distance_us/8);  // closest dynamic equation to evaluate microseconds based on machine ops I could derive
          start_ping = 0;
//          sendString("PINGED\n\r");
          break;
        }
        TIMSK1 &= ~(1<<OCIE1A);
        arr[i] = distance_us/58.2;
      }
      cur_distance_cm = 0;
      uint8_t maxi = 0; uint8_t mini = 0;
      for (int i=1; i<5; i++) {
        if (arr[i]>arr[maxi]) {
          maxi = i;
        }
        if (arr[i]<arr[mini]) {
          mini = i;
        }
      }
      for (int i=0; i<5; i++) {
        if (i==mini || i==maxi) {
          continue;
        } else {
          cur_distance_cm += arr[i];
        }
      }
      cur_distance_cm/=3;
    }

    /* // line stopping code of calling interrupt from PCINT0_vect and PCINT5
    if(!pinging) {
      pinging = 1;

      SET_BIT(DDRB, DDB4);
      SET_BIT(PORTB, PB4);
    }
    */

    // check soil sensors

    if (ss.touchRead(0) < 400 &&  !is_open) {
      openvalve = 1;
      close_n = get_s_ticks() + 2;  // open anywhere between 1.01 and 2.00 seconds
    }
    // logic of program determine wether to water based on soil and last watering


    //testing with some GPIO lights
  /*  if (cur_distance_cm > 0) {
      SET_BIT(PORTB, PB6);
      SET_BIT(DDRB, DDB6);
      CLEAR_BIT(DDRB, DDB7);
      CLEAR_BIT(PORTB, PB7);
      _delay_ms(10);
    } else if (pinging == 1) {
      SET_BIT(PORTB, PB7);
      SET_BIT(DDRB, DDB7);
      CLEAR_BIT(PORTB, PB6);
      CLEAR_BIT(DDRB, DDB6);
      _delay_ms(10);
    }
    if(get_s_ticks() > 10) {
      SET_BIT(PORTB, PB6);
      SET_BIT(DDRB, DDB6);
      CLEAR_BIT(DDRB, DDB7);
      CLEAR_BIT(PORTB, PB7);
      _delay_ms(9000);
    //  pinging=0;
    //  return;
    }*/

  } /* end while(1) */
} /* end main() */

ISR (TIMER0_COMPA_vect) {
  ms_ticks++;
  if (!(ms_ticks%1000)) {
    //ms_ticks-=1000;
    s_ticks++;
  }
}  /* end ISR (TIMER0_COMPA_vect) */

ISR (TIMER1_COMPA_vect) {  // microsecond timer
  us_ticks++;
}

ISR (PCINT0_vect) {  // echo pin, pin 9, attached to pcint5
  uint64_t distance_us = get_us_ticks();
  uint8_t m = PINB & (1<<PCINT5);
  m=1;
  if (m) {
    cur_distance_cm = distance_us/58.2;
    pinging = 0;
    CLEAR_BIT(PORTB, PB5);
//    CLEAR_BIT(DDRB, DDB5);
  }
}  // end ISR (PCINT0_vect)

ISR (USART1_RX_vect) {
    uint8_t ch = UDR1;
    if (ch == 13 || (recv_buffer_ptr >= RECEIVE_BUFFER_LENGTH)) {
        user_command_ready = 1;
    }
    else if (ch == 8) { //backspace supposedly
        if (recv_buffer_ptr >= 1)
            --recv_buffer_ptr;
        recv_buffer[recv_buffer_ptr] = 0;
    }
    else if
		( (ch == ' ') || (ch == '.') || (ch == '+') || (ch == '-') ||
		((ch >= '0') && (ch <= '9')) ||
		((ch >= 'A') && (ch <= 'Z')) ||
		((ch >= 'a') && (ch <= 'z')) ) {
        recv_buffer[recv_buffer_ptr] = ch;
        ++recv_buffer_ptr;
    }
    sendString(recv_buffer);
    sendString("\n\r");
//  sprintf(output_buffer,"%s\n\r",recv_buffer);
//  sendString(output_buffer);
//  memset(output_buffer,'\0',OUTPUT_BUFFER_LENGTH);
} // end ISR(USART1_RX_vect)
