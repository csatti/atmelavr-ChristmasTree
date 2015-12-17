/*
 * blink.h
 *
 * Created: 2015.12.12. 15:05:52
 *  Author: Csatti
 */ 

#ifndef BLINK_H_
#define BLINK_H_

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint32_t timeReference;	// time elapsed since start up (in milliseconds)
extern volatile uint16_t timeStep;

void set_led_control(uint8_t control);
void set_color(uint8_t color);
void set_intensity(uint8_t intensity);
void set_color_with_intensity(uint8_t color, uint8_t intensity);
void setup_timers(void);



#endif /* BLINK_H_ */
