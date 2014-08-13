#ifndef _MACROS_H
#define _MACROS_H
	
	//Threshold values of sensor input data
	//Displays error if crossed
	#define TEMP_MIN 0
	#define TEMP_MAX 30
	#define POL_MIN 1
	#define POL_MAX 100
	
	//Macros to refer to ADC channels
	#define TEMP_SEN 0
	#define POL_SEN 1
	
	//Used in the menu_pick() function in menu.c
	//column position to write the '*' to, int the second row
	#define MENU_TEMP_POS 2
	#define MENU_POL_POS 7
	#define MENU_RUN_POS 12
	#define T_HIGH_POS 9
	#define T_LOW_POS 2
	#define POL_MAX_POS 10
	#define POL_MIN_POS 2

	//time elapsed before menu time-out occurs in millisecs
	#define MENU_TIMEOUT_MILLIS 5000
	
	//Bit manipulation macros
	#define getBit(num,pos) ((num>>pos)&1)
	#define setBit(num,pos) (num | (1<<pos))
	#define resetBit(num,pos) (num & (~(1<<pos)))
	
#endif