/*
 * PWM.c
 *
 * Created: 9/21/2022 5:22:18 AM
 *  Author: MHamiid
 */ 


#include "PWM.h"
#include "../../Utilities/registers.h"
#include "../../Utilities/bit.h"

#define OC0_PIN	3
#define OC2_PIN	7


/* Initialize */
static EN_PWMMode_t s_PWMTimer0InitializedMode = PWM_FAST;
static EN_PWMMode_t s_PWMTimer2InitializedMode = PWM_FAST;

void PWM_init(EN_PWMTimer_t timer, EN_PWMMode_t mode)
{
	switch(timer)
	{
		case PWM_TIMER0:
			if(mode == PWM_FAST)
			{
				// Set Timer0 mode to Fast PWM | Non-inverted mode | Clock with No pre-scaling
				TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
				
				s_PWMTimer0InitializedMode = PWM_FAST;
			}
			else if(mode == PWM_PHASE_CORRECT)
			{
				// Set Timer0 mode to Phase Correct PWM | Non-inverted mode | Clock with No pre-scaling
				TCCR0 |= (1<<WGM00) | (1<<COM01) | (1<<CS00);
				TCCR0 &= ~(1<<WGM01);
				
				s_PWMTimer0InitializedMode = PWM_PHASE_CORRECT;
			}
			
			// Set OC0 pin as output
			BIT_SET(DDRB, OC0_PIN);
						
			break;
			
		case PWM_TIMER2:
			if(mode == PWM_FAST)
			{
				// Set Timer2 mode to Fast PWM | Non-inverted mode | Clock with No pre-scaling
				TCCR2 |= (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS20);
				
				s_PWMTimer2InitializedMode = PWM_FAST;
			}
			else if(mode == PWM_PHASE_CORRECT)
			{
				// Set Timer2 mode to Phase Correct PWM | Non-inverted mode | Clock with No pre-scaling
				TCCR2 |= (1<<WGM20) | (1<<COM21) | (1<<CS20);
				TCCR2 &= ~(1<<WGM21);
				
				s_PWMTimer2InitializedMode = PWM_PHASE_CORRECT;
			}
			
			// Set OC2 pin as output
			BIT_SET(DDRD, OC2_PIN);
				
			break;
	}
}
void PWM_setDutyCycle(uint8_t dutyCycle, EN_PWMTimer_t timer)
{
	switch(timer)
	{
		case PWM_TIMER0:
			if(s_PWMTimer0InitializedMode == PWM_FAST)
			{
				// Duty cycle 0 can't be achieved with PWM_FAST mode (in non-inverted mode)
				if(dutyCycle == 0)
				{
					dutyCycle = 1;
				}
				
				OCR0 = (uint8_t)(((dutyCycle / 100.0) * 256) - 1);
			}
			else if(s_PWMTimer0InitializedMode == PWM_PHASE_CORRECT)
			{
				OCR0 = (uint8_t)((dutyCycle / 200.0) * (2 * (256 - 1)));
			}
			
			break;
			
		case PWM_TIMER2:
			if(s_PWMTimer2InitializedMode == PWM_FAST)
			{
				// Duty cycle 0 can't be achieved with PWM_FAST mode (in non-inverted mode)
				if(dutyCycle == 0)
				{
					dutyCycle = 1;
				}
				
				OCR2 = (uint8_t)(((dutyCycle / 100.0) * 256) - 1);
			}
			else if(s_PWMTimer2InitializedMode == PWM_PHASE_CORRECT)
			{
				OCR2 = (uint8_t)((dutyCycle / 200.0) * (2 * (256 - 1)));
			}
			
			break;
	}
}
