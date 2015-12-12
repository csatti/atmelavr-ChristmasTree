/*
 * blink.c
 *
 * Created: 2015.12.12. 15:06:02
 *  Author: Csatti
 */ 

#include "blink.h"

volatile uint16_t progcnt = 0;
volatile uint8_t prog[200];

volatile uint32_t timeReference = 0;	// time elapsed since start up (in milliseconds)
volatile uint16_t timeStep = 1000;
volatile uint8_t lcnt = 16; 
volatile uint8_t lclr = 1;


void set_ledcontrol(uint8_t control)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (control & 1) {
			switch (i)
			{
				case 0: PORTC &= ~_BV(0); break;
				case 1: PORTC &= ~_BV(1); break;
				case 2: PORTC &= ~_BV(4); break;
				case 3: PORTC &= ~_BV(5); break;
				case 4: PORTC &= ~_BV(2); break;
				case 5: PORTC &= ~_BV(3); break;
				case 6: PORTD &= ~_BV(0); break;
				case 7: PORTD &= ~_BV(1); break;
			}
		}
		else
		{
			switch (i)
			{
				case 0: PORTC |= _BV(0); break;
				case 1: PORTC |= _BV(1); break;
				case 2: PORTC |= _BV(4); break;
				case 3: PORTC |= _BV(5); break;
				case 4: PORTC |= _BV(2); break;
				case 5: PORTC |= _BV(3); break;
				case 6: PORTD |= _BV(0); break;
				case 7: PORTD |= _BV(1); break;
			}
		}
		control >>= 1;
	}
}

void set_color(uint8_t color)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		if (color & 1) {
			switch (i)
			{
				case 0: PORTD &= ~_BV(6); break;
				case 1: PORTD &= ~_BV(3); break;
				case 2: PORTD |= _BV(5); break;
			}
		}
		else
		{
			switch (i)
			{
				case 0: PORTD |= _BV(6); break;
				case 1: PORTD |= _BV(3); break;
				case 2: PORTD &= ~_BV(5); break;
			}
		}
		color >>= 1;
	}
}

ISR(TIMER1_CAPT_vect)
{
	//timeReference++;
	if (--timeStep == 0) {
		timeStep = 200;
		if (lcnt == 0) lcnt = 1;
		set_ledcontrol(lcnt);
		lcnt <<= 1;
	}
}

void click(void)
{
	//timeReference++;
	timeStep--;
	if (timeStep == 0) {
		timeStep = 200;
		if (lcnt == 16) {
			lcnt = 0x11;
			
		}
		lclr >>= 1;
			if (!lclr) lclr = 4;
			set_color(lclr);
		set_ledcontrol(lcnt);
		lcnt <<= 1;
	}
}

void setup_timer(void)
{
	TCCR1A = 0;
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // No prescaling, CTC with ICR1
	TCCR1C = 0;
	TCNT1 = 0;
	ICR1 = 999;
	OCR1A = 500;
	//TIMSK1 = _BV(OCIE1A) | _BV(ICIE1);
}