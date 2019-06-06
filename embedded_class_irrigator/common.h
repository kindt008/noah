#ifndef COMMON_H_
#define COMMON_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#define MATCH250 15625 //16000*250/256
#define MICROSEC (F_CPU*(1/64))
#define ERROR -1

#define OUTPUT_BUFFER_LENGTH 50
#define RECEIVE_BUFFER_LENGTH 50
static volatile char output_buffer[OUTPUT_BUFFER_LENGTH];
static volatile char recv_buffer[RECEIVE_BUFFER_LENGTH];
static volatile char test_buffer[OUTPUT_BUFFER_LENGTH];

uint8_t openvalve;
uint8_t closevalve;

uint8_t is_open;

uint64_t close_n;  // next time closing should be done. in s_ticks
uint64_t flow_n;  // next time water will be allowed to flow. in s_ticks.
uint8_t valve(uint8_t cmd);  // pass in 0 to close valve 1 to open returns 0 on success and error code otherwise
uint64_t us_ticks;
uint64_t ms_ticks;
uint64_t s_ticks;

volatile uint8_t recv_buffer_ptr;
volatile uint8_t user_command_ready;
uint8_t in_ui_mode;
volatile uint32_t last_ms;

uint8_t pinging;
uint64_t cur_distance_cm;

//class Adafruit_seesaw;

Adafruit_seesaw ss;  // data type for soil sensor in downloaded library

uint64_t get_s_ticks();
uint64_t get_ms_ticks();
uint64_t get_us_ticks();

void adc_init();
uint16_t adc_read_1(void);
uint16_t adc_read_2(void);

void sendChar(char c);
void sendString(char* s);
void setupUART(void);
void handleInput();
void printMenu();

#define SET_BIT(port, pin) (port |= (1<<pin))
#define CLEAR_BIT(port, pin) (port &= ~(1<<pin))
#define TOGGLE_BIT(port, pin) (port ^= (1<<pin))

#define CONFIG_OUTPUT(port, pin) (SET_BIT(port,pin))
#define CONFIG_INPUT(port,pin) (CLEAR_BIT(port,pin))

typedef struct {
  volatile uint8_t * ddr;
  volatile uint8_t * port;
  uint8_t pin;
  volatile uint8_t * portin;
} IO_struct;

#endif
