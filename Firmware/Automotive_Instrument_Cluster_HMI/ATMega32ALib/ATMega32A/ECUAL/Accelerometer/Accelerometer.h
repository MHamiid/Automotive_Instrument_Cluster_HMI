/*
 * Accelerometer.h
 *
 * Created: 9/26/2022 10:14:19 PM
 *  Author: MHamiid
 */ 


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "../../MCAL/ADC/ADC.h"

/**
 * @brief Initialize the ADC channel that the accelerometer is connected to
 *
 * Avcc pin must be connected to 5 Volts as it is used as Vreference to the ADC
 * 
 * @param channel				ADC single ended input channel that the accelerometer is connected to
 *
 * @return void
 */
void accelerometer_init(EN_ADCChannel_t channel);

/**
 * @brief Read the ADC channel and return the result mapped to the accelerometer range in (g)s [-1.0f g : 1.0f g]
 *
 * @param channel				ADC single ended input channel that the accelerometer is connected to
 *
 * @return void
 */
float accelerometer_read(EN_ADCChannel_t channel);


#endif /* ACCELEROMETER_H_ */