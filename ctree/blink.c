/*
 * blink.c
 *
 * Created: 2015.12.12. 15:06:02
 *  Author: Csatti
 */ 

#include "blink.h"

volatile uint16_t progcnt = 0;
volatile uint8_t prog[200];

volatile uint32_t timeReference = 0;	// time elapsed since start up (approx in milliseconds)
volatile uint16_t timeStep = 1000;
volatile uint8_t lcnt = 16; 
volatile uint8_t lclr = 0;

uint8_t intensityRed = 0xFF;
uint8_t intensityBlue = 0xFF;
uint8_t intensityGreen = 0xFF;


void set_ledcontrol(uint8_t control)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (control & 1) {
			switch (i)
			{
				case 0: PORTC &= ~_BV(1); break;
				case 1: PORTC &= ~_BV(2); break;
				case 2: PORTC &= ~_BV(5); break;
				case 3: PORTD &= ~_BV(0); break;
				case 4: PORTC &= ~_BV(0); break;
				case 5: PORTC &= ~_BV(3); break;
				case 6: PORTC &= ~_BV(4); break;
				case 7: PORTD &= ~_BV(1); break;
			}
		}
		else
		{
			switch (i)
			{
				case 0: PORTC |= _BV(1); break;
				case 1: PORTC |= _BV(2); break;
				case 2: PORTC |= _BV(5); break;
				case 3: PORTD |= _BV(0); break;
				case 4: PORTC |= _BV(0); break;
				case 5: PORTC |= _BV(3); break;
				case 6: PORTC |= _BV(4); break;
				case 7: PORTD |= _BV(1); break;
			}
		}
		control >>= 1;
	}
}

void set_color(uint8_t color)
{
	if (color == 1) {
		TCCR0B = _BV(CS01);  // Prescaler = 8
		OCR0A = intensityRed >> 2;
		TCNT0 = 0;
		TCCR0A |= _BV(COM0A1) | _BV(COM0A0);
	}
	else
	{
		TCCR0A &= ~(_BV(COM0A1)| _BV(COM0A0)); 
		PORTD |= _BV(6);
	}
	if (color == 2) {
		OCR2B = intensityGreen >> 1;
		TCNT2 = 0;
		TCCR2A |= _BV(COM2B1) | _BV(COM2B0);
	}
	else
	{
		TCCR2A &= ~(_BV(COM2B1)| _BV(COM2B0)); 
		PORTD |= _BV(3);
	}
	if (color == 3) {
		TCCR0B = _BV(WGM02) | _BV(CS01) | _BV(CS00); // Prescaler = 64, fast PWM with OCR0A as top
		OCR0A = 0x3F;
		OCR0B = intensityBlue >> 5;
		TCNT0 = 0;
		TCCR0A |= _BV(COM0B1);
	}
	else
	{
		TCCR0A &= ~_BV(COM0B1); 
		PORTD &= ~_BV(5);
	}
}

ISR(TIMER2_COMPB_vect)
{
	timeReference++;
	if (timeStep) timeStep--;
}

void click(void)
{
	if (timeStep == 0) {
		timeStep = 200;
		if (lcnt == 0x00) {
			lcnt = 0x1;
			
		}
		if (++lclr > 3) lclr = 1;
		set_color(lclr);
		set_ledcontrol(lcnt);
		lcnt <<= 1;
	}
}

void setup_timers(void)
{
	cli();
	
	// Timer 0, PWM for blue and red colors
	TCCR0A = _BV(WGM00) | _BV(WGM01);
	TCNT0 = 0;
	OCR0A = 0x7F;
	OCR0B = 0x7F;
	
	// Timer 2, PWM for green color
	TCCR2A = _BV(WGM20) | _BV(WGM21);
	TCNT2 = 0x7F; // offset compared to timer 0
	OCR2B = 0x7F;
	OCR2A = 0x7F;
	TIMSK2 = _BV(OCIE2B);
	
	TCCR0B = _BV(CS01); // Enable timer 0 with prescaler = 8
	TCCR2B = _BV(CS21)| _BV(WGM22); // Enable timer 2 with prescaler = 8
	sei();
}