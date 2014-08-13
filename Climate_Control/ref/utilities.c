#include "utilities.h"

	sint8 cur_temp=0;
	uint8 cur_pol=0;
	uint8 err_flag=0, print_flag=1;
	
	//Range upper and lower bounds set by menu
	extern uint8 t_low, t_high, p_low, p_high;
	
	// Set to 1-> Menu is invoked at next iteration of master loop
	extern uint8 menu_flag;
	
	//Calls system intialization functions
	void system_init(){
		board_init();
		adc_config();
		lcd_init();
		key_interrupt_config();
		timer_ra_init();
	}
	
	//displays the splash screen at start of device
	void splash_screen(){
		uint8 count=0;
		
		//*print_welcome2
		sp_init_lcd();
		clr_lcd();
		
		lcd_write_byte(0x86,CMD);
		for(count=0;count<3;count++)
		lcd_write_byte(count,DATA);
		
		lcd_write_byte(0xC6,CMD);
		for(count=5;count>2;count--)
		lcd_write_byte(count,DATA);
		delay(500000);//*/ 
		clr_lcd();
		
		//*print_welcome
		sp_init2_lcd();
		clr_lcd();
		lcd_write_byte(0x84,CMD);
		for(count=0;count<8;count++)
		lcd_write_byte(count,DATA);
		lcd_write_byte(0xc0,CMD);
		lcd_write_str("InventedForLife",16);
		delay(500000);//*/ 
		
		//lcd_write_str("Welcome!!!",16); delay(500000);
		clr_lcd();
		
		}
	
	//Reads values from ADC channels
	//Values are set in corresponding variables
	void read_sensors(){
		cur_temp=adc_read(TEMP_SEN);
		cur_pol=adc_read(POL_SEN);
		}
	
	//Processes the values from sensors for error in range
	//Determines if the value is beyond min, max values
	void error_process(){
		if(cur_temp>TEMP_MAX || cur_temp<TEMP_MIN){
			clr_lcd();
			lcd_write_str("   TS ERROR!!",16);
			err_flag=1;
			}
		
		if(cur_pol>POL_MAX || cur_pol<POL_MIN){
			clr_lcd();
			lcd_write_str("   PS ERROR!!",16);
			err_flag=1;
			}
		
		if(err_flag){
			WINDOW_IND = IND_OFF;
			AIV_IND = IND_OFF;
			AC_IND = IND_OFF;
			
			//If error is detected, goes into a loop until values return back to acceptable range 
			while( ( (cur_temp>TEMP_MAX || cur_temp<TEMP_MIN) || (cur_pol>POL_MAX || cur_pol<POL_MIN) ) && !menu_flag){
				WINDOW_IND = !WINDOW_IND;
				AIV_IND = !AIV_IND;
				AC_IND = !AC_IND;
				
				cur_temp=adc_read(TEMP_SEN);
				cur_pol=adc_read(POL_SEN);
				
				delay(50000);
				}	
		
			WINDOW_IND = IND_OFF;
			AIV_IND = IND_OFF;
			AC_IND = IND_OFF;
			
			err_flag=0;			//Error flag is reset after exting from loop
			print_flag = 1;		// Print flag is set => Static content in main display screen need to be reprinted
			
			}
	}
	
	//Print to display the static and dynamic contents on the screen
	void update_display(){
		
		// Print flag is set => Static content in main display screen need to be reprinted
		if(print_flag){
			lcd_write_byte(0x80,CMD);
			lcd_write_str(" TEMP :     ßC ",16);
			lcd_write_byte(0xC0,CMD);
			lcd_write_str(" POL  :     %",16);
			print_flag=0;
		}	
		
			//Dynamic content - updated irrespective of print_flag at every execution
			lcd_write_byte(0x88,CMD);
			lcd_write_num(cur_temp,3);
			lcd_write_byte(0xC8,CMD);
			lcd_write_num(cur_pol,3);
	
	}
	
	//Turns on, off or sets blinking the LED indicators based on the values from sensors and toggle switches
	
	//////////////////////////////////////////////////////////////////////////////////
	//		  \	AC	//				ON				//				OFF				//
	//	WINDOW \ 	//								//								//
	//////////////////////////////////////////////////////////////////////////////////
	//	OPEN		//		window_blink()			//		WINDOW_IND = IND_ON		//
	//////////////////////////////////////////////////////////////////////////////////
	//	CLOSED		//		WINDOW_IND = IND_OFF	//		WINDOW_IND = IND_OFF	//
	//////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//		  \	POL>P_HIGH		//				TRUE			//				FALSE			//
	//	WINDOW \ 				//								//								//
	//////////////////////////////////////////////////////////////////////////////////////////////
	//	OPEN					//		window_blink()			//		WINDOW_IND = IND_ON		//
	//////////////////////////////////////////////////////////////////////////////////////////////
	//	CLOSED					//		WINDOW_IND = IND_OFF	//		WINDOW_IND = IND_OFF	//
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//		  \	POL>P_HIGH		//				TRUE			//				FALSE			//
	//	AIV	   \ 				//								//								//
	//////////////////////////////////////////////////////////////////////////////////////////////
	//	OPEN					//		aiv_blink()				//		AIV_IND = IND_ON		//
	//////////////////////////////////////////////////////////////////////////////////////////////
	//	CLOSED					//		AIV_IND = IND_OFF		//		AIV_IND = IND_OFF		//
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	void update_devices(){
			if(cur_temp>t_high) AC_IND=IND_ON;
			if(cur_temp<t_low) AC_IND=IND_OFF;
		
		
			if(WINDOW==OPEN){
				if(AC_IND==IND_ON || cur_pol>p_high) window_blink();
			
				else {
					window_blink_stop(); WINDOW_IND=IND_ON;
					}
				}		
			else {
				window_blink_stop(); WINDOW_IND=IND_OFF;
			}
		
		
		
			if(AIV==OPEN){
				if(cur_pol<p_high){ aiv_blink_stop(); AIV_IND=IND_ON; }
				else aiv_blink();				
			}
			else{
				aiv_blink_stop(); AIV_IND=IND_OFF;	
		}
		
		}