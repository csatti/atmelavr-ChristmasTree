/*
 * ctree.c
 *
 * Created: 2015.12.12. 14:20:29
 * Author : Csatti
 */ 
#define F_CPU 1000000

#include <avr/io.h>
#include "blink.h"
#include "button.h"
#include "progs.h"
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <avr/boot.h>

static const uint16_t EEMEM ADC_2V4	= 442;
static uint8_t (* prog)(uint8_t);
static uint8_t progID = 0;

void battery_check(void)
{
	uint16_t adc;
	uint16_t adc_2v4 = eeprom_read_word(&ADC_2V4);
	set_color(3); // Blue
	set_led_control(0x0F); // Switch on blue lights as a load
	set_intensity(0x7F);
	PRR &= ~_BV(PRADC);
	_delay_ms(200);
	
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // AVCC as reference, measuring internal 1V1
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | _BV(ADPS1); // ADC enable, start conversion, interrupt flag cleared, prescaler = 4
	while (!(ADCSRA & _BV(ADIF))) {}
	adc = ADC;	
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | _BV(ADPS1); // New conversion	
	while (!(ADCSRA & _BV(ADIF))) {}
	adc = ADC;
	ADCSRA = 0;
	if (adc > adc_2v4) set_color(1); // Red, about 2.4V and bellow
	else set_color(2); // Green
	PRR |= _BV(PRADC);
	_delay_ms(2000);
	set_led_control(0);
}

void go_to_sleep(void)
{
	EIMSK = _BV(INT0);
	PORTB = 0xFE;
	PORTC = 0x3F;
	PORTD = 0xDF;
	ignorePress = 1;
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	PORTB = 0x01;
	PORTD = 0x44;
}


void setup(void)
{
	uint8_t highfuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
	DDRB = 0x01;
	DDRC = 0x3F;
	DDRD = 0x6B;
	PORTB = 0x01;
	PORTC = 0x3F;
	PORTD = 0x47;
	setup_timers();
	setup_button();
	PRR = _BV(PRTWI) | _BV(PRTIM1) | _BV(PRUSART0) | _BV(PRADC); // Switch off TWI, Timer 1, USART and ADC
	if (highfuse & _BV(6)) PRR |= _BV(PRSPI); // debugWire disabled, switch off SPI
}

void next_prog(void)
{
	progID++;
	progID %= 4;
	switch(progID)
	{
		case 0: prog = prog1; break;
		case 1: prog = prog4; break;
		case 2: prog = prog3; break;
		case 3: prog = prog5; break;
		case 4: prog = prog5; break;
	}
	prog(1);
}

int main(void)
{

	
	setup();
	battery_check();
	prog = prog1;
	prog(1);
    while (1) 
    {

		if (prog(0)) next_prog();
		//}
		if (shortPress) {
			shortPress = 0;
			next_prog();
		}
		if (longPress) {
			longPress = 0;
			go_to_sleep();
			battery_check();
		}
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
		
    }
}

