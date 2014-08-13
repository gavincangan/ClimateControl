#include "blink.h"
#include "hw_setup.h"

uint8 window_blink_on=0;
uint8 aiv_blink_on=0;

//Plain old delay function using decrementing loop variable
void delay(uint32 count){
	while(count--);
}

//Sets flag to blink WINDOW_IND LED
//Sets timer RA enable flag
void window_blink(){
	if(window_blink_on) return;
    window_blink_on =1;
	tstart_tracr=1; //timer_start
	WINDOW_IND=IND_OFF;
}

//Sets flag to blink AIV_IND LED
//Sets timer RA enable flag
void aiv_blink(){
	if(aiv_blink_on) return;
    aiv_blink_on =1;
	tstart_tracr=1; //timer_start
	AIV_IND=IND_OFF;
}

//Calls stop_timer() and resets window_blink_on flag
void window_blink_stop(){
	 	stop_timer();
	window_blink_on=0; 
}

//Calls stop_timer() and resets aiv_blink_on flag
void aiv_blink_stop(){
	 	stop_timer();
	aiv_blink_on=0;
}