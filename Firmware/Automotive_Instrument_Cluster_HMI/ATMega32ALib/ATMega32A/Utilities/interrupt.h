/*
 * interrupt.h
 *
 * Created: 8/2/2022 2:55:07 PM
 *  Author: MHamiid
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

/************************************************************************/
/* Interrupt Vectors                                                    */
/************************************************************************/

/* External Interrupt Vectors */
// External interrupt request 0
#define EXT_INT_0_VECTOR __vector_1
// External interrupt request 1
#define EXT_INT_1_VECTOR __vector_2
// External interrupt request 2
#define EXT_INT_2_VECTOR __vector_3

/* USART Interrupt Vectors */
// USART reception (RX) complete
#define USART_RECEPTION_COMPLETE_VECTOR __vector_13



// Set global interrupts -> Sets the I-bit (7th bit) in the status register to 1
#define sei() __asm__ __volatile__ ("sei" ::: "memory")

// Clear global interrupts -> Sets the I-bit (7th bit) in the status register to 0
#define cli() __asm__ __volatile__ ("cli" ::: "memory")

/* ISR Function Macro */
#define ISR(INT_VECT) void INT_VECT(void) __attribute__ ((signal, used));  /*The function prototype (declaration)*/\
void INT_VECT(void) /*The function body (definition)*/

#endif /* INTERRUPT_H_ */