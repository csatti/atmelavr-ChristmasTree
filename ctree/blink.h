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

void click(void);
void setup_timer(void);



#endif /* BLINK_H_ */
