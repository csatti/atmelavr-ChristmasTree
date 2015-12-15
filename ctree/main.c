/*
 * ctree.c
 *
 * Created: 2015.12.12. 14:20:29
 * Author : Csatti
 */ 

#include <avr/io.h>
#include "blink.h"
#include "button.h"
#include <util/delay.h>
#include <avr/sleep.h>

void go_to_sleep(void)
{
	PORTB = 0xFE;
	PORTC = 0x3F;
	PORTD = 0xDF;
	ignorePress = 1;
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
	sleep_disable();
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
	PORTD = 0x44;
	setup_timer();
	setup_button();
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
		if (shortPress) {
			shortPress = 0;
			PORTC = 0;
			_delay_ms(2000);
		}
		if (longPress) {
			longPress = 0;
			go_to_sleep();
		}
		
    }
}

