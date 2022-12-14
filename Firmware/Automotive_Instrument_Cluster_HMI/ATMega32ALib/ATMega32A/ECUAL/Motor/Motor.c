/*
 * Motor.c
 *
 * Created: 9/24/2022 4:04:42 PM
 *  Author: MHamiid
 */ 


#include "Motor.h"
#include <math.h>

/* Initialize */
static uint8_t gs_motorDutyCycle = 0;

void motor_init(EN_ADCChannel_t channel, EN_PWMTimer_t timer)
{
	ADC_init(channel);
	PWM_init(timer, PWM_PHASE_CORRECT);
}

void motor_start(EN_ADCChannel_t channel, EN_PWMTimer_t timer)
{
	// Read the 10-bit ADC conversion result
	uint16_t ADCDigitalValue = ADC_read(channel);
	
	// Convert the ADC conversion result to analog
	double ADCAnalogValue = ADCDigitalValue * ADC_STEP;
	
	// Update the current motor duty cycle
	gs_motorDutyCycle = round(((ADCAnalogValue * 100) / (float)ADC_VREF));
	
	// Set the PWM duty cycle of the motor (where 100% duty cycle is at 5 Volts (ADC_VREF) ADC read)
	PWM_setDutyCycle(gs_motorDutyCycle, timer);
}

void motor_stop(EN_PWMTimer_t timer)
{
	// Set the output to the motor to 0% duty cycle (0 Volts)
	PWM_setDutyCycle(0, timer);
}

uint8_t motor_getDutyCycle()
{
	return gs_motorDutyCycle;
}