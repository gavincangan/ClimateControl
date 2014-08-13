#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H
	#include "typedef.h"
	#include "..\\sfr_r823.h"
	#include "hw_setup.h"
	#include "lcd.h"
	#include "macros.h"

	void key_interrupt_config();
	void timer_ra_init();
	
	#pragma interrupt  KI_isr(vect = 13);
	void KI_isr();
	
	#pragma interrupt  timer_ra_isr(vect = 22);
	void timer_ra_isr();
	
	void stop_timer();
	
#endif