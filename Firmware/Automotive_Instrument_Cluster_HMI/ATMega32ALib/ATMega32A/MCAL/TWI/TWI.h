/*
 * TWI.h
 *
 * Created: 9/14/2022 9:47:23 PM
 *  Author: MHamiid
 */ 


#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>
#include <stdbool.h>

// Mask for TWSR status 5-bits, the higher 5-bits
#define TWI_TWSR_STATUS_BITS_MASK 0xF8

#define TWI_WRITE_BIT				0
#define TWI_READ_BIT				1
#define TWI_ACK						0
#define TWI_NACK					1


/* TWSR Register States */
/* Master States */
#define TWI_START_SENT_STATE							0x08
#define TWI_REPEATED_START_SENT_STATE					0x10
#define TWI_SLAVE_ADDRESS_W_SENT_ACK_RECEIVED_STATE		0x18
#define TWI_SLAVE_ADDRESS_W_SENT_NACK_RECEIVED_STATE	0x20
#define TWI_SLAVE_ADDRESS_R_SENT_ACK_RECEIVED_STATE		0x40
#define TWI_SLAVE_ADDRESS_R_SENT_NACK_RECEIVED_STATE	0x48
#define TWI_MASTER_DATA_SENT_ACK_RECEIVED_STATE			0x28
#define TWI_MASTER_DATA_SENT_NACK_RECEIVED_STATE		0x30
#define TWI_MASTER_DATA_RECEIVED_ACK_SENT_STATE			0x50
#define TWI_MASTER_DATA_RECEIVED_NACK_SENT_STATE		0x58
#define TWI_MASTER_ARBITRATION_LOST_STATE				0x38
/* Slave States */
#define TWI_SLAVE_ADDRESS_W_RECEIVED_STATE				0x60
#define TWI_SLAVE_ADDRESS_R_RECEIVED_STATE				0xA8
#define TWI_GENERAL_ADDRESS_RECEIVED_STATE				0x70
#define TWI_SLAVE_DATA_RECEIVED_ACK_SENT_STATE			0x80
#define TWI_SLAVE_DATA_RECEIVED_NACK_SENT_STATE			0x88
#define TWI_SLAVE_DATA_SENT_ACK_RECEIVED_STATE			0xB8
#define TWI_SLAVE_DATA_SENT_NACK_RECEIVED_STATE			0xC0
#define TWI_SLAVE_STO_RSTA_RECEIVED_STATE				0xA0


typedef enum EN_TWI_EVENT_STATUS_t
{
	TWI_INVALID_OPERATION,
	TWI_UNHANDLED_EVENT,
	TWI_INTERRUPT_HANDLED,
	/* Master Events */
	TWI_START_SENT,
	TWI_REPEATED_START_SENT,
	TWI_STOP_SENT,		// Has no status code
	TWI_SLAVE_ADDRESS_W_SENT_ACK_RECEIVED,
	TWI_SLAVE_ADDRESS_W_SENT_NACK_RECEIVED,
	TWI_SLAVE_ADDRESS_R_SENT_ACK_RECEIVED,
	TWI_SLAVE_ADDRESS_R_SENT_NACK_RECEIVED,
	TWI_MASTER_DATA_SENT_ACK_RECEIVED,
	TWI_MASTER_DATA_SENT_NACK_RECEIVED,
	TWI_MASTER_DATA_RECEIVED_ACK_SENT,
	TWI_MASTER_DATA_RECEIVED_NACK_SENT,
	TWI_MASTER_ARBITRATION_LOST,
	/* Slave Events */
	TWI_SLAVE_ADDRESS_W_RECEIVED,
	TWI_SLAVE_ADDRESS_R_RECEIVED,
	TWI_GENERAL_ADDRESS_RECEIVED,
	TWI_SLAVE_DATA_RECEIVED_ACK_SENT,
	TWI_SLAVE_DATA_RECEIVED_NACK_SENT,
	TWI_SLAVE_DATA_SENT_ACK_RECEIVED,
	TWI_SLAVE_DATA_SENT_NACK_RECEIVED,
	TWI_SLAVE_STO_RSTA_RECEIVED
	
} EN_TWI_EVENT_STATUS_t;


/**
 * @brief Initialize TWI in master mode
 * 
 * @param SCLFrequency											SCL line clock frequency (Max value: 400000 "400 KHz")
 *
 * @return void
 */
void TWI_master_init(uint32_t SCLFrequency);

/**
 * @brief Transmit a START condition as soon as the bus becomes free
 *
 * Function is called after TWI has been initialized in master mode
 *
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 * 
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return TWI_START_SENT										START condition has been transmitted
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_master_start(bool interruptHandled);

/**
 * @brief Transmit a REPEATED START condition
 *
 * Function is called after TWI has been initialized in master mode
 * 
 * REPEATED START enables the master to switch between slaves,
 * master transmitter mode and master receiver mode without losing control of the bus
 *
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return TWI_REPEATED_START_SENT								REPEATED START condition has been transmitted
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_master_repeatedStart(bool interruptHandled);

/**
 * @brief Transmit a STOP condition
 * 
 * STOP releases the bus and ends the communication 
 *
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return void
 */
EN_TWI_EVENT_STATUS_t TWI_master_stop(bool interruptHandled);

/**
 * @brief Transmit the 7-bit address of the slave to start communication with, and the read/write bit
 * 
 * Function is called after a START/REPEATED START condition has been sent by the master.
 * Master transmitter mode can be entered if write bit is selected, otherwise the master receiver mode can be entered if read bit is selected
 *
 * @param slaveAddress											Slave's 7-bit address that the master wants to start communication with
 * @param rw													TWI_WRITE_BIT for master transmit operation. TWI_READ_BIT for master receive operation
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return TWI_SLAVE_ADDRESS_W_SENT_ACK_RECEIVED				Slave address + Write sent and an ACK received
 * @return TWI_SLAVE_ADDRESS_W_SENT_NACK_RECEIVED				Slave address + Write sent and a NACK received
 * @return TWI_SLAVE_ADDRESS_R_SENT_ACK_RECEIVED				Slave address + Read sent and an ACK received
 * @return TWI_SLAVE_ADDRESS_R_SENT_NACK_RECEIVED				Slave address + Read sent and a NACK received
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_master_transmitSlaveAddress(uint8_t slaveAddress, uint8_t rw, bool interruptHandled);

/**
 * @brief Transmit a byte of data to the addressed slave after the master has entered master transmitter mode
 * 
 * Function is called after the slave address + write has been sent by the master (Master transmitter mode entered).
 *
 * @param data													A byte of data to be transmitted to addressed slave
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return TWI_MASTER_DATA_SENT_ACK_RECEIVED					Data sent and an ACK received
 * @return TWI_MASTER_DATA_SENT_NACK_RECEIVED					Data sent and a NACK received
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_master_transmit(uint8_t data, bool interruptHandled);

/**
 * @brief Receive a byte of data from the addressed slave after the master has entered master receiver mode
 * 
 * Function is called after the slave address + read has been sent by the master (Master receiver mode entered).
 * Master should inform the transmitting slave by sending a TWI_NACK as a response after the last received data byte,
 * the transfer is then ended by generating a STOP condition or a REPEATED START condition
 *
 * @param receivedData											A byte of data received from addressed slave (Only data is written when @param interruptHandled is set to false. And the parameter can be passed as NULL, as it has no usage is this case)
 * @param response												Master response to the received data. Can be [TWI_ACK, TWI_NACK]
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return TWI_INVALID_OPERATION								Invalid @param response used
 * @return TWI_MASTER_DATA_RECEIVED_ACK_SENT					Data received and an ACK sent
 * @return TWI_MASTER_DATA_RECEIVED_NACK_SENT					Data received and an NACK sent
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_master_receive(uint8_t* receivedData, uint8_t response, bool interruptHandled);

/**
 * @brief Initialize TWI in slave mode
 * 
 * @param slaveAddress											Device's own 7-bit slave address, with will be used buy other masters to address this device
 *
 * @return void
 */
void TWI_slave_init(uint8_t slaveAddress);

/**
 * @brief Slave listens to the bus for its own slave address
 * 
 * Function is called after TWI has been initialized in slave mode
 * Slave receiver mode is entered if own slave address + write is received, otherwise slave transmitter mode is entered if own slave address + read is received
 *
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true 
 * @return TWI_SLAVE_ADDRESS_W_RECEIVED							Own slave address + Write received and an ACK sent
 * @return TWI_SLAVE_ADDRESS_R_RECEIVED							Own slave address + Read received and an ACK sent
 * @return TWI_GENERAL_ADDRESS_RECEIVED							General call address received and an ACK sent
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_slave_listen(bool interruptHandled);

/**
 * @brief Receive a byte of data from the master after the slave has entered slave receiver mode
 * 
 * Function is called after own slave address + write has been received from the master (Slave receiver mode entered).
 * Slave receiver can send TWI_NACK to be used as a response to indicate that the slave is not able to receive any more bytes from the master,
 * the transfer is then ended by receiving a STOP condition or a REPEATED START condition
 *
 * @param receivedData											A byte of data received from master (Only data is written when @param interruptHandled is set to false. And the parameter can be passed as NULL, as it has no usage is this case)
 * @param response												Slave response to the received data. Can be [TWI_ACK, TWI_NACK]
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return TWI_INVALID_OPERATION								Invalid @param response used
 * @return TWI_SLAVE_DATA_RECEIVED_ACK_SENT						Data received and an ACK sent
 * @return TWI_SLAVE_DATA_RECEIVED_NACK_SENT					Data received and an NACK sent
 * @return TWI_SLAVE_STO_RSTA_RECEIVED							STOP or REPEATED START condition received
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_slave_receive(uint8_t* receivedData, uint8_t response, bool interruptHandled);

/**
 * @brief Transmit a byte of data to the master after the slave has entered slave transmitter mode
 * 
 * Function is called after own slave address + read has been received from the master (Slave transmitter mode entered).
 * Slave transmitter would receive a NACK from the master to indicate that the master has received the final byte from the slave,
 * the transfer is then ended by receiving a STOP condition or a REPEATED START condition
 *
 * @param data													A byte of data to be transmitted to the master
 * @param interruptHandled										Set to true to stop waiting for the operation/event to finish (stop waiting for TWI interrupt flag to be set), indicating that the interrupt is handled in the TWI ISR **No Busy Wait**
 *
 * @return TWI_INTERRUPT_HANDLED								Always returned when @param interruptHandled is set to true
 * @return TWI_SLAVE_DATA_SENT_ACK_RECEIVED						Data sent and an ACK received
 * @return TWI_SLAVE_DATA_SENT_NACK_RECEIVED					Data sent and an NACK received
 * @return TWI_SLAVE_STO_RSTA_RECEIVED							STOP or REPEATED START condition received
 * @return TWI_UNHANDLED_EVENT									Received unexpected status code
 */
EN_TWI_EVENT_STATUS_t TWI_slave_transmit(uint8_t data, bool interruptHandled);

/**
 * @brief Return the TWI data register
 *
 * @return			TWI data register value
 */
uint8_t TWI_getDataRegister();

/**
 * @brief Return the current TWI status
 *
 * @return			Current TWI status
 */
uint8_t TWI_getStatus();

/**
 * @brief Set a callback function to be called inside the ISR for TWI interrupt
 *
 * Function enables the TWI interrupt and the global interrupts
 * 
 * @param callbackFunction				void function will be called when the ISR for the TWI interrupt is called
 *
 * @return void
 */
void TWI_setInterruptCallback(void(*callbackFunction)());


#endif /* TWI_H_ */