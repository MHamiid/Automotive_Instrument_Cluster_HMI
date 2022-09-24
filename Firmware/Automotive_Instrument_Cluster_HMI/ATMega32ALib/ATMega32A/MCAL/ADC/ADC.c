/*
 * ADC.c
 *
 * Created: 9/23/2022 5:15:43 PM
 *  Author: MHamiid
 */ 


#include "ADC.h"
#include "../../Utilities/registers.h"
#include "../../Utilities/bit.h"

void ADC_init(EN_ADCChannel_t channel)
{
	// Set ADC channel pin to be input
	BIT_CLEAR(DDRA, channel);
	
	// Select single ended input channel
	//ADMUX |= (channel<<MUX0);
	
	// Set Vref: Avcc
	BIT_SET(ADMUX, REFS0);
	BIT_CLEAR(ADMUX, REFS1);
	
	// Set ADC result to be right adjusted
	BIT_CLEAR(ADMUX, ADLAR);
	
	// Enable ADC (with (/2) pre-scaler)
	BIT_SET(ADCSRA, ADEN);
}

uint16_t ADC_read(EN_ADCChannel_t channel)
{
	uint16_t ADCResult = 0;
	
	// Select ADC single ended input channel to read
	ADMUX |= (channel<<MUX0);
	
	// Start ADC conversion
	BIT_SET(ADCSRA, ADSC);
	
	// Wait until conversion is complete
	while(BIT_READ(ADCSRA, ADIF) != 1);		// Busy wait
	
	// Read the ADC conversion 10-bit digital result
	ADCResult = ADCL;
	ADCResult |= (ADCH << 8);
	
	return ADCResult;
}