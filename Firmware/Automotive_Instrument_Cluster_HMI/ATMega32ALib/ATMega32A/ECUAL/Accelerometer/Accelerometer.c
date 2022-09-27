/*
 * Accelerometer.c
 *
 * Created: 9/26/2022 10:14:29 PM
 *  Author: MHamiid
 */ 


#include "Accelerometer.h"

void accelerometer_init(EN_ADCChannel_t channel)
{
	ADC_init(channel);
}

float accelerometer_read(EN_ADCChannel_t channel)
{
	// Read the ADC channel and return the result mapped to the accelerometer range in (g)s [-1.0f g : 1.0f g]
	return (((ADC_read(channel) * 2.0) / 1023.0) - 1);
}