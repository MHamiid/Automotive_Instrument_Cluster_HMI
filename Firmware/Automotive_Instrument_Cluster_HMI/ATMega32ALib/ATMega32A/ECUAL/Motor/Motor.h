/*
 * Motor.h
 *
 * Created: 9/24/2022 4:04:23 PM
 *  Author: MHamiid
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "../../MCAL/ADC/ADC.h"
#include "../../MCAL/PWM/PWM.h"

/**
 * @brief Initialize the ADC channel that controls the motor speed and initialize the PWM timer which its (OC) output pin is driving the motor speed
 *
 * PWM timer is initialized in PWM_PHASE_CORRECT correct mode
 *
 * Avcc pin must be connected to 5 Volts as it is used as Vreference to the ADC
 * 
 * @param channel				ADC single ended input channel that controls the motor speed
 * @param timer					Timer to be used in PWM mode which its (OC) output pin is driving the motor speed
 *
 * @return void
 */
void motor_init(EN_ADCChannel_t channel, EN_PWMTimer_t timer);

/**
 * @brief Read the ADC channel and output the PWM duty cycle respective to the ADC read value
 *
 * PWM duty cycle is at 100% when the ADC channel read is 5 Volts
 *
 * @param channel				ADC single ended input channel that controls the motor speed
 * @param timer					Timer that is initialized in PWM mode which its (OC) output pin is driving the motor speed
 *
 * @return void
 */
void motor_start(EN_ADCChannel_t channel, EN_PWMTimer_t timer);

/**
 * @brief Stop ADC reading and Output 0% duty cycle (0 Volts to the motor) to the PWM pin
 *
 * @param timer					Timer that is initialized in PWM mode which its (OC) output pin is driving the motor speed
 *
 * @return void
 */
void motor_stop(EN_PWMTimer_t timer);

uint8_t motor_getDutyCycle();


#endif /* MOTOR_H_ */