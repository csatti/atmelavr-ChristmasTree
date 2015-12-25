/*
 * progs.c
 *
 * Created: 2015.12.16. 14:38:26
 *  Author: Csatti
 */ 

#include "progs.h"
#include "blink.h"

uint8_t lcnt = 16;
uint8_t lclr = 0;
uint16_t progCnt = 0;

uint8_t prog1(uint8_t init)
{
	if (init) {
		set_intensity(0xFF);
		return 0;
	}
	if (timeStep == 0) {
		timeStep = 100;
		if (lcnt == 0x00) {
			lcnt = 0x1;
			
		}
		if (++lclr > 3) lclr = 1;
		set_color(lclr);
		set_led_control(lcnt);
		lcnt <<= 1;
		progCnt++;
		if (progCnt > 100) { // cycle num
			progCnt = 0;
			return 1;
		}
	}
	return 0;
}

uint8_t prog2(uint8_t init)
{
	static uint8_t color = 1;
	static uint8_t ledint = 0;
	
	if (init) {
		ledint = 0;
		color = 1;
		set_color_with_intensity(color, ledint);
		set_led_control(0xFF);
		return 0;
	}
	if (timeStep) return 0;
	timeStep = 25;
	if (!ledint) ledint = 1;
	else
		ledint += (ledint+1) / 2;
	set_intensity(ledint);
	if (ledint > 100) {
		color++;
		ledint = 0;
		timeStep = 200;
		set_intensity(ledint);
		if (color > 2) color = 1;
		set_color(color);
		progCnt++;
		if (progCnt > 6) { //cycle num
			progCnt = 0;
			return 1;
		}	
	}
	return 0;
}

uint8_t prog3(uint8_t init)
{
	static uint8_t color;
	static uint8_t ledctrl;
	static uint8_t run;
	static uint8_t runcolor;
	
	if (init) {
		color = 1;
		run = 0;
		runcolor = 0;
		ledctrl = 0;
		set_color_with_intensity(color, 0xFF);
		set_led_control(0x0);
		return 0;
	}
	
	if (timeStep) return 0;
	timeStep = 5;
	run++;
	run %= 20;
	if (!run) {
		ledctrl++;
		ledctrl %= 4;
		runcolor++;
		runcolor %= 24; //cycle num
		if (!runcolor) {
			color++;
			if (color == 4) return 1;
		}
	}
	set_led_control(0x0);
	if (run & 1) {
		set_color(color + 1);
		set_led_control(0x11 << ((ledctrl + 1) % 4));
	}
	else
	{
		set_color(color);
		set_led_control(0x11 << ledctrl);
	}
	return 0;
}

uint8_t prog4(uint8_t init)
{
	static uint8_t color;
	static uint8_t run;
	static uint8_t rundir;
	static uint8_t runcolor;
	
	if (init) {
		color = 1;
		run = 0;
		runcolor = 0;
		rundir = 1;
		set_color_with_intensity(0, 0x7F);
		set_led_control(0xF0);
		return 0;
	}
	
	if (timeStep) return 0;
	if (rundir) run++;
	else run --;
	if (run == 120) rundir = 0;
	if (!run) {
		runcolor++;
		runcolor %= 2; // cycle num
		rundir = 1;
		if (!runcolor) {
			color++;
			if (color == 4) return 1;
		}
	}
	if (run & 1) {		
		timeStep = run / 24;
		if (timeStep) set_color(color + 1);
	}
	else
	{		
		timeStep = (120 - run) / 24;
		if (timeStep) set_color(color);
	}
	return 0;
	
	
}
	
	
uint8_t prog5(uint8_t init)
{
	static uint8_t color;
	static uint8_t run;
	static uint8_t cycle;
	static uint8_t delay;
	
	if (init) {
		color = 1;
		run = 0;
		cycle = 0;
		delay = 0;
		set_color_with_intensity(0, 0x7F);
		set_led_control(0xF0);
		return 0;
	}
	
	if (timeStep) return 0;
	if (!delay) {
		if ((run == 0) || (run == 60)) delay = 50;
	}
	if (delay) delay--;
	if (!delay) run++;
	if (run == 120) {
		run = 0;
		color++;
		if (color == 4) {
			color = 1;
			cycle++;
			cycle %= 5; // cycle num
			if (!cycle) return 1;
		}
	}
	if (!(run & 1) && !(delay & 1)) {
		timeStep = run / 12;
		if (timeStep) set_color(color + 1);
	}
	else
	{
		timeStep = (120 - run) / 12;
		if (timeStep) set_color(color);
	}
	return 0;
	
	
}