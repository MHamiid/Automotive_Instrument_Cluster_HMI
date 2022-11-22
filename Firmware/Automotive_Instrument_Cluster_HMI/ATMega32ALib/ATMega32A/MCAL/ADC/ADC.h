/*
 * ADC.h
 *
 * Created: 9/23/2022 5:15:19 PM
 *  Author: MHamiid
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define ADC_VREF	5						// 5 Volts
#define ADC_STEP	(ADC_VREF / 1024.0)		// ADC_VREF / 2^10 (As the ADC is a 10-bit ADC) 

typedef enum EN_ADCChannel_t
{
	ADC_CHANNEL_0 = 0,
	ADC_CHANNEL_1 = 1,
	ADC_CHANNEL_2 = 2,
	ADC_CHANNEL_3 = 3,
	ADC_CHANNEL_4 = 4,
	ADC_CHANNEL_5 = 5,
	ADC_CHANNEL_6 = 6,
	ADC_CHANNEL_7 = 7
} EN_ADCChannel_t;


/**
 * @brief Initialize the ADC with the specified single ended input channel
 *
 * Use Avcc as Vreference, clock with (/2) pre-scaling, and ADC conversion result to be right adjusted
 * 
 * @param channel				ADC single ended input channel
 *
 * @return void
 */
void ADC_init(EN_ADCChannel_t channel);

/**
 * @brief Read the ADC conversion result for the specified channel
 *
 * Function will not exit until ADC conversion is complete **Uses Busy Wait**
 *
 * @param channel				ADC single ended input channel
 *
 * @return						10-bit ADC conversion result
 */
uint16_t ADC_read(EN_ADCChannel_t channel);


#endif /* ADC_H_ */