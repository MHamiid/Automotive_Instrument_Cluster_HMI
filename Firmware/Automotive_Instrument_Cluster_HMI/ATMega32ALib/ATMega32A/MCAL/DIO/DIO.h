/*
 * DIO.h
 *
 * Created: 9/20/2022 9:09:45 PM
 *  Author: MHamiid
 */ 


#ifndef DIO_H_
#define DIO_H_

#include <stdint.h>


typedef enum EN_DIOPort_t
{
	DIO_PORT_A,
	DIO_PORT_B,
	DIO_PORT_C,
	DIO_PORT_D
} EN_DIOPort_t;

typedef enum EN_DIOPin_t
{
	DIO_PIN_0 = 0,
	DIO_PIN_1,
	DIO_PIN_2,
	DIO_PIN_3,
	DIO_PIN_4,
	DIO_PIN_5,
	DIO_PIN_6,
	DIO_PIN_7,
} EN_DIOPin_t;

typedef enum EN_DIODirection_t
{
	DIO_DIRECTION_INPUT,
	DIO_DIRECTION_OUTPUT
} EN_DIODirection_t;

/* Digital Values */
typedef enum EN_DIODigitalValue_t
{
	DIO_LOW = 0,
	DIO_HIGH = 1
} EN_DIODigitalValue_t;

typedef enum EN_DIOExternalInterrupt_t
{
	DIO_INT0,
	DIO_INT1,
	DIO_INT2
} EN_DIOExternalInterrupt_t;

typedef enum EN_DIOExternalInterruptSense_t
{
	DIO_EXTERNAL_INT_LOW_LEVEL,
	DIO_EXTERNAL_INT_ANY_LOGICAL_CHANGE,
	DIO_EXTERNAL_INT_FALLING_EDGE,
	DIO_EXTERNAL_INT_RISING_EDGE,
} EN_DIOExternalInterruptSense_t;


typedef enum EN_DIOErrorStatus_t
{
	DIO_NO_ERROR,
	DIO_INVALID_PORT,
	DIO_INVALID_PIN_NUMBER,
	DIO_INVALID_DIRECTION,
	DIO_INVALID_DIGITAL_VALUE,
	DIO_INVALID_INTERRUPT_TYPE,
	DIO_INVALID_EXTERNAL_INTERRUPT_SENSE
} EN_DIOErrorStatus_t;



/**
 * @brief Initialize a DIO Pin in the specified Port by setting its direction
 * 
 * @param port							The Port where the Pin is located
 * @param pinNumber						The Pin Number in the Port
 * @param direction						Pin Direction
 *
 * @return DIO_NO_ERROR					Initialization was successful
 * @return DIO_INVALID_PIN_NUMBER		Initialization failed, invalid pin number
 * @return DIO_INVALID_DIRECTION		Initialization failed, invalid direction
 * @return DIO_INVALID_PORT				Initialization failed, invalid port
 */
EN_DIOErrorStatus_t DIO_init(EN_DIOPort_t port, EN_DIOPin_t pinNumber, EN_DIODirection_t direction);

/**
 * @brief Write a digital output value to the DIO Pin in the specified Port
 * 
 * @param port							The port where the pin is located
 * @param pinNumber						The pin number in the port
 * @param digitalValue					Digital output value
 *
 * @return DIO_NO_ERROR					Digital Write was successful
 * @return DIO_INVALID_PIN_NUMBER		Digital Write failed, invalid pin number
 * @return DIO_INVALID_DIGITAL_VALUE	Digital Write failed, invalid digital value
 * @return DIO_INVALID_PORT				Digital Write failed, invalid port
 */
EN_DIOErrorStatus_t DIO_write(EN_DIOPort_t port, EN_DIOPin_t pinNumber, EN_DIODigitalValue_t digitalValue);

/**
 * @brief Toggle the current digital value of a DIO Pin in the specified Port
 * 
 * @param port							The port where the pin is located
 * @param pinNumber						The pin number in the port
 *
 * @return DIO_NO_ERROR					Digital Toggle was successful
 * @return DIO_INVALID_PIN_NUMBER		Digital Toggle failed, invalid pin number
 * @return DIO_INVALID_PORT				Digital Toggle failed, invalid port
 */
EN_DIOErrorStatus_t DIO_toggle(EN_DIOPort_t port, EN_DIOPin_t pinNumber);

/**
 * @brief Read the current digital value of a DIO Pin in the specified Port
 * 
 * @param port							The port where the pin is located
 * @param pinNumber						The pin number in the port
 * @param retDigitalValue				The read digital value
 *
 * @return DIO_NO_ERROR					Digital Read was successful
 * @return DIO_INVALID_PIN_NUMBER		Digital Read failed, invalid pin number
 * @return DIO_INVALID_PORT				Digital Read failed, invalid port
 */
EN_DIOErrorStatus_t DIO_read(EN_DIOPort_t port, EN_DIOPin_t pinNumber, EN_DIODigitalValue_t* retDigitalValue);

/**
 * @brief Enable External Interrupt for the specified DIO external interrupt type with the specified interrupt sense
 * 
 *	Note that (DIO_INT2) has only two possible interrupt senses [ DIO_EXTERNAL_INT_FALLING_EDGE, DIO_EXTERNAL_INT_RISING_EDGE ]
 *
 * @param externalInterruptType						The External Interrupt to be enabled
 * @param externalInterruptSense					Interrupt sense type
 *
 * @return DIO_NO_ERROR								External Interrupt enable was successful
 * @return DIO_INVALID_EXTERNAL_INTERRUPT_SENSE		External Interrupt enable failed, invalid external interrupt sense
 * @return DIO_INVALID_INTERRUPT_TYPE				External Interrupt enable failed, invalid external interrupt type
 */
EN_DIOErrorStatus_t DIO_enableExternalInterrupt(EN_DIOExternalInterrupt_t externalInterruptType, EN_DIOExternalInterruptSense_t externalInterruptSense);

/**
 * @brief Disable External Interrupt for the specified DIO external interrupt type
 * 
 * @param externalInterruptType					The External Interrupt to be disabled
 *
 * @return DIO_NO_ERROR							External Interrupt disable was successful
 * @return DIO_INVALID_INTERRUPT_TYPE			External Interrupt disable failed, invalid external interrupt type
 */
EN_DIOErrorStatus_t DIO_disableExternalInterrupt(EN_DIOExternalInterrupt_t externalInterruptType);

/**
 * @brief Set a callback function to be called inside the ISR for the specified DIO external interrupt type
 * 
 * @param externalInterruptType			The External Interrupt to set the callback function
 * @param callbackFunction				void function will be called when the ISR for the External Interrupt is called
 *
 * @return DIO_NO_ERROR					Setting a callback function for the External Interrupt was successful
 * @return DIO_INVALID_INTERRUPT_TYPE	Setting a callback function for the External Interrupt failed, invalid external interrupt type
 */
EN_DIOErrorStatus_t DIO_setExternalInterruptCallback(EN_DIOExternalInterrupt_t externalInterruptType, void(*callbackFunction)());


#endif /* DIO_H_ */