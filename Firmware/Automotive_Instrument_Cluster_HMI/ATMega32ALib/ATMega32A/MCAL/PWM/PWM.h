/*
 * PWM.h
 *
 * Created: 9/21/2022 5:21:56 AM
 *  Author: MHamiid
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

typedef enum EN_PWMTimer_t
{
	PWM_TIMER0,
	/* (TODO): Add support for PWM_TIMER1 */
	//PWM_TIMER1,
	PWM_TIMER2
} EN_PWMTimer_t;

typedef enum EN_PWMMode_t
{
	PWM_FAST,
	PWM_PHASE_CORRECT
} EN_PWMMode_t;


/**
 * @brief Initialize the specified timer in the specified PWM mode with non-inverted mode and clock with no pre-scaling
 * 
 * @param timer					Timer to be used in PWM mode
 * @param mode					PWM mode
 *
 * @return void
 */
void PWM_init(EN_PWMTimer_t timer, EN_PWMMode_t mode);

/**
 * @brief Set the duty cycle for the specified Timer which has been initialized in a PWM mode
 * 
 * In PWM_FAST mode (0%) duty cycle can't be achieved and the duty cycle will be mapped to (1%)
 *
 * @param dutyCycle				Duty cycle percentage in range [0 : 100]
 * @param timer					Timer that is initialized in a PWM mode
 *
 * @return void
 */
void PWM_setDutyCycle(uint8_t dutyCycle, EN_PWMTimer_t timer);


#endif /* PWM_H_ */
