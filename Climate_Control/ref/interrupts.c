#include "interrupts.h"
	
	uint16 counter=0, menu_timer=0;
	extern uint8 key;
	
	//flags - set by their respective fucntions in blink.c file
	//control the LED toggle based on the flags
	// Menu flag: Set to 1-> Menu is invoked at next iteration of master loop
	extern uint8 window_blink_on, aiv_blink_on, menu_flag;
	
	//Sets the priority levels and enables key interrupt	
	void key_interrupt_config(){
		kupic = 0x01;
		kien = 0x50;
		asm("FSET I");	 //Assembly instruction to enable interrupts
	}
	
	//Key interrupt service routine
	//Masks further key interrupts and sets menu_flag
	void KI_isr(){
		//kien = 0x00;
		kupic = 0x00;
		menu_flag=1;
		key = 0;	
	}
	
	//Intialise timer RA
	//function call frequency = f_osc/((1 + tra)(1 + trapre))
	//f_osc = 10MHz <= fra2=0x02 (divide_by_four) set in board_init.c
	void timer_ra_init(){
		tramr=0x20; //FOCO, timer mode
		tra=99; //initial value from calc
		trapre=99; //pre-scaler
		traic=0x01;	//enable interrupt - level set
		asm("FSET I");
		//tstart_tracr=1; // start counting - Timer RA - Control Register
		}
	
	//Timer RA underflow - interrupt service routine
	void timer_ra_isr(){
		tstart_tracr=0;
		counter++; 
		if(counter>1000){
			if(window_blink_on) WINDOW_IND=!WINDOW_IND;
			if(aiv_blink_on) AIV_IND=!AIV_IND;
			counter=0;
			}
		menu_timer++;
		if(menu_timer>MENU_TIMEOUT_MILLIS){
			menu_timer=0;
			//lcd_write_byte(0x80,0);
			//lcd_write_str("TIME UP!!",9);
			menu_flag=0;
			}
		tstart_tracr=1;
		}
	
	//Stops timer and resets menu_timer and counter variables
	//Checks if any flags is active since timer is shared by various functions
	void stop_timer(){
		if(window_blink_on || aiv_blink_on || menu_flag) return;
		tstart_tracr=0;
		counter = 0;
		menu_timer = 0;
		}