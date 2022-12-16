/*
 * Timer.h
 *
 * Created: 12/13/2022 11:12:09 AM
 *  Author: MHamiid
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

typedef enum EN_Timer_t
{
	TIMER0,
	/* (TODO): Add support for TIMER1 */
	//TIMER1,
	TIMER2
} EN_Timer_t;

typedef enum EN_TimerMode_t
{
	TIMER_MODE_NORMAL,
	TIMER_MODE_CTC
} EN_TimerMode_t;

typedef enum EN_TimerClockSource_t
{
	TIMER_CLOCK_SOURCE_NONE,
	TIMER_CLOCK_SOURCE_NO_PRESCALING,
	TIMER_CLOCK_SOURCE_PRESCALER_8    = 8,
	TIMER_CLOCK_SOURCE_PRESCALER_32   = 32,
	TIMER_CLOCK_SOURCE_PRESCALER_64   = 64,
	TIMER_CLOCK_SOURCE_PRESCALER_128  = 128,
	TIMER_CLOCK_SOURCE_PRESCALER_256  = 256,
	TIMER_CLOCK_SOURCE_PRESCALER_1024 = 1024,
	TIMER_CLOCK_SOURCE_EXTERNAL_FALLING_EDGE,
	TIMER_CLOCK_SOURCE_EXTERNAL_RISING_EDGE
} EN_TimerClockSource_t;

typedef enum EN_TimerErrorStatus_t
{
	TIMER_ERROR_NONE,
	TIMER_ERROR_INVALID_TIMER,
	TIMER_ERROR_INVALID_TIMER_MODE,
	TIMER_ERROR_INVALID_TIMER_CLOCK_SOURCE,
} EN_TimerErrorStatus_t;


/**
 * @brief Initialize timer in the specified mode and set its clock source
 *
 * @param timer                                      Timer to be initialized
 * @timerMode                                        Timer mode of operation
 * @timerClockSource                                 Timer clock source input
 *
 * @return TIMER_ERROR_NONE                          Timer is initialized successfully
 * @return TIMER_ERROR_INVALID_TIMER                 Invalid timer type
 * @return TIMER_ERROR_INVALID_TIMER_MODE            Invalid timer mode or unsupported mode for the specified timer
 * @return TIMER_ERROR_INVALID_TIMER_CLOCK_SOURCE    Invalid timer clock source or unsupported clock source for the specified timer
 */
EN_TimerErrorStatus_t timer_init(EN_Timer_t timer, EN_TimerMode_t timerMode, EN_TimerClockSource_t timerClockSource);

/**
 * @brief Generate a delay by using "Busy Wait"
 *
 * @param timer                                      Timer to be used for delay generation
 * @param delay_ms                                   Delay in milliseconds
 *
 * @return TIMER_ERROR_NONE                          Dlay is generated successfully
 * @return TIMER_ERROR_INVALID_TIMER                 Invalid timer type
 * @return TIMER_ERROR_INVALID_TIMER_MODE            Invalid timer mode or unsupported timer mode for delay generation
 */
EN_TimerErrorStatus_t timer_delayMS(EN_Timer_t timer, uint32_t delay_ms);


#endif /* TIMER_H_ */