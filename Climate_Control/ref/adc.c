/************************************************************************/
/*																		*/
/*																		*/
/*	FILE        :adc.c													*/
/*	DESCRIPTION :contains adc related functions							*/
/*																		*/
/*	CPU GROUP   :23														*/
/*																		*/
/*																		*/
/************************************************************************/

#include "adc.h"

void adc_config(){
	adcon0=0x00;
	adcon1=0x28;
	adcon2=0x00;
	}

//Reads and returns data from ADC
//Channel sepcified  0->TemperatureSensor  1->PollutionSensor
uint16 adc_read(uint8 channel){
	uint16 value=0;
	adcon0 &= 0xf8; //clear channel area
	adcon0 |= channel;
	adst=1; //start a/d
	while(adst==1); //wait for completion
	value=ad;
	if(value>1000) value=1000; //cap at 1000 - (approx)
	if(channel) return (value/10); //pollution 0 to 1024 -> 0 to 100
	else return ( (value/20) - 10); //temperature 0-1024  -> -10 to 40
	}