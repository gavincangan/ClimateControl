#ifndef _ADC_H
#define _ADC_H
	#include "adc.h"
	#include "typedef.h"
	#include "..\\sfr_r823.h"
	
	void adc_config();
	uint16 adc_read(uint8 channel);
#endif