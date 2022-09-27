/*
 * LM35.c
 *
 * Created: 9/27/2022 3:34:53 AM
 *  Author: MHamiid
 */ 


#include "LM35.h"

void LM35_init(EN_ADCChannel_t channel)
{
	ADC_init(channel);
}

float LM35_read(EN_ADCChannel_t channel)
{
	// Read the ADC channel and return the result converted to Celsius (each 1 Millivolt = 1 Celsius degree)
	return (ADC_read(ADC_CHANNEL_2) * ADC_STEP * 100.0);
}