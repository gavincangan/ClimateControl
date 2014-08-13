#ifndef _UTILITIES_H
#define _UTILITIES_H

	#include "macros.h"
	#include "hw_setup.h"
	#include "blink.h"
	#include "adc.h"
	#include "lcd.h"
	#include "menu.h"
	#include "board_init.h"
	#include "typedef.h"	
	
	void system_init();
	void splash_screen();
	void read_sensors();
	void error_process();
	void update_display();
	void update_devices();
	
#endif