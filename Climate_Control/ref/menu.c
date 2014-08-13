#include "menu.h"
	
	uint8 t_low=TEMP_MIN;
	uint8 t_high=TEMP_MAX;
	uint8 p_low=POL_MIN;
	uint8 p_high=POL_MAX;
	
	// Set to 1-> Menu is invoked at next iteration of master loop
	extern uint8 menu_flag;
	
	uint8 key=0;

	extern uint16 menu_timer;
	
	//Called by various functions to print the static part of the text
	//to be displayed on LCD display in each page
	//page   0->main menu   1->temperature   2->pollution
	void print_menu(uint8 page){	
		clr_lcd();
		switch(page){
			case 0:
			lcd_write_byte(0x80,0);
			lcd_write_str("***** Menu *****",16);
			lcd_write_byte(0xc0,0);
			lcd_write_str(" ( )T ( )P ( )R ",16);
			break;
			
			case 1:
			lcd_write_byte(0x80,0);
			lcd_write_str("Temperature :",16);
			lcd_write_byte(0xc0,0);
			lcd_write_str(" ( )LOW ( )HIGH ",16);
			break;
			
			case 2:
			lcd_write_byte(0x80,0);
			lcd_write_str("Pollution :",16);
			lcd_write_byte(0xc0,0);
			lcd_write_str(" ( )MIN  ( )MAX ",16);
			break;
		}
	}
	
	//Called by various functions
	//Moves the select pointer on LCD screen and returns a value based on choice made
	//Argument: menu->choices/to/be/made   0->Temp/Pol/Run    1->Temp_L/Temp_H     2->Pol_L/Pol_H
	uint8 menu_pick(uint8 menu){
		uint8 choice_cap=0,start=0,now=0;
		//positions defined in menu.h as macros
		const uint8 menu_pos[]={MENU_TEMP_POS,MENU_POL_POS,MENU_RUN_POS,T_LOW_POS,T_HIGH_POS,POL_MIN_POS,POL_MAX_POS};
		if(menu==0)	{start=0;choice_cap=3;}	// position from 'menu_pos' array the start and choice_cap
		if(menu==1)	{start=3;choice_cap=5;}	// are positions in the above array. They are chosen based on
		if(menu==2)	{start=5;choice_cap=7;}	// input value of menu
		now=start;
		lcd_locate(1,menu_pos[now]); lcd_write_byte('*',1);
		do{
		key = readkey();
			if(getBit(key,0)){
				while(!NEXT & SELECT);		//loop until NEXT is released - to debounce 			
				lcd_locate(1,menu_pos[now]); lcd_write_byte(' ',1);		//clear current position
				if(++now>=choice_cap) now=start;		//next position //if overflow reset
				lcd_locate(1,menu_pos[now]); lcd_write_byte('*',1); 	//write to new position
			}
		}while( !getBit(key,1) && menu_flag);	//loop until SELECT is pressed
		while(!SELECT && menu_flag);			//loop until SELECT is released - to debounce 	
		
		return (now-start);
	}
	
	//Reads the tact switches and returns a value based on their current values
	//Resets the menu time-out timer if any key is pressed i.e. in case of non-zero value being returned
	uint8 readkey(){
		uint8 k=0;
		if(!NEXT)k+=1;
		if(!SELECT)k+=2;
		if(k) menu_timer=0;
		return (k);
		}
	
	//Called by menu_invoke()
	//Reads the choice made by user and calls corresponding function
	//choice  0->temperature 1->pollution 2->run
	void menu_process(){
			uint8 choice=0;
			choice=menu_pick(0);
			if(choice == 0)			go_temp();
			else if(choice == 1)	go_pol();
			else if(choice == 2)	run();
		}
	
	//Exit point for the menu
	//Resets menu_flag and sets the interrupt priority flag to disable the masking of key interrupts
	//Key interrupt would have been masked by the ISR.
	void run(){
		menu_flag=0;
		//kien = 0x50;
		kupic = 0x01; // enable key interrupt
		}
	
	//Called by menu_process() based on choice made (temperature) in first screen
	//Prints second page of menu and reads input -> 't_high' or 't_low'
	//Calls set_value() with the corresponding argument
	void go_temp(){
			uint8 choice=0;
			print_menu(1);
			choice=menu_pick(1);
			if(choice == 0)			t_low=set_value(0,t_low);
			if(choice == 1)			t_high=set_value(1,t_high);
			
			if(menu_flag){
				menu_invoke();	
			}
			run();
		}
	
	//Called by menu_process() based on choice made (pollution) in first screen
	//Prints second page of menu and reads input -> 'p_high' or 'p_low'
	//Calls set_value() with the corresponding argument
	void go_pol(){
			uint8 choice=0;
			print_menu(2);
			choice=menu_pick(2);
			if(choice == 0)			p_low=set_value(2,p_low);
			if(choice == 1)			p_high=set_value(3,p_high);
			
			if(menu_flag){
				menu_invoke();
			}
			run();
		}
	
	//This function is called to read low and high values of temperature and pollution
	//Takes choice and previous value of the chosen parameter as arguments
	//choice 0->t_low 1->t_high 2->p_low 3->p_high
	uint8 set_value(uint8 choice,sint8 value){
		
		sint8 lower_bound=0, higher_bound=0;
		uint8 data_type=0;
		if(!menu_flag) return value; 
		
		//lower_bound and upper_bound are set to avoid t_low being set higher than t_high
		//or p_low being set higher than p_high or vice versa
		switch(choice){
			case 0:	lower_bound=TEMP_MIN;	higher_bound=t_high-1; data_type=0; break;
			case 1:	lower_bound=t_low+1;	higher_bound=TEMP_MAX; data_type=0; break;
			case 2:	lower_bound=POL_MIN;	higher_bound=p_high-1; data_type =1; break;
			case 3:	lower_bound=p_low+1;	higher_bound=POL_MAX; data_type = 1; break;			
		}
		
		//lcd_locate(1,0); lcd_write_num(value,16);
		block_print(value,data_type);
		
		do{
		key = readkey();
			
			//if at any loop menu_flag is reset (by menu time-out), exit loop
			if(getBit(key,0)&& menu_flag){
				while(!NEXT & SELECT && menu_flag); //loop until NEXT is released - to debounce 				
				
				//since there is only one key to change values
				//whether to increment or decrement is chosen based 
				//on the variable. For t_high and p_high decrement
				//for t_low and p_low increment
				if(choice & 1)--value;
				else ++value;
				
				//value is contained within bounds by setting to the other bound once limit is crossed
				if(value>higher_bound) value=lower_bound;
				if(value<lower_bound) value=higher_bound;
				
				//lcd_locate(1,0); lcd_write_num(value,3);
				block_print(value,data_type);
			}
		}while( !getBit(key,1)  && menu_flag);	//loop until SELECT key is pressed
		while(!SELECT && menu_flag);			//loop until SELECT is released - to debounce 	

		return value;
	}
	
	//This function is the entry point to invoke the menu.
	//Its starts the timer RA for menu-time-up detection 
	//Time-up at 5 secs of no response while displaying menu
	void menu_invoke(){
		tstart_tracr=1;
		print_menu(0);
		menu_process();
		}