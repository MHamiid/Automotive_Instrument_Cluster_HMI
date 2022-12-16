/*
 * Timer.c
 *
 * Created: 12/13/2022 11:12:18 AM
 *  Author: MHamiid
 */ 

#include "Timer.h"
#include "../../Config/Config.h"
#include "../../Utilities/registers.h"
#include "../../Utilities/interrupt.h"
#include "../../Utilities/bit.h"
#include <math.h>

#define TIMER_8_BIT_MAX_COUNT 256        // 8-bit timer (2^8 = 256), used for TIMER0 and TIMER2

/* Initialize */
static EN_TimerClockSource_t s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_NONE;
static EN_TimerClockSource_t s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_NONE;
static EN_TimerMode_t s_timer0InitializedMode = TIMER_MODE_NORMAL;
static EN_TimerMode_t s_timer2InitializedMode = TIMER_MODE_NORMAL;

EN_TimerErrorStatus_t timer_init(EN_Timer_t timer, EN_TimerMode_t timerMode, EN_TimerClockSource_t timerClockSource)
{
	switch (timer)
	{
		case TIMER0:
			// Initialize the timer value to zero
			TCNT0 = 0x00;
			
			// Set the timer mode of operation
			if(timerMode == TIMER_MODE_NORMAL)
			{
				// Set timer mode to normal mode
				TCCR0 &= ~(1<<WGM00) & ~(1<<WGM01);
				
                // Set the initialized mode of operation for timer0 to normal mode
				s_timer0InitializedMode = TIMER_MODE_NORMAL;
			}
			else if(timerMode == TIMER_MODE_CTC)
			{
				// Set timer mode to CTC mode
				BIT_CLEAR(TCCR0, WGM00);
				BIT_SET(TCCR0, WGM01);
				
                // Set the initialized mode of operation for timer0 to CTC mode
				s_timer0InitializedMode = TIMER_MODE_CTC;
			}
			else
			{
				// Invalid timer mode of operation error handling
				return TIMER_ERROR_INVALID_TIMER_MODE;
			}
			
			// Set timer clock source
			switch(timerClockSource)
			{
				case TIMER_CLOCK_SOURCE_NO_PRESCALING:
					// Set clock source to clock with no prescaling
					TCCR0 &= ~(1<<CS01) & ~(1<<CS02);
					BIT_SET(TCCR0, CS00);
					
                    // Set the initialized clock source input for timer0
					s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_NO_PRESCALING;
					
					break;
					
				case TIMER_CLOCK_SOURCE_PRESCALER_8:
					// Set clock source to clock with clock/8 prescaler
					TCCR0 &= ~(1<<CS00) & ~(1<<CS02);
					BIT_SET(TCCR0, CS01);
					
					s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_8;
				
					break;
					
				case TIMER_CLOCK_SOURCE_PRESCALER_64:
					// Set clock source to clock with clock/64 prescaler
					TCCR0 |= (1<<CS00) | (1<<CS01);
					BIT_CLEAR(TCCR0, CS02);
					
                    // Set the initialized clock source input for timer0
					s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_64;
					
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_256:
					// Set clock source to clock with clock/256 prescaler
					TCCR0 &= ~(1<<CS00) & ~(1<<CS01);
					BIT_SET(TCCR0, CS02);
					
                    // Set the initialized clock source input for timer0
					s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_256;
					
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_1024:
					// Set clock source to clock with clock/1024 prescaler
					TCCR0 |= (1<<CS00) | (1<<CS02);
					BIT_CLEAR(TCCR0, CS01);
					
                    // Set the initialized clock source input for timer0
					s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_1024;		
					
					break;
				
				case TIMER_CLOCK_SOURCE_EXTERNAL_FALLING_EDGE:
					// Set clock source to external clock on T0 pin. Clock on falling edge
					TCCR0 |= (1<<CS01) | (1<<CS02);
					BIT_CLEAR(TCCR0, CS00);
					
                    // Set the initialized clock source input for timer0
					s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_EXTERNAL_FALLING_EDGE;
					
					break;
				
				case TIMER_CLOCK_SOURCE_EXTERNAL_RISING_EDGE:
					// Set clock source to external clock on T0 pin. Clock on rising edge
					TCCR0 |= (1<<CS00) | (1<<CS01) | (1<<CS02);
					
                    // Set the initialized clock source input for timer0
					s_timer0InitializedClockSource = TIMER_CLOCK_SOURCE_EXTERNAL_RISING_EDGE;
					
					break;
				
				default:
					return TIMER_ERROR_INVALID_TIMER_CLOCK_SOURCE;
			}
			
			return TIMER_ERROR_NONE;
			
		case TIMER2:
			// Initialize the timer value to zero
			TCNT2 = 0x00;
			
			// Set the timer mode of operation
			if(timerMode == TIMER_MODE_NORMAL)
			{
				// Set timer mode to normal mode
				TCCR2 &= ~(1<<WGM20) & ~(1<<WGM21);
				
                // Set the initialized mode of operation for timer2 to normal mode
				s_timer2InitializedMode = TIMER_MODE_NORMAL;
			}
			else if(timerMode == TIMER_MODE_CTC)
			{
				// Set timer mode to CTC mode
				BIT_CLEAR(TCCR2, WGM20);
				BIT_SET(TCCR2, WGM21);
				
                // Set the initialized mode of operation for timer2 to CTC mode
				s_timer2InitializedMode = TIMER_MODE_CTC;
			}
			else
			{
				// Invalid timer mode of operation error handling
				return TIMER_ERROR_INVALID_TIMER_MODE;
			}
			
			// Set timer clock source
			switch(timerClockSource)
			{
				case TIMER_CLOCK_SOURCE_NO_PRESCALING:
					// Set clock source to clock with no prescaling
					TCCR2 &= ~(1<<CS21) & ~(1<<CS22);
					BIT_SET(TCCR2, CS20);
				
                    // Set the initialized clock source input for timer2
					s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_NO_PRESCALING;
				
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_8:
					// Set clock source to clock with clock/8 prescaler
					TCCR2 &= ~(1<<CS20) & ~(1<<CS22);
					BIT_SET(TCCR2, CS21);
				
                    // Set the initialized clock source input for timer2
					s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_8;
				
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_32:
					// Set clock source to clock with clock/64 prescaler
					TCCR2 |= (1<<CS20) | (1<<CS21);
					BIT_CLEAR(TCCR2, CS22);
				
                    // Set the initialized clock source input for timer2
					s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_32;
				
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_64:
					// Set clock source to clock with clock/256 prescaler
					TCCR2 &= ~(1<<CS20) & ~(1<<CS21);
					BIT_SET(TCCR2, CS22);
				
                    // Set the initialized clock source input for timer2
					s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_64;
					
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_128:
					// Set clock source to clock with clock/1024 prescaler
					TCCR2 |= (1<<CS20) | (1<<CS22);
					BIT_CLEAR(TCCR2, CS21);
				
                    // Set the initialized clock source input for timer2
					s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_128;
					
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_256:
					// Set clock source to external clock on T2 pin. Clock on falling edge
					TCCR2 |= (1<<CS21) | (1<<CS22);
					BIT_CLEAR(TCCR2, CS20);
				
                    // Set the initialized clock source input for timer2
					s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_256;
				
					break;
				
				case TIMER_CLOCK_SOURCE_PRESCALER_1024:
					// Set clock source to external clock on T2 pin. Clock on rising edge
					TCCR2 |= (1<<CS20) | (1<<CS21) | (1<<CS22);
				
                    // Set the initialized clock source input for timer2
					s_timer2InitializedClockSource = TIMER_CLOCK_SOURCE_PRESCALER_1024;
					
					break;
				
				default:
					return TIMER_ERROR_INVALID_TIMER_CLOCK_SOURCE;
			}
			
			return TIMER_ERROR_NONE;
							
		default:
			// Timer type error handling
			return TIMER_ERROR_INVALID_TIMER;
	}
}

EN_TimerErrorStatus_t timer_delayMS(EN_Timer_t timer, uint32_t delay_ms)
{
	volatile uint32_t numberOfOverflows = 0;
	volatile uint8_t* timerRegisterPtr = 0;
	volatile uint8_t* outputCompareRegisterPtr = 0;
	uint8_t TOV = 0;	// Overflow flag bit
	uint8_t OCF = 0;	// Output compare flag bit
	EN_TimerClockSource_t timerInitializedClockSource = TIMER_CLOCK_SOURCE_NONE;
	EN_TimerMode_t timerInitializedMode = TIMER_MODE_NORMAL;
	
	/* Select the timer's registers, register's bits, and timer settings */
	if(timer == TIMER0)
	{
		timerRegisterPtr = &TCNT0;
		outputCompareRegisterPtr = &OCR0;
		TOV = 0;
		OCF = 1;
		timerInitializedClockSource = s_timer0InitializedClockSource;
		timerInitializedMode = s_timer0InitializedMode;
	}
	else if(timer == TIMER2)
	{
		timerRegisterPtr = &TCNT2;
		outputCompareRegisterPtr = &OCR2;
		TOV = 6;
		OCF = 7;
		timerInitializedClockSource = s_timer2InitializedClockSource;
		timerInitializedMode = s_timer2InitializedMode;
	}
	else
	{
		// Timer type error handling
		return TIMER_ERROR_INVALID_TIMER;
	}
	
	/**
	* Number of overflows calculations:
	* 1) Convert delay_ms to seconds by dividing by 1000.0 (Note that 1000.0 is a double to make the division results in decimal format)
	* 2) Calculate the time of a single tick ((float)s_timer0InitializedClockSource / F_CPU) 
	* 3) Calculate the maximum time possible for a single overflow (time of a single tick * TIMER0_MAX_COUNT)
	* 4) Divide the delay by the maximum time for a single overflow
	* 5) Get the ceil of the number overflows as we always round up and later on we calculate the initial value for the timer to reach the desired delay
	*/
	numberOfOverflows = ceil((delay_ms / 1000.0) /  (((float)timerInitializedClockSource / F_CPU) * TIMER_8_BIT_MAX_COUNT));
			
	if(timerInitializedMode == TIMER_MODE_NORMAL)
	{
		uint8_t timerInitialValue = 0;
				
		/**
		* Timer initial value calculations:
		* 1) Convert delay_ms to seconds by dividing by 1000.0 (Note that 1000.0 is a double to make the division results in decimal format)
		* 2) Calculate the time of a single tick ((float)s_timer0InitializedClockSource / F_CPU)
		* 3) Calculate the total number of ticks needed to generate the delay
		* 4) Divide that by the number of overflows to get the ticks needed for each single overflow
		* 5) Subtract that from the TIMER0_MAX_COUNT to get the initial value that would get us to count the desired number of ticks before reaching the overflow
		*/
		timerInitialValue = TIMER_8_BIT_MAX_COUNT - (uint16_t)((delay_ms / 1000.0) / (((float)timerInitializedClockSource / F_CPU)) / numberOfOverflows);
		
		// Clear the overflow flag by writing one to the overflow flag bit
		BIT_SET(TIFR, TOV);
		
		for(uint32_t i = 0; i < numberOfOverflows; i++)
		{
			// Set the initial timer value
			*timerRegisterPtr = timerInitialValue;
			
			// Wait for the overflow flag to be set
			while(BIT_READ(TIFR, TOV) != 1);   // Busy wait
			
			// Clear the overflow flag by writing one to the overflow flag bit
			BIT_SET(TIFR, TOV);
		}
	}
	else if(timerInitializedMode == TIMER_MODE_CTC)
	{
		/**
		* Output compare register value calculations:
		* 1) Convert delay_ms to seconds by dividing by 1000.0 (Note that 1000.0 is a double to make the division results in decimal format)
		* 2) Calculate the time of a single tick ((float)s_timer0InitializedClockSource / F_CPU) 
		* 3) Calculate the total number of ticks needed to generate the delay
		* 4) Divide that by the number of overflows to get the ticks needed for each single overflow
		* 5) Subtract (-1) from that value so that would get us to count the desired number of ticks before reaching the overflow, as OCF is set on the next cycle of compare match with the OCR value
		*/
		*outputCompareRegisterPtr = ((delay_ms / 1000.0) / (((float)timerInitializedClockSource / F_CPU)) / numberOfOverflows) - 1;
				
		// Clear timer
		*timerRegisterPtr = 0x00;
		
		// Clear the output compare flag by writing one to the output compare flag bit
		BIT_SET(TIFR, OCF);	
		
		for(uint32_t i = 0; i < numberOfOverflows; i++)
		{
			// Wait for the output compare flag to be set
			while(BIT_READ(TIFR, OCF) != 1);   // Busy wait
				
			// Clear the output compare flag by writing one to the output compare flag bit
			BIT_SET(TIFR, OCF);
		}
	}
	else
	{
		// Invalid timer mode of operation error handling
		return TIMER_ERROR_INVALID_TIMER_MODE;
	}
	
	return TIMER_ERROR_NONE;
}
