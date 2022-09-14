/*
 * registers.h
 *
 * Created: 7/29/2022 6:03:51 AM
 *  Author: MHamiid
 */ 

/************************************************************************/
/* ATmega32A Register File                                              */
/************************************************************************/

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>

/************************************************************************/
/* DIO Registers                                                        */
/************************************************************************/

// PORTA registers
#define PORTA	(*((volatile uint8_t*)0x3B))
#define DDRA	(*((volatile uint8_t*)0x3A))
#define PINA	(*((volatile uint8_t*)0x39))

// PORTB registers
#define PORTB	(*((volatile uint8_t*)0x38))
#define DDRB	(*((volatile uint8_t*)0x37))
#define PINB	(*((volatile uint8_t*)0x36))

// PORTC registers
#define PORTC	(*((volatile uint8_t*)0x35))
#define DDRC	(*((volatile uint8_t*)0x34))
#define PINC	(*((volatile uint8_t*)0x33))

// PORTD registers
#define PORTD	(*((volatile uint8_t*)0x32))
#define DDRD	(*((volatile uint8_t*)0x31))
#define PIND	(*((volatile uint8_t*)0x30))

/************************************************************************/
/* Timer Registers                                                      */
/************************************************************************/

// Timer 0
#define TCCR0	(*((volatile uint8_t*)0x53))
#define TCNT0	(*((volatile uint8_t*)0x52))
#define TIFR	(*((volatile uint8_t*)0x58))

/************************************************************************/
/* External Interrupt Registers                                         */
/************************************************************************/

#define MCUCR	(*((volatile uint8_t*)0x55))
#define MCUCSR	(*((volatile uint8_t*)0x54))
#define GICR	(*((volatile uint8_t*)0x5B))
#define GIFR	(*((volatile uint8_t*)0x5A))

/************************************************************************/
/* UART Registers                                                       */
/************************************************************************/

#define UBRRH	(*((volatile uint8_t*)0x40))
#define UBRRL	(*((volatile uint8_t*)0x29))
#define UDR		(*((volatile uint8_t*)0x2C))
#define UCSRA	(*((volatile uint8_t*)0x2B))
/* UCSRA Bits */
#define MPCM	0
#define U2X		1
#define PE		2
#define DOR		3
#define FE		4
#define UDRE	5
#define TXC		6
#define RXC		7
#define UCSRB	(*((volatile uint8_t*)0x2A))
/* UCSRB Bits */
#define TXB8	0
#define RXB8	1
#define UCSZ2	2
#define TXEN	3
#define RXEN	4
#define UDRIE	5
#define TXCIE	6
#define RXCIE	7
#define UCSRC	(*((volatile uint8_t*)0x40))
/* UCSRC Bits */
#define UCPOL	0
#define UCSZ0	1
#define UCSZ1	2
#define USBS	3
#define UPM0	4
#define UPM1	5
#define UMSEL	6
#define URSEL	7

/************************************************************************/
/* TWI (I2C) Registers                                                  */
/************************************************************************/

#define TWBR	(*((volatile uint8_t*)0x20))
#define TWAR	(*((volatile uint8_t*)0x22))
#define TWDR	(*((volatile uint8_t*)0x23))
#define TWSR	(*((volatile uint8_t*)0x21))
/* TWSR Bits */
#define TWPS0   0
#define TWPS1   1
#define TWS3    3
#define TWS4    4
#define TWS5    5
#define TWS6    6
#define TWS7    7
#define TWCR	(*((volatile uint8_t*)0x56))
/* TWCR Bits */
#define TWIE    0
#define TWEN    2
#define TWWC    3
#define TWSTO   4
#define TWSTA   5
#define TWEA    6
#define TWINT   7


#endif /* REGISTERS_H_ */