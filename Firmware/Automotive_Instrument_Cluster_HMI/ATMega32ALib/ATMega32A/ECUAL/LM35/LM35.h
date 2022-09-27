/*
 * LM35.h
 *
 * Created: 9/27/2022 3:34:46 AM
 *  Author: MHamiid
 */ 


#ifndef LM35_H_
#define LM35_H_

#include "../../MCAL/ADC/ADC.h"

/**
 * @brief Initialize the ADC channel that the LM35 is connected to
 *
 * Avcc pin must be connected to 5 Volts as it is used as Vreference to the ADC
 * 
 * @param channel				ADC single ended input channel that the LM35 is connected to
 *
 * @return void
 */
void LM35_init(EN_ADCChannel_t channel);

/**
 * @brief Read the ADC channel and return the result converted to Celsius (each 1 Millivolt = 1 Celsius degree)
 *
 * @param channel				ADC single ended input channel that the LM35 is connected to
 *
 * @return LM35 temperature reading in Celsius 
 */
float LM35_read(EN_ADCChannel_t channel);


#endif /* LM35_H_ */