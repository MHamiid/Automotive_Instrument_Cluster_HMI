/*
 * UART.h
 *
 * Created: 9/12/2022 8:17:34 PM
 *  Author: MHamiid
 */ 


#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
/* prevent compiler error by supplying a default F_CPU */
# warning "F_CPU not defined for <ATMega32A/MCAL/UART/UART.h>. Using 1 MHz as a default F_CPU"

/**
  * The value 1 MHz only provided as a fall back 
  * if no user-provided definition could be found
*/
# define F_CPU 1000000UL
#endif

#include <stdint.h>

/**
 * @brief Initialize UART(Asynchronous Mode) in both transmission and reception mode in normal speed mode with frame format of [ 8 data bits | no parity | 1 stop bit ]
 * 
 * @param baudRate					Baud rate of UART communication
 *
 * @return void
 */
void UART_init(uint32_t baudRate);

/**
 * @brief Read/Receive one byte of data
 *
 * Function will not exit until a byte of data is received **Uses Busy Wait**
 * 
 * @return							The received byte of data
 */
uint8_t UART_receive();

/**
 * @brief Write/Transmit one byte of data
 * 
 * Function will not exit until the transmit buffer is empty, so it can write the byte of data to be transmitted **Uses Busy Wait**
 *
 * @param data						A byte of data to be transmitted
 *
 * @return void
 */
void UART_transmit(uint8_t data);

/**
 * @brief Write/Transmit a string, where the string **MUST** be null terminated. After string transmission a null terminator
 * character is transmitted, so the receiver can determine the end of the string
 * 
 * Function will not exit until the transmit buffer is empty, so it can write the byte of data to be transmitted **Uses Busy Wait**.
 * Function calls UART_transmit() internally
 *
 * @param string					A null terminated string to be transmitted
 *
 * @return void
 */
void UART_transmitString(const uint8_t* string);

/**
 * @brief Hook a callback function that gets called when a byte reception is complete
 * 
 * @param onReceiveCallbackFunction		Callback function that gets called every time a byte is received, where the received byte is passed as a parameter to the function
 *
 * @return void
 */
void UART_onReceive(void(*onReceiveCallbackFunction)(uint8_t));


#endif /* UART_H_ */