/*
 * ServoMotor.c
 *
 * Created: 9/25/2022 2:55:47 AM
 *  Author: MHamiid
 */ 


#include "ServoMotor.h"
#include <math.h>


void ServoMotor_init(EN_PWMTimer_t timer)
{
	PWM_init(timer, PWM_PHASE_CORRECT);
}

void ServoMotor_setRotationAngle(int8_t rotationAngle, EN_PWMTimer_t timer)
{
	// Map the range from [-90 : 90] to [0 : 180] (to get rid of negative values). Then map the range from [0 : 180] to [ SERVO_MOTOR_MIN_PWM_VALUE : SERVO_MOTOR_MAX_PWM_VALUE ]
	uint8_t dutyValue = round(((uint16_t)SERVO_MOTOR_MAX_PWM_VALUE * (rotationAngle + 90)) / 180.0);	// Casting to (uint16_t) to avoid arithmetic overflow which resulting in incorrect arithmetic values
	// We check for an edge case before setting the duty value where rotationAngle == -90 (aka dutyValue == 0). As dutyValue == 0 means no duty cycle which means no servo movement
	PWM_setDutyValue(rotationAngle == -90 ? SERVO_MOTOR_MIN_PWM_VALUE : dutyValue, timer);
}

void ServoMotor_setRotationAngleValue(uint8_t rotationAngleValue, EN_PWMTimer_t timer)
{
	PWM_setDutyValue(rotationAngleValue, timer);
}