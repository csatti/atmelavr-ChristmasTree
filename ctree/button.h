/*
 * button.h
 *
 * Created: 2015.12.15. 15:22:08
 *  Author: Csatti
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t shortPress;
extern volatile uint8_t longPress;
extern volatile uint8_t ignorePress;

void setup_button(void);


#endif /* BUTTON_H_ */
