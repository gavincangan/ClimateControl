//lcd.c

#include "lcd.h"

	// Set to 1 if there has been modifications in the LCD display
	// To enable printing the 'text' part in next iteration
	extern uint8 print_flag;

	//Writes one byte data/command to LCD display
	//reg: Command ->CMD/0 Data->DATA/1
	void lcd_write_byte(uint8 value, uint8 reg){
       	//p0_5 = 1;
	    LCD_PORT &= 0xc0;	//Clearing the first 6 bits
        LCD_RS = reg;		//Command->0 or Data->1
        LCD_PORT |= value >> 4;	//Write the lower nibble
		lcd_latch();
        
		LCD_PORT &= 0xc0;	//Clearing the last 6 bits
        LCD_RS = reg;		//Command->0 or Data->1
        LCD_PORT |= value & 0x0F;	//Write the higher nibble
        lcd_latch();
		
		delay(1000);		//LCD wait time
    }
	
	//Falling edge at EN PIN of LCD display
	//Latch after writing four bits
	void lcd_latch(){
		LCD_EN = 1;
		LCD_EN = 0;
		}
    
	
    void lcd_init(){
        // function mode selection
        lcd_write_byte(0x28,CMD);  // 4 bit IO, 2 lines, 5x7 font
        lcd_write_byte(0x28,CMD);
        lcd_write_byte(0x28,CMD);
		
        // entry mode selection
        lcd_write_byte(0x06,CMD);  // auto increment after write, No shifting
        lcd_write_byte(0x0c,CMD);  // cursor off, display on
        lcd_write_byte(0x01,CMD);  // clear screen (optional)
        }
    
	//Clears the LCD display
	void clr_lcd(){
		lcd_write_byte(0x01,CMD);
		print_flag=1;
		}
	
	//Move cursor to location in LCD display
	//0x80 -> DDRAM address for 0,0
	//0xC0 -> DDRAM address for 1,0
    void lcd_locate(uint8 row, uint8 col){
        if(col>15)return;
        if(row=0)lcd_write_byte(0x80+col,CMD);
        if(row=1)lcd_write_byte(0xc0+col,CMD);
		}
	
	//To print strings in LCD
	//Length may be greater or less than string length
	//If length is smaller string will be truncated in printng
	//If length is larger the additional length will be filled with ' '
    void lcd_write_str(const uint8 *str, uint8 len){
        uint8 i=0;
        for(;str[i] && i<len;i++)
            lcd_write_byte(str[i],DATA);
        for(;i<len;i++)
            lcd_write_byte(' ',DATA);            
        }
	
	//used to print numbers in LCD
	//takes number and length to clear in LCD as arguments
	//Assummptions	->	only 32-bit numbers
	//				->	max 11 digits excluding sign (if applicable)
	//Warning: length must be greater than no. of digits in the number to be printed
	//No truncation in number will be made in number if otherwise
	//Length is only used to clear the space in LCD after printing the number
	void lcd_write_num(sint32 a,uint8 len){
		uint8 ch[11]={0};
		sint8 i=0,j=0;
			if(a<0){lcd_write_byte('-',DATA);a=~a; a++;}
			if(!a) {
				lcd_write_byte('0',DATA);
			    for(j=1;j<len-1;j++)
            	lcd_write_byte(' ',DATA); return;
				}
			for(i=0;a;i++){
				ch[i]=(a%10)+'0';
				a=a/10;
				}
			j=--i;
			for(;i>=0;i--)
				lcd_write_byte(ch[i],DATA);
			for(;j<len-1;j++)
            	lcd_write_byte(' ',DATA);
	}
	
	//to display progress
	//get value and data_type
	//data_type  0->temperature   1->pollution
	void block_print(uint8 value, uint8 data_type){
		uint8 band_gap=0;
		uint8 value_per_block=0;
		uint8 no_of_full_blocks=0,blocks_printed=0;
		//value-=lower_bound;		//removed because lower_bound is 0 by default
		sym_init_lcd();
		
		if(data_type==0){
			band_gap=TEMP_MAX - TEMP_MIN;
			lcd_write_byte(0x80,CMD); lcd_write_str("Temperature : ",16);
		}
		if(data_type==1){
			band_gap=POL_MAX - POL_MIN;
			lcd_write_byte(0x80,CMD); lcd_write_str("Pollution : ",16);
		}
		
		value_per_block=band_gap/ROW_LENGTH;
		no_of_full_blocks = value/value_per_block;
				
		lcd_write_byte(START_POS,CMD);
		for(;blocks_printed<16;blocks_printed++)
		lcd_write_byte(BLANK_SPACE,DATA);
		
		blocks_printed=0;
		lcd_write_byte(START_POS,CMD);
		while(blocks_printed<no_of_full_blocks){
			lcd_write_byte(FULL_BLOCK,DATA);
			blocks_printed++;
		}
		if(value%value_per_block > value_per_block/2 ){
			lcd_write_byte(HALF_BLOCK,DATA);
			blocks_printed++;
		}
		while(blocks_printed<ROW_LENGTH){
			lcd_write_byte(BLANK_SPACE,DATA);
			blocks_printed++;
		}
		lcd_write_byte(VAL_POS,CMD);
		
		if(value<100)	lcd_write_byte(' ',DATA);
		if(value<10)	lcd_write_byte(' ',DATA);
		
		lcd_write_num(value,VAL_LENGTH);
		lcd_write_byte(0xce,CMD);
		if(data_type==0)lcd_write_str("ßC",2);
		if(data_type==1)lcd_write_str(" %",2);
	}
	
	void sp_init2_lcd(){
		uint8 count=0;
		uint8 val_array[64]={	0x6,0xa,0x13,0x12,0x12,0x13,0xa,0x6,
								0xc,0xa,0x19,0x9,0x9,0x19,0xa,0xc,
								0x1e,0x1b,0x1b,0x1e,0x1b,0x1b,0x1e,0x0,
								0xe,0x1b,0x1b,0x1b,0x1b,0x1b,0xe,0x0,
								0xf,0x18,0x1e,0xf,0x3,0x1b,0x1e,0x0,
								0xe,0x1b,0x18,0x18,0x18,0x1b,0x1e,0x0,
								0x1b,0x1b,0x1b,0x1f,0x1b,0x1b,0x1b,0x0
								};
			lcd_write_byte(0x40,CMD);
			for(;count<64;count++){
				lcd_write_byte(val_array[count],DATA);
				//if(!(count&7)) clr_lcd();
			}	
		clr_lcd();
	}
	
	void sp_init_lcd(){
		sint8 count=0;
		uint8 val_array[24]={	0x0,0x3,0x4,0x8,0x9,0x13,0x15,0x15,
								0x1f,0x0,0x0,0x0,0x0,0x0,0x1f,0x0,
								0x0,0x18,0x4,0x2,0x12,0x19,0x15,0x15
								};
			lcd_write_byte(0x40,CMD);
			for(;count<24;count++)
				lcd_write_byte(val_array[count],DATA);
			for(count=23;count>=0;count--)
				lcd_write_byte(val_array[count],DATA);
				//if(!(count&7)) clr_lcd();	
		clr_lcd();
	}
	
	void sym_init_lcd(){
		sint8 count=0;
		uint8 val_array[64]={	0x1f,0x0,0x1f,0x1f,0x1f,0x1f,0x0,0x1f,
								0x1c,0x04,0x14,0x14,0x14,0x14,0x04,0x1c,
								};
		lcd_write_byte(0x40,CMD);
		for(;count<64;count++)
			lcd_write_byte(val_array[count],DATA);
		clr_lcd();
	}

/**/