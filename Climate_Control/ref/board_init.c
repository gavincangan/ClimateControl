#include "board_init.h"

void board_init(){
	prcr = 0x07;
	//prcr = 0x03;
	pd0 = 0x3f; //lcd
	pm0 = 0x00;
	cm0 = 0x28;
	cm1 = 0x10;
	fra00 = 1;
	fra01 = 1;
	fra2 = 0x02;// clock divided by 4
	prcr = 0x00;
	// led
	pd1_7 = 1;
    pd2_0 = 1;
  	pd2_2 = 1;
	
	// toggle
	pd4_3 = 0;
	pd4_4 = 0;
	
	// tact
	pd1_3 = 0;
	pd1_2 = 0;
	
	WINDOW_IND = IND_OFF;
	AIV_IND = IND_OFF;
	AC_IND = IND_OFF;
	
	WINDOW = AIV = NEXT = SELECT = 1;
}
