#ifndef _LCD_H
    #define _LCD_H
    #include "hw_setup.h"
    #include "typedef.h"
    #include "macros.h"
	#include "..\\sfr_r823.h"
	#include "blink.h"
	
	#define CMD 0
	#define DATA 1
	
	//block_print
	
	//Characters to use in block_print
	#define BLANK_SPACE ' '
	#define FULL_BLOCK 0
	//'='
	#define HALF_BLOCK 1
	//'-'
	
	//Max length of row of blocks and the value to print
	#define ROW_LENGTH 10
	#define VAL_LENGTH 3
	
	//Position to print in second row 0xC0 + column_number
	#define START_POS 0xc0
	#define VAL_POS 0xcb
	
	
    void lcd_write_byte(uint8 value, uint8 reg);
    void lcd_init();
    void lcd_locate(uint8 row, uint8 col);
    void lcd_write_str(const uint8 *str, uint8 len); 
	void lcd_write_num(sint32 a,uint8 len);
	void clr_lcd();
	void lcd_off();
	void block_print(uint8, uint8);
	void lcd_latch();
	void sp_init_lcd();
	void sp_init2_lcd();
	void sym_init_lcd();
#endif