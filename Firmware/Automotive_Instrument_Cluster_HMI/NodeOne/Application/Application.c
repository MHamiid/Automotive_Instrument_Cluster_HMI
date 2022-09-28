/*
 * Application.c
 *
 * Created: 9/25/2022 6:26:59 PM
 *  Author: MHamiid
 */ 


#include "Application.h"
#include <ATMega32A/MCAL/TWI/TWI.h>
#include <ATMega32A/ECUAL/Motor/Motor.h>
#include <ATMega32A/ECUAL/Accelerometer/Accelerometer.h>
#include <ATMega32A/ECUAL/LM35/LM35.h>

static float gs_accelerometerValue = 0.0f;
static float gs_temperatureValue = 0.0f;

/* Handle TWI interrupts */
static void TWIInterruptCallback()
{
	// Get the current TWI status
	EN_TWI_EVENT_STATUS_t TWI_status = TWI_getStatus();
	
	/* Handle TWI slave logic based on the current TWI_status */
}

void application_init()
{
	// Initialize TWI in slave mode with own slave address 0xA0
	TWI_slave_init(0xA0);
	// Set TWI interrupt callback function
	TWI_setInterruptCallback(TWIInterruptCallback);
	
	motor_init(ADC_CHANNEL_0, PWM_TIMER2);
	accelerometer_init(ADC_CHANNEL_1);
	LM35_init(ADC_CHANNEL_2);
}

void application_loop()
{
	// Start listening for own slave address on the TWI bus and handle TWI status in the interrupt callback function
	TWI_slave_listen(true);
	
	motor_start(ADC_CHANNEL_0, PWM_TIMER2);
	gs_accelerometerValue = accelerometer_read(ADC_CHANNEL_1);
	gs_temperatureValue = LM35_read(ADC_CHANNEL_2);
}
