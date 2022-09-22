/*
 * bit.h
 *
 * Created: 9/22/2022 8:37:10 PM
 *  Author: MHamiid
 */ 


#ifndef BIT_H_
#define BIT_H_

#define BIT_SET(byte, nbit)			(  (byte) |=  (1<<(nbit)) )
#define BIT_CLEAR(byte, nbit)		(  (byte) &= ~(1<<(nbit)) )
#define BIT_TOGGLE(byte, nbit)		(  (byte) ^=  (1<<(nbit)) )
#define BIT_READ(byte, nbit)		( ((byte) &   (1<<(nbit))) >> nbit )  // Sets the read bit to be either 0 or 1



#endif /* BIT_H_ */