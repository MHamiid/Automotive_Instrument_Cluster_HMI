/*
 * Application.c
 *
 * Created: 9/25/2022 6:26:59 PM
 *  Author: MHamiid
 */ 


#include "Application.h"
#include <ATMega32A/ECUAL/Motor/Motor.h>
#include <ATMega32A/ECUAL/Accelerometer/Accelerometer.h>
#include <ATMega32A/ECUAL/LM35/LM35.h>

static float gs_accelerometerValue = 0.0f;
static float gs_temperatureValue = 0.0f;

void application_init()
{
	motor_init(ADC_CHANNEL_0, PWM_TIMER2);
	accelerometer_init(ADC_CHANNEL_1);
	LM35_init(ADC_CHANNEL_2);
}

void application_loop()
{
	motor_start(ADC_CHANNEL_0, PWM_TIMER2);
	gs_accelerometerValue = accelerometer_read(ADC_CHANNEL_1);
	gs_temperatureValue = LM35_read(ADC_CHANNEL_2);
}
