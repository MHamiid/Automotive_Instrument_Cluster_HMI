/*
 * UART.c
 *
 * Created: 9/12/2022 8:30:29 PM
 *  Author: MHamiid
 */ 

#include "UART.h"
#include "../../Config/Config.h"
#include "../../Utilities/registers.h"
#include "../../Utilities/interrupt.h"
#include <math.h>
#include <stdio.h>  // For using size_t

#ifndef F_CPU
/* prevent compiler error by supplying a default F_CPU */
# warning "F_CPU not defined for <ATMega32A/MCAL/TWI/TWI.c>. Using 1 MHz as a default F_CPU. Maybe try to #include <Config/Config.h>."

/**
  * The value 1 MHz only provided as a fall back 
  * if no user-provided definition could be found
*/
# define F_CPU 1000000UL
#endif

/**
 * Calculate the value for the UBRR register from the baud rate, returns value type (double)
 * (F_CPU * 1.0)  -> for conversion to double
*/
#define UBRR_FROM_BAUD_RATE(BAUD_RATE) (((F_CPU * 1.0 / (BAUD_RATE* 16UL))) - 1)

/* UART Callback Functions */
static void(*ON_RECEIVE_CALLBACK_FUNCTION)(uint8_t) = NULL;	// Initialize the function pointer to NULL 


void UART_init(uint32_t baudRate)
{
	// Calculate UBRR value from baud rate
	uint16_t UBRR = round(UBRR_FROM_BAUD_RATE(baudRate)); // Round the value to the closest integer value
	// Write the higher byte of UBRR
	UBRRH = (uint8_t)(UBRR >> 8);
	// Write the lower byte of UBRR
	UBRRL = (uint8_t)UBRR;	// Will write only the first 8-bits of the 16-bit
	
	// Enable UART transmission and reception
	UCSRB |= (1<<TXEN) | (1<<RXEN);
	
	// Set frame Format -> 8 data bits, no parity, and 1 stop bit
	UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
}

uint8_t UART_receive()
{
	// Wait for data to be received
	while(!(UCSRA & (1<<RXC)));	// Busy wait
	
	// Return the received data
	return UDR;
}

void UART_transmit(uint8_t data)
{
	// Wait for the transmit buffer to be empty (UDR), so it can receive new data to be transmitted
	while(!(UCSRA & (1<<UDRE))); // Busy wait
	
	// Write the data into the buffer, which automatically starts sending the data
	UDR = data;
}

void UART_transmitString(const uint8_t* string)
{
	for(size_t charIndex = 0; string[charIndex] != '\0'; charIndex++)
	{
		UART_transmit(string[charIndex]);
	}
	
	// Send the null terminator, so the receiver can determine the end of the string
	UART_transmit('\0');
}

void UART_onReceive(void(*onReceiveCallbackFunction)(uint8_t))
{
	// Enable RX complete interrupt
	UCSRB |= (1 << RXCIE);
	
	// Enable global interrupt
	sei();
	
	// Set the callback function
	ON_RECEIVE_CALLBACK_FUNCTION = onReceiveCallbackFunction;
}

ISR(USART_RECEPTION_COMPLETE_VECTOR)
{
	// Safe check that the callback function is not NULL
	if(ON_RECEIVE_CALLBACK_FUNCTION != NULL)
	{
		// Call the callback function with the received byte of data
		ON_RECEIVE_CALLBACK_FUNCTION(UDR);
	}
	
	// Clear the interrupt flag
	UCSRA |= (1<<RXC);
}