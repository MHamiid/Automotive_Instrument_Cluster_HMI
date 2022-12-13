/*
 * DIO.c
 *
 * Created: 9/20/2022 9:09:55 PM
 *  Author: MHamiid
 */ 

#include "DIO.h"
#include "../../Utilities/registers.h"
#include "../../Utilities/interrupt.h"
#include "../../Utilities/bit.h"

/* External Interrupts Callback Functions */
static void(*INT0_CALLBACK_FUNCTION)() = 0;
static void(*INT1_CALLBACK_FUNCTION)() = 0;
static void(*INT2_CALLBACK_FUNCTION)() = 0;

EN_DIOErrorStatus_t DIO_init(EN_DIOPort_t port, EN_DIOPin_t pinNumber, EN_DIODirection_t direction)
{
	// Validate the pin number
	if(!(pinNumber >= DIO_PIN_0 && pinNumber <= DIO_PIN_7))
	{
		return DIO_ERROR_INVALID_PIN_NUMBER;
	}
	
	switch(port)
	{
		case DIO_PORT_A:
			if(direction == DIO_DIRECTION_INPUT)
			{
				BIT_CLEAR(DDRA, pinNumber);    // Set pin direction to input
				return DIO_ERROR_NONE;
			}
			else if(direction == DIO_DIRECTION_OUTPUT)
			{
				BIT_SET(DDRA, pinNumber);      // Set pin direction to output
				return DIO_ERROR_NONE;
			}
			else
			{
				// Direction error handling
				return DIO_ERROR_INVALID_DIRECTION;
			}
		
		case DIO_PORT_B:
			if(direction == DIO_DIRECTION_INPUT)
			{
				BIT_CLEAR(DDRB, pinNumber);    // Set pin direction to input
				return DIO_ERROR_NONE;
			}
			else if(direction == DIO_DIRECTION_OUTPUT)
			{
				BIT_SET(DDRB, pinNumber);      // Set pin direction to output
				return DIO_ERROR_NONE;
			}
			else
			{
				// Direction error handling
				return DIO_ERROR_INVALID_DIRECTION;
			}
		
		case DIO_PORT_C:
			if(direction == DIO_DIRECTION_INPUT)
			{
				BIT_CLEAR(DDRC, pinNumber);    // Set pin direction to input
				return DIO_ERROR_NONE;
			}
			else if(direction == DIO_DIRECTION_OUTPUT)
			{
				BIT_SET(DDRC, pinNumber);      // Set pin direction to output
				return DIO_ERROR_NONE;
			}
			else
			{
				// direction Error handling
				return DIO_ERROR_INVALID_DIRECTION;
			}
		
		case DIO_PORT_D:
			if(direction == DIO_DIRECTION_INPUT)
			{
				BIT_CLEAR(DDRD, pinNumber);    // Set pin direction to input
				return DIO_ERROR_NONE;
			}
			else if(direction == DIO_DIRECTION_OUTPUT)
			{
				BIT_SET(DDRD, pinNumber);      // Set pin direction to output
				return DIO_ERROR_NONE;
			}
			else
			{
				// Direction error handling
				return DIO_ERROR_INVALID_DIRECTION;
			}
		
		default:
			// Port Number error handling
			return DIO_ERROR_INVALID_PORT;
	}
}

EN_DIOErrorStatus_t DIO_write(EN_DIOPort_t port, EN_DIOPin_t pinNumber, EN_DIODigitalValue_t digitalValue)
{
	// Validate the pin number
	if(!(pinNumber >= DIO_PIN_0 && pinNumber <= DIO_PIN_7))
	{
		return DIO_ERROR_INVALID_PIN_NUMBER;
	}
	
	switch(port)
	{
		case DIO_PORT_A:
			if(digitalValue == DIO_LOW)
			{
				BIT_CLEAR(PORTA, pinNumber);    // Write 0 to the pin output
				return DIO_ERROR_NONE;
			}
			else if(digitalValue == DIO_HIGH)
			{
				BIT_SET(PORTA, pinNumber);      // Write 1 to the pin output
				return DIO_ERROR_NONE;
			}
			else
			{
				// Digital output value error handling
				return DIO_ERROR_INVALID_DIGITAL_VALUE;
			}
		
		case DIO_PORT_B:
			if(digitalValue == DIO_LOW)
			{
				BIT_CLEAR(PORTB, pinNumber);    // Write 0 to the pin output
				return DIO_ERROR_NONE;
			}
			else if(digitalValue == DIO_HIGH)
			{
				BIT_SET(PORTB, pinNumber);      // Write 1 to the pin output
				return DIO_ERROR_NONE;
			}
			else
			{
				// Digital value error handling
				return DIO_ERROR_INVALID_DIGITAL_VALUE;
			}
		
		case DIO_PORT_C:
			if(digitalValue == DIO_LOW)
			{
				BIT_CLEAR(PORTC, pinNumber);    // Write 0 to the pin output
				return DIO_ERROR_NONE;
			}
			else if(digitalValue == DIO_HIGH)
			{
				BIT_SET(PORTC, pinNumber);      // Write 1 to the pin output
				return DIO_ERROR_NONE;
			}
			else
			{
				// Digital output value error handling
				return DIO_ERROR_INVALID_DIGITAL_VALUE;
			}
		
		case DIO_PORT_D:
			if(digitalValue == DIO_LOW)
			{
				BIT_CLEAR(PORTD, pinNumber);    // Write 0 to the pin output
				return DIO_ERROR_NONE;
			}
			else if(digitalValue == DIO_HIGH)
			{
				BIT_SET(PORTD, pinNumber);      // Write 1 to the pin output
				return DIO_ERROR_NONE;
			}
			else
			{
				// Digital output value error handling
				return DIO_ERROR_INVALID_DIGITAL_VALUE;
			}
		
		default:
			// Port Number error handling
			return DIO_ERROR_INVALID_PORT;
	}
}

EN_DIOErrorStatus_t DIO_toggle(EN_DIOPort_t port, EN_DIOPin_t pinNumber)
{
	// Validate the pin number
	if(!(pinNumber >= DIO_PIN_0 && pinNumber <= DIO_PIN_7))
	{
		return DIO_ERROR_INVALID_PIN_NUMBER;
	}
	
	switch(port)
	{
		case DIO_PORT_A:
			BIT_TOGGLE(PORTA, pinNumber);     // Toggle the current pin state
			return DIO_ERROR_NONE;
		
		case DIO_PORT_B:
			BIT_TOGGLE(PORTB, pinNumber);     // Toggle the current pin state
			return DIO_ERROR_NONE;
		
		case DIO_PORT_C:
			BIT_TOGGLE(PORTC, pinNumber);     // Toggle the current pin state
			return DIO_ERROR_NONE;
		
		case DIO_PORT_D:
			BIT_TOGGLE(PORTD, pinNumber);     // Toggle the current pin state
			return DIO_ERROR_NONE;
		
		default:
			// Port Number error handling
			return DIO_ERROR_INVALID_PORT;
	}
}

EN_DIOErrorStatus_t DIO_read(EN_DIOPort_t port, EN_DIOPin_t pinNumber, EN_DIODigitalValue_t* retDigitalValue)
{
	// Validate the pin number
	if(!(pinNumber >= DIO_PIN_0 && pinNumber <= DIO_PIN_7))
	{
		return DIO_ERROR_INVALID_PIN_NUMBER;
	}
	
	switch(port)
	{
		case DIO_PORT_A:
			*retDigitalValue = BIT_READ(PINA, pinNumber);		// Read pin bit ===> 0 (DIO_LOW) or 1 (DIO_HIGH)
			return DIO_ERROR_NONE;
		
		case DIO_PORT_B:
			*retDigitalValue = BIT_READ(PINB, pinNumber);		// Read pin bit ===> 0 (DIO_LOW) or 1 (DIO_HIGH)
			return DIO_ERROR_NONE;
		
		case DIO_PORT_C:
			*retDigitalValue = BIT_READ(PINC, pinNumber);		// Read pin bit ===> 0 (DIO_LOW) or 1 (DIO_HIGH)
			return DIO_ERROR_NONE;
		
		case DIO_PORT_D:
			*retDigitalValue = BIT_READ(PIND, pinNumber);		// Read pin bit ===> 0 (DIO_LOW) or 1 (DIO_HIGH)
			return DIO_ERROR_NONE;
		
		default:
			// Port Number error handling
			return DIO_ERROR_INVALID_PORT;
	}
}

/*
* Currently this function is just limited to sense interrupts on rising edge for simplicity
*/
EN_DIOErrorStatus_t DIO_enableExternalInterrupt(EN_DIOExternalInterrupt_t externalInterruptType, EN_DIOExternalInterruptSense_t externalInterruptSense)
{
	switch(externalInterruptType)
	{
		case DIO_INT0:
			/* Initialize External Interrupts for INT0 */
		
			// Enable global interrupts
			sei();
		
			/* Set The External Interrupt Sense */
			if(externalInterruptSense == DIO_EXTERNAL_INT_LOW_LEVEL)
			{
				BIT_CLEAR(MCUCR, ISC00);
				BIT_CLEAR(MCUCR, ISC01);
			}
			else if(externalInterruptSense == DIO_EXTERNAL_INT_ANY_LOGICAL_CHANGE)
			{
				BIT_SET(MCUCR, ISC00);
				BIT_CLEAR(MCUCR, ISC01);
			}
			else if(externalInterruptSense == DIO_EXTERNAL_INT_FALLING_EDGE)
			{
				BIT_CLEAR(MCUCR, ISC00);
				BIT_SET(MCUCR, ISC01);
			}
			else if(externalInterruptSense == DIO_EXTERNAL_INT_RISING_EDGE)
			{
				BIT_SET(MCUCR, ISC00);
				BIT_SET(MCUCR, ISC01);
			}
			else
			{
				return DIO_ERROR_INVALID_EXTERNAL_INTERRUPT_SENSE;
			}
			
			// Enable external interrupt 0 - INT0
			BIT_SET(GICR, INT0);
		
			return DIO_ERROR_NONE;
		
		case DIO_INT1:
			/* Initialize External Interrupts for INT1 */

			// Enable global interrupts
			sei();
		
			/* Set The External Interrupt Sense */
			if(externalInterruptSense == DIO_EXTERNAL_INT_LOW_LEVEL)
			{
				BIT_CLEAR(MCUCR, ISC10);
				BIT_CLEAR(MCUCR, ISC11);
			}
			else if(externalInterruptSense == DIO_EXTERNAL_INT_ANY_LOGICAL_CHANGE)
			{
				BIT_SET(MCUCR, ISC10);
				BIT_CLEAR(MCUCR, ISC11);
			}
			else if(externalInterruptSense == DIO_EXTERNAL_INT_FALLING_EDGE)
			{
				BIT_CLEAR(MCUCR, ISC10);
				BIT_SET(MCUCR, ISC11);
			}
			else if(externalInterruptSense == DIO_EXTERNAL_INT_RISING_EDGE)
			{
				BIT_SET(MCUCR, ISC10);
				BIT_SET(MCUCR, ISC11);
			}
			else
			{
				return DIO_ERROR_INVALID_EXTERNAL_INTERRUPT_SENSE;
			}
			
			// Enable external interrupt 1 - INT1
			BIT_SET(GICR, INT1);
		
			return DIO_ERROR_NONE;
		
		case DIO_INT2:
			/* Initialize External Interrupts for INT2 */
		
			// Enable global interrupts
			sei();
		
			/* Set The External Interrupt Sense */
			if(externalInterruptSense == DIO_EXTERNAL_INT_FALLING_EDGE)
			{
				BIT_CLEAR(MCUCSR, ISC2);
			}
			else if(externalInterruptSense == DIO_EXTERNAL_INT_RISING_EDGE)
			{
				BIT_SET(MCUCSR, ISC2);
			}
			else
			{
				return DIO_ERROR_INVALID_EXTERNAL_INTERRUPT_SENSE;
			}
		
			// Enable external interrupt 2 - INT2
			BIT_SET(GICR, INT2);
		
			return DIO_ERROR_NONE;
		
		default:
			// External interrupt type error handling
			return DIO_ERROR_INVALID_INTERRUPT_TYPE;
	}
}

EN_DIOErrorStatus_t DIO_disableExternalInterrupt(EN_DIOExternalInterrupt_t externalInterruptType)
{
	switch(externalInterruptType)
	{
		case DIO_INT0:
			// Disable external interrupt 0 - INT0
			BIT_CLEAR(GICR, INT0);
		
			return DIO_ERROR_NONE;
		
		case DIO_INT1:
			// Disable external interrupt 1 - INT1
			BIT_CLEAR(GICR, INT1);
		
			return DIO_ERROR_NONE;
		
		case DIO_INT2:
			// Disable external interrupt 2 - INT2
			BIT_CLEAR(GICR, INT2);
		
			return DIO_ERROR_NONE;
		
		default:
			// External interrupt type error handling
			return DIO_ERROR_INVALID_INTERRUPT_TYPE;
	}
}

EN_DIOErrorStatus_t DIO_setExternalInterruptCallback(EN_DIOExternalInterrupt_t externalInterruptType, void(*callbackFunction)())
{
	switch(externalInterruptType)
	{
		case DIO_INT0:
			INT0_CALLBACK_FUNCTION = callbackFunction;
			
			return DIO_ERROR_NONE;
			
		case DIO_INT1:
			INT1_CALLBACK_FUNCTION = callbackFunction;
			
			return DIO_ERROR_NONE;
			
		case DIO_INT2:
			INT2_CALLBACK_FUNCTION = callbackFunction;
			
			return DIO_ERROR_NONE;
		
		default:
			// External interrupt type error handling
			return DIO_ERROR_INVALID_INTERRUPT_TYPE;
	}
}

ISR(EXT_INT_0_VECTOR)
{
	INT0_CALLBACK_FUNCTION();
}

ISR(EXT_INT_1_VECTOR)
{
	INT1_CALLBACK_FUNCTION();
}

ISR(EXT_INT_2_VECTOR)
{
	INT2_CALLBACK_FUNCTION();
}