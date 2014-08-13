#ifndef _MENU_H
#define _MENU_H
	#include "lcd.h"
	#include "typedef.h"
	#include "macros.h"
	#include "interrupts.h"
	
	void print_menu(uint8);
	uint8 menu_pick(uint8);
	void menu_process();
	void go_temp();
	void go_pol();
	uint8 set_value(uint8 choice,sint8 value);
	void menu_invoke();
	uint8 readkey();
	void run();
#endif