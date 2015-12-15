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
#include <util/delay.h>
#include <avr/sleep.h>

void battery_check(void)
{
	uint16_t adc;
	
	set_color(3); // Blue
	set_ledcontrol(0x11); // Switch on blue lights as a load
	_delay_ms(200);
	
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // AVCC as reference, measuring internal 1V1
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | _BV(ADPS1); // ADC enable, start conversion, interrupt flag cleared, prescaler = 4
	while (!(ADCSRA & _BV(ADIF))) {}
	adc = ADC;	
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | _BV(ADPS1); // New conversion	
	while (!(ADCSRA & _BV(ADIF))) {}
	adc = ADC;
	ADCSRA = 0;
	if (adc > 442) set_color(1); // Red, about 2.4V and bellow
	else set_color(2); // Green
	_delay_ms(2000);
	set_ledcontrol(0);
}

void go_to_sleep(void)
{
	PORTB = 0xFE;
	PORTC = 0x3F;
	PORTD = 0xDF;
	ignorePress = 1;
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	PORTB = 0x01;
	PORTD = 0x44;
	EIMSK = _BV(INT0);
}


void setup(void)
{
	DDRB = 0x01;
	DDRC = 0x3F;
	DDRD = 0x6B;
	PORTB = 0x01;
	PORTC = 0x3F;
	PORTD = 0x47;
	setup_timers();
	setup_button();
}

int main(void)
{
    /* Replace with your application code */
	setup();
	battery_check();
    while (1) 
    {
		if (TIFR1 & _BV(ICF1)) {
			TIFR1 |= _BV(ICF1);
			click();
		}
		if (shortPress) {
			shortPress = 0;
			PORTC = 0;
			_delay_ms(2000);
		}
		if (longPress) {
			longPress = 0;
			go_to_sleep();
			battery_check();
		}
		
    }
}

