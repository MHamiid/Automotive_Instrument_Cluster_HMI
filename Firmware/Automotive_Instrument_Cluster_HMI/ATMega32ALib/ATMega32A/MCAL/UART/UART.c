/*
 * UART.c
 *
 * Created: 9/12/2022 8:30:29 PM
 *  Author: MHamiid
 */ 

#include "UART.h"
#include "../../Utilities/registers.h"
#include <math.h>
#include <stdio.h>  // For using size_t

/**
 * Calculate the value for the UBRR register from the baud rate, returns value type (double)
 * (F_CPU * 1.0)  -> for conversion to double
*/
#define UBRR_FROM_BAUD_RATE(BAUD_RATE) (((F_CPU * 1.0 / (BAUD_RATE* 16UL))) - 1)


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