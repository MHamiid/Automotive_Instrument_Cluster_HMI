/*
 * ServoMotor.h
 *
 * Created: 9/25/2022 2:55:36 AM
 *  Author: MHamiid
 */ 


#ifndef SERVOMOTOR_H_
#define SERVOMOTOR_H_

#include "../../MCAL/PWM/PWM.h"
#include <stdint.h>

/**
 * Where Servo Motor:
 *		Minimum Control Pulse = 2 microseconds
 *		Maximum Control Pulse = 508 microseconds
 *		Minimum Angle = -90.0
 *		Maximum Angle = +90.0
*/
#define SERVO_MOTOR_MIN_PWM_VALUE 1			// -90 degree
#define SERVO_MOTOR_MAX_PWM_VALUE 254		// 90 degree


/**
 * @brief Initialize the PWM timer which its (OC) output pin is driving the servo motor
 *
 * PWM timer is initialized in PWM_PHASE_CORRECT correct mode
 * 
 * @param timer					Timer to be used in PWM mode which its (OC) output pin is driving the servo motor
 *
 * @return void
 */
void servoMotor_init(EN_PWMTimer_t timer);

/**
 * @brief Set servo motor rotation angle by outputting the corresponding PWM duty cycle
 *
 * @param rotationAngle			Servo motor rotation angle in range [-90 : 90]
 * @param timer					Timer that is initialized in PWM mode which its (OC) output pin is driving the servo motor
 *
 * @return void
 */
void servoMotor_setRotationAngle(int8_t rotationAngle, EN_PWMTimer_t timer);

/**
 * @brief Set servo motor PWM duty value directly in the (OCRx) register
 *
 * @param rotationAngleValue	Rotation angle PWM duty value in range [SERVO_MOTOR_MIN_PWM_VALUE : SERVO_MOTOR_MAX_PWM_VALUE]
 * @param timer					Timer that is initialized in PWM mode which its (OC) output pin is driving the servo motor
 *
 * @return void
 */
void servoMotor_setRotationAngleValue(uint8_t rotationAngleValue, EN_PWMTimer_t timer);


#endif /* SERVOMOTOR_H_ */