 #ifndef _HW_SETUP_H
    #define _HW_SETUP_H
    #include "..\\sfr_r823.h"
    	
		//Indicators
		#define WINDOW_IND p2_2
   		#define AC_IND p2_0
   	 	#define AIV_IND p1_7
  	 	
		//Indicators (output) - states - to set
		#define IND_ON 0
    	#define IND_OFF 1
	     
		//Tact and Toggle switches
    	#define NEXT p1_3
    	#define SELECT p1_2
    	#define WINDOW p4_3
    	#define AIV p4_4   
		
		//LCD pins
		#define LCD_RS p0_4
		#define LCD_EN p0_5
		#define LCD_PORT p0	
		
		//Switches (input) - states - to read
		#define OPEN 0
		#define CLOSE 1
	
#endif