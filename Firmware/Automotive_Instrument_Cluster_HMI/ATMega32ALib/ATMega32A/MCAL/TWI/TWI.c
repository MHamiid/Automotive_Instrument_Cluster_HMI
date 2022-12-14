/*
 * TWI.c
 *
 * Created: 9/14/2022 9:47:34 PM
 *  Author: MHamiid
 */ 

#include "TWI.h"
#include "../../Config/Config.h"
#include "../../Utilities/registers.h"
#include "../../Utilities/interrupt.h"
#include <math.h>

#ifndef F_CPU
/* prevent compiler error by supplying a default F_CPU */
# warning "F_CPU not defined for <ATMega32A/MCAL/TWI/TWI.c>. Using 1 MHz as a default F_CPU. Maybe try to #include <Config/Config.h>"

/**
  * The value 1 MHz only provided as a fall back 
  * if no user-provided definition could be found
*/
# define F_CPU 1000000UL
#endif

/* TWI Interrupt Callback Function */
static void(*TWI_INTERRUPT_CALLBACK_FUNCTION)() = 0;		// Initialize

/**
 * Calculate the value for the TWSR register from the SCL frequency, F_CPU, and the TWI pre-scaler(TWPS0, TWPS1)
*/
#define TWBR_FROM_SCL_FREQUENCY(SCL_FREQUENCY, TWSR) ((F_CPU / SCL_FREQUENCY) - 16) / (2 * pow(4, (TWSR & ((1<<TWPS0) | (1<<TWPS1)))))

void TWI_master_init(uint32_t SCLFrequency)
{
	// Prevent exceeding the maximum SCL frequency
	if(SCLFrequency > 400000)
	{
		SCLFrequency = 400000;
	}
	
	// Clear the TWI status bits and set the TWI pre-scaler to 1
	TWSR = 0x00;
	
	// Calculate TWBR value from SCL frequency and TWI pre-scaler value
	TWBR  = TWBR_FROM_SCL_FREQUENCY(SCLFrequency, TWSR);
}

void TWI_slave_init(uint8_t slaveAddress)
{
	// Clear the TWI status bits
	TWSR &= ~(TWI_TWSR_STATUS_BITS_MASK);
	
	// Set the slave address in TWI slave register
	TWAR = slaveAddress;
}

EN_TWI_EVENT_STATUS_t TWI_master_start(bool interruptHandled)
{
	// Clear TWINT flag to start a new event operation | Generate start condition event | Enable TWI
	TWCR |= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}
	
	// Wait until TWI finish its current job/event
	while(!(TWCR & (1<<TWINT)));		// Busy wait
	
	// Check the TWI status
	if((TWSR & TWI_TWSR_STATUS_BITS_MASK) == TWI_START_SENT_STATE)
	{
		return TWI_START_SENT;
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
}

EN_TWI_EVENT_STATUS_t TWI_master_repeatedStart(bool interruptHandled)
{
	// Clear TWINT flag to start a new event operation | Generate start condition event | Enable TWI
	TWCR |= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}	
	
	// Wait until TWI finish its current job/event
	while(!(TWCR & (1<<TWINT)));		// Busy wait
	
	// Check the TWI status
	if((TWSR & TWI_TWSR_STATUS_BITS_MASK) == TWI_REPEATED_START_SENT_STATE)
	{
		return TWI_REPEATED_START_SENT;
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
	
}

EN_TWI_EVENT_STATUS_t TWI_master_transmitSlaveAddress(uint8_t slaveAddress, uint8_t rw, bool interruptHandled)
{
	uint8_t TWI_status = 0;
	
	// Write the 7-bits slave address + R/W bit in TWI data register - data **MUST** be written to TWDR before clearing TWINT
	if(rw == TWI_WRITE_BIT)
	{
		TWDR = slaveAddress;		// Write slave address + write(0) in TWI data register
	}
	else if(rw == TWI_READ_BIT)
	{
		TWDR = slaveAddress + 1;	// Write slave address + read(1) in TWI data register
	}
	else
	{
		// Invalid Read/Write bit
		return TWI_INVALID_OPERATION;
	}
	
	// Clear TWINT flag to start a new event operation | Enable TWI
	TWCR |= (1<<TWINT) | (1<<TWEN);
	
	// Make sure that TWSTA and TWSTO are zeros
	TWCR &= ~((1<<TWSTA) | (1<<TWSTO));
	
	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}

	// Wait until TWI finish its current job/event
	while(!(TWCR & (1<<TWINT)));		// Busy wait
	
	// Read the TWI status
	TWI_status = (TWSR & TWI_TWSR_STATUS_BITS_MASK);
	
	if(TWI_status == TWI_SLAVE_ADDRESS_W_SENT_ACK_RECEIVED_STATE)
	{
		return TWI_SLAVE_ADDRESS_W_SENT_ACK_RECEIVED;
	}
	else if(TWI_status == TWI_SLAVE_ADDRESS_W_SENT_NACK_RECEIVED_STATE)
	{
		return TWI_SLAVE_ADDRESS_W_SENT_NACK_RECEIVED;
	}
	else if(TWI_status == TWI_SLAVE_ADDRESS_R_SENT_ACK_RECEIVED_STATE)
	{
		return TWI_SLAVE_ADDRESS_R_SENT_ACK_RECEIVED;
	}
	else if(TWI_status == TWI_SLAVE_ADDRESS_R_SENT_NACK_RECEIVED_STATE)
	{
		return TWI_SLAVE_ADDRESS_R_SENT_NACK_RECEIVED;
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
	
}

EN_TWI_EVENT_STATUS_t TWI_master_transmit(uint8_t data, bool interruptHandled)
{
	uint8_t TWI_status = 0;

	// Set the data into TWI data register - data **MUST** be written to TWDR before clearing TWINT
	TWDR = data;
	
	// Clear TWINT flag to start a new event operation | Enable TWI
	TWCR |= (1<<TWINT) | (1<<TWEN);
	
	// Make sure that TWSTA and TWSTO are zeros
	TWCR &= ~((1<<TWSTA) | (1<<TWSTO));
	
	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}

	// Wait until TWI finish its current job/event
	while(!(TWCR&(1<<TWINT)));		// Busy wait
	
	// Read the TWI status
	TWI_status = (TWSR & TWI_TWSR_STATUS_BITS_MASK);
	
	// Check data transmission status
	if(TWI_status == TWI_MASTER_DATA_SENT_ACK_RECEIVED_STATE)
	{
		return TWI_MASTER_DATA_SENT_ACK_RECEIVED;
	}
	else if(TWI_status == TWI_MASTER_DATA_SENT_NACK_RECEIVED_STATE)
	{
		return TWI_MASTER_DATA_SENT_NACK_RECEIVED;
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
}

EN_TWI_EVENT_STATUS_t TWI_master_receive(uint8_t* receivedData, uint8_t response, bool interruptHandled)
{
	uint8_t TWI_status = 0;

	if(response == TWI_ACK)
	{
		// Clear TWINT flag to start a new event operation | Enable TWI | Enable Generation of ACK when data is received
		TWCR |= (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	}
	else if(response == TWI_NACK)
	{
		// Clear TWINT flag to start a new event operation | Enable TWI
		TWCR |= (1<<TWINT) | (1<<TWEN);
		
		// Enable Generation of NACK when data is received
		TWCR &= ~(1<<TWEA);
	}
	else
	{
		// Invalid ACK status
		return TWI_INVALID_OPERATION;
	}
	
	// Make sure that TWSTA and TWSTO are zeros
	TWCR &= ~((1<<TWSTA) | (1<<TWSTO));

	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}
	
	// Wait until TWI finish its current job/event
	while(!(TWCR & (1<<TWINT)));		// Busy wait
	
	// Read the data from TWI data register
	*receivedData = TWDR;
	
	// Read the TWI status
	TWI_status = (TWSR & TWI_TWSR_STATUS_BITS_MASK);
	
	// Check data reception and ACK/NACK sent status
	if(TWI_status == TWI_MASTER_DATA_RECEIVED_ACK_SENT_STATE)
	{
		return TWI_MASTER_DATA_RECEIVED_ACK_SENT;			// Data is received and ACK is sent
	}
	else if(TWI_status == TWI_MASTER_DATA_RECEIVED_NACK_SENT_STATE)
	{
		return TWI_MASTER_DATA_RECEIVED_NACK_SENT;			// Data is received
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
}

EN_TWI_EVENT_STATUS_t TWI_master_stop(bool interruptHandled)
{
	// Clear TWINT flag to start a new event operation | Generate stop condition event | Enable TWI
	TWCR |= (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	
	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}
	
	// Wait until stop condition execution (TWSTO is cleared after execution)
	while(TWCR & (1<<TWSTO));		// Busy wait
	
	// A return to make sure that the function always returns a value in all cases 
	return TWI_STOP_SENT;
}

EN_TWI_EVENT_STATUS_t TWI_slave_listen(bool interruptHandled)
{
	uint8_t TWI_status = 0;

	// Clear TWINT flag to start a new event operation | Enable ACK generation for this device slave address of to the general call address (if enabled) | Enable TWI
	TWCR |= (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	
	// Make sure that TWSTA and TWSTO are zeros
	TWCR &= ~((1<<TWSTA) | (1<<TWSTO));

	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}

	// Wait until the device is being addressed
	while(!(TWCR & (1<<TWINT)));		// Busy wait
	
	// Read the TWI status
	TWI_status = (TWSR & TWI_TWSR_STATUS_BITS_MASK);
	
	// Slave address received + write operation, and ACK returned
	if(TWI_status == TWI_SLAVE_ADDRESS_W_RECEIVED_STATE)
	{
		return TWI_SLAVE_ADDRESS_W_RECEIVED;
	}
	// Slave address received + read operation, and ACK returned
	else if(TWI_status == TWI_SLAVE_ADDRESS_R_RECEIVED_STATE)
	{
		return TWI_SLAVE_ADDRESS_R_RECEIVED;
	}
	// General call address received and ACK returned
	else if(TWI_status == TWI_GENERAL_ADDRESS_RECEIVED_STATE)
	{
		return TWI_GENERAL_ADDRESS_RECEIVED;
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
	
}

EN_TWI_EVENT_STATUS_t TWI_slave_receive(uint8_t* receivedData, uint8_t response, bool interruptHandled)
{
	uint8_t TWI_status = 0;
	
	if(response == TWI_ACK)
	{
		// Clear TWINT flag to start a new event operation | Enable ACK generation for this device slave address of to the general call address (if enabled) | Enable TWI
		TWCR |= (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	}
	else if(response == TWI_NACK)
	{
		// Clear TWINT flag to start a new event operation | Enable TWI
		TWCR |= (1<<TWINT) | (1<<TWEN);
		// Enable NACK generation for this device slave address of to the general call address (if enabled)
		TWCR &= ~(1<<TWEA);
	}
	else
	{
		// Invalid ACK status
		return TWI_INVALID_OPERATION;
	}
	
	// Make sure that TWSTA and TWSTO are zeros
	TWCR &= ~((1<<TWSTA) | (1<<TWSTO));
	
	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}
	
	// Wait until data is received
	while(!(TWCR & (1<<TWINT)));		// Busy wait
	
	// Read the TWI status
	TWI_status = (TWSR & TWI_TWSR_STATUS_BITS_MASK);
	
	// Read the received data from TWI data register
	*receivedData = TWDR;
	
	// Data received, and ACK returned
	if(TWI_status == TWI_SLAVE_DATA_RECEIVED_ACK_SENT_STATE)
	{
		return TWI_SLAVE_DATA_RECEIVED_ACK_SENT;
	}
	// Data received, and NACK returned
	else if(TWI_status == TWI_SLAVE_DATA_RECEIVED_NACK_SENT_STATE)
	{	
		return TWI_SLAVE_DATA_RECEIVED_NACK_SENT;
	}
	// STOP or REPEATED START received
	else if(TWI_status == TWI_SLAVE_STO_RSTA_RECEIVED_STATE)
	{
		return TWI_SLAVE_STO_RSTA_RECEIVED;
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
}

EN_TWI_EVENT_STATUS_t TWI_slave_transmit(uint8_t data, bool interruptHandled)
{
	uint8_t TWI_status = 0;

	// Set the data into TWI data register - data **MUST** be written to TWDR before clearing TWINT
	TWDR = data;

	// Clear TWINT flag to start a new event operation | Enable ACK generation for this device slave address of to the general call address (if enabled) | Enable TWI
	TWCR |= (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	
	// Make sure that TWSTA and TWSTO are zeros
	TWCR &= ~((1<<TWSTA) | (1<<TWSTO));

	// Return from function and to not wait for the operation/event to be finished (to not wait for the TWIINT flag to be set). Reading the operation/event TWI status should be handled in TWI ISR
	if(interruptHandled)
	{
		return TWI_INTERRUPT_HANDLED;
	}

	// Wait until data is transmitted
	while(!(TWCR & (1<<TWINT)));		// Busy wait
	
	// Read the TWI status
	TWI_status = (TWSR & TWI_TWSR_STATUS_BITS_MASK);
	
	// Data sent, and ACK returned
	if(TWI_status == TWI_SLAVE_DATA_SENT_ACK_RECEIVED_STATE)
	{
		return TWI_SLAVE_DATA_SENT_ACK_RECEIVED;
	}
	// Data sent, and NACK returned
	else if(TWI_status == TWI_SLAVE_DATA_SENT_NACK_RECEIVED_STATE)
	{	
		return TWI_SLAVE_DATA_SENT_NACK_RECEIVED;
	}
	// STOP or REPEATED START received
	else if(TWI_status == TWI_SLAVE_STO_RSTA_RECEIVED_STATE)
	{
		return TWI_SLAVE_STO_RSTA_RECEIVED;
	}
	else
	{
		return TWI_UNHANDLED_EVENT;
	}
}

uint8_t TWI_getDataRegister()
{
	return TWDR;
}

uint8_t TWI_getStatus()
{
	// Read the TWI status
	uint8_t TWI_status = (TWSR & TWI_TWSR_STATUS_BITS_MASK);
	
	return TWI_status;	
}

void TWI_setInterruptCallback(void(*callbackFunction)())
{
	// Enable global interrupts
	sei();
	
	// Enable TWI interrupt
	TWCR |= (1 << TWIE);
	 
	// Set the callback function
	TWI_INTERRUPT_CALLBACK_FUNCTION = callbackFunction;
}

ISR(TWI_VECTOR)
{
	// Make sure that the callback function is not NULL
	if(TWI_INTERRUPT_CALLBACK_FUNCTION != 0)
	{
		TWI_INTERRUPT_CALLBACK_FUNCTION();
	}
}