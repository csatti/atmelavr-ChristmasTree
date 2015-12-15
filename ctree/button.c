/*
 * button.c
 *
 * Created: 2015.12.15. 15:22:20
 *  Author: Csatti
 */ 

#include "button.h"
#include "blink.h"

volatile uint8_t shortPress;
volatile uint8_t longPress;
volatile uint8_t ignorePress;
volatile uint32_t buttonPressed;

ISR(INT0_vect)
{
	if (PIND & _BV(2)) {
		if (ignorePress) {
			ignorePress = 0;
			return;
		}
		uint32_t p = timeReference - buttonPressed;
		if ( p > 1000) {
			longPress = 1;
			return;
		}
		if ( p > 20) shortPress = 1;				
	}
	else buttonPressed = timeReference;
	
}

void setup_button(void)
{
	EICRA = _BV(ISC00);
	cli();
	EIMSK = _BV(INT0);
	sei();
}

