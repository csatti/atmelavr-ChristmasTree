/*
 * ctree.c
 *
 * Created: 2015.12.12. 14:20:29
 * Author : Csatti
 */ 

#include <avr/io.h>
#include "blink.h"


void setup(void)
{
	DDRB = 0x01;
	DDRC = 0x3F;
	DDRD = 0x6B;
	PORTB = 0x01;
	PORTD = 0x44;
	setup_timer();
	sei();
}

int main(void)
{
    /* Replace with your application code */
	setup();
    while (1) 
    {
		if (TIFR1 & _BV(ICF1)) {
			TIFR1 |= _BV(ICF1);
			click();
		}
    }
}

