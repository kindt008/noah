#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "timers.h"
#include "leds.h"

/* Set up a timer using ctc mode.
 * parameter [in] timer_num : 0 (8-bit) or 1,3 (16-bit)
 * parameter [in] prescaler : 1,8,64, ... as indicated in datasheet options
 * parameter [in] ms_period : desired period of ISR
 * parameter [out] : -1 for error in setting up. 1 otherwise.
 */
int SetUpTimerCTC( int timer_num, int prescaler, int ms_period ) {
  if ((0 != timer_num) && ((1 != timer_num) && (3 != timer_num))) {
    //TOGGLE_BIT(*(&_red)->port, _red.pin);
    return ERROR;
  }

  // determine the match value for the desired period given prescaler
  // @TODO check that this is an integer value -- if not give warning.
  // ********************************************************************* //
  // ******** MODIFY THIS LINE. Use the input parameters.  *************** //
  // ********************************************************************* //
  if(prescaler<0){
    //TOGGLE_BIT(*(&_red)->port, _red.pin);
    return ERROR;
  }

  uint32_t match32=(F_CPU/1000.0)*ms_period/prescaler;
  // Check that the match fits in its counter register (8-bit or 16-bit)
  if (match32>=65535 || ((0 == timer_num) && (match32 > 255))) {
    //TOGGLE_BIT(*(&_red)->port, _red.pin);
    return ERROR;
  }

  // Get the clock-select bits for the chosen prescaler
  char CSbits = GetCSBits(prescaler);

  switch(timer_num) {
    case(0) : return SetUpTimer_0(CSbits, (uint8_t) match32); break;
    case(1) : return SetUpTimer_1(CSbits, (uint16_t) match32); break;
    case(3) : return SetUpTimer_3(CSbits, (uint16_t) match32); break;
    default : return ERROR;
  }
  return 1;
}

int SetUpTimer_0(char CSbits, uint8_t match) {

  // Clear the Control Registers for Timer 0
  TCCR0A = 0;
  TCCR0B = 0;

  // CTC mode is 2 = 010
  TCCR0A |= (1 << WGM01);
  // Clock Select Bits are bits 00000XXX in TCCR0B
  TCCR0B |= CSbits;
  // CTC uses OCR0A as top / match
  OCR0A = match;
  // Enable the Interrupt on the OCR0A match
  TIMSK0 |= (1<<OCIE0A);
  return 1;
}

int SetUpTimer_1(char CSbits, uint16_t match) {
  // ********      FILL THIS IN **************************//
  // Clear the Control Registers for Timer 1
  TCCR1A = 0;
  TCCR1B = 0;

  // bits should clear on match
  // CTC mode is 2 = 010
  TCCR1B |= (1 << WGM12);  // set this to proper PWM value now
  // Clock Select Bits are bits 00000XXX in TCCR0B
  TCCR1B |= CSbits;
  // CTC uses OCR0A as top / match
  OCR1A = match;  // 1b uses TIMER1_COMPB_vect
  // Enable the Interrupt on the OCR0A match
  TIMSK1 |= (1<<OCIE1A);
  return 1;
}

int SetUpTimer_3(char CSbits, uint16_t match) {
  // ********      FILL THIS IN **************************//
  // Clear the Control Registers for Timer 3
  TCCR3A = 0;
  TCCR3B = 0;

  // CTC mode is 2 = 010
  TCCR3B |= (1 << WGM32);
  // Clock Select Bits are bits 00000XXX in TCCR0B
  TCCR3B |= CSbits;
  // CTC uses OCR0A as top / match
  OCR3A = match;
  // Enable the Interrupt on the OCR0A match
  TIMSK3 |= (1<<OCIE3A);
  return 1;
}

char GetCSBits(int prescaler) {
  // homegrown dictionary.
  // key: prescaler. value: clock select bits value
  switch(prescaler) {
    case(1) : return 1; break;
    case(8) : return 2; break;
    case(64) : return 3; break;
    case(256) : return 4; break;
    case(1024) : return 5; break;
    default : return 0;
  }
}

int setUpTimer1PWM(uint16_t match) {
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 &= (1<<OCIE1A);
//  TIMSK1 |= (1<<OCIE1A);

  DDRB |= (1<<DDB6);  // here??? idk?

  TCCR1A |= (1<<WGM11) | (1<<WGM10);
  TCCR1A |= (1<<COM1A1);  // setting combits
  TCCR1B |= 1;  // look at return values of GetCSBits for operating value "csbits"
  TCCR1B |= (1<<WGM12) | (1<<WGM13);
  OCR1A=16;
  ICR1=32;
  //ICR1=MICROSEC*2;
  //OCR1A=MICROSEC;
  return 1;

}

int setUpTimer3PWM(uint16_t match) {
  TCCR3A = 0;
  TCCR3B = 0;


  TCCR3A |= (1<<WGM30);
  TCCR3A |= (1<<COM3A1);  // setting combits
  TCCR3B |= 4;  // look at return values of GetCSBits for operating value "csbits"
  TCCR3B |= (1<<WGM32);

  //ICR3=2*match;
  OCR3A=match;
  return 1;
}
