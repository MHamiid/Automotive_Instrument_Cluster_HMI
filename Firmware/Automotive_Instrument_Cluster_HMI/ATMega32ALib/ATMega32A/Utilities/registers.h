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

/* PORTA Registers */
#define PORTA	(*((volatile uint8_t*)0x3B))
#define DDRA	(*((volatile uint8_t*)0x3A))
#define PINA	(*((volatile uint8_t*)0x39))

/* PORTB Registers */
#define PORTB	(*((volatile uint8_t*)0x38))
#define DDRB	(*((volatile uint8_t*)0x37))
#define PINB	(*((volatile uint8_t*)0x36))

/* PORTC Registers */
#define PORTC	(*((volatile uint8_t*)0x35))
#define DDRC	(*((volatile uint8_t*)0x34))
#define PINC	(*((volatile uint8_t*)0x33))

/* PORTD Registers */
#define PORTD	(*((volatile uint8_t*)0x32))
#define DDRD	(*((volatile uint8_t*)0x31))
#define PIND	(*((volatile uint8_t*)0x30))

/************************************************************************/
/* Timer Registers                                                      */
/************************************************************************/

#define TIFR	(*((volatile uint8_t*)0x58))
/* Timer 0 */
#define TCNT0	(*((volatile uint8_t*)0x52))
#define OCR0	(*((volatile uint8_t*)0x5C))
#define TCCR0	(*((volatile uint8_t*)0x53))
/* TCCR0 Bits */
#define CS00    0
#define CS01    1
#define CS02    2
#define WGM01   3
#define COM00   4
#define COM01   5
#define WGM00   6
#define FOC0    7

/* Timer 2 */
#define TCNT2	(*((volatile uint8_t*)0x44))
#define OCR2	(*((volatile uint8_t*)0x43))
#define TCCR2	(*((volatile uint8_t*)0x45))
/* TCCR2 Bits */
#define CS20    0
#define CS21    1
#define CS22    2
#define WGM21   3
#define COM20   4
#define COM21   5
#define WGM20   6
#define FOC2    7

/************************************************************************/
/* External Interrupt Registers                                         */
/************************************************************************/

#define GIFR	(*((volatile uint8_t*)0x5A))
#define MCUCR	(*((volatile uint8_t*)0x55))
/* MCUCR Bits */
#define ISC00   0
#define ISC01   1
#define ISC10   2
#define ISC11   3
#define SM0     4
#define SM1     5
#define SM2     6
#define SE      7
#define MCUCSR	(*((volatile uint8_t*)0x54))
/* MCUCSR Bits */
#define ISC2    6
#define PORF    0
#define EXTRF   1
#define BORF    2
#define WDRF    3
#define JTRF    4
#define JTD     7
#define GICR	(*((volatile uint8_t*)0x5B))
/* GICR Bits */
#define IVCE    0
#define IVSEL   1
#define INT2    5
#define INT0    6
#define INT1    7

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