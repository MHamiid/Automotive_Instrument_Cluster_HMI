/*
 * Application.c
 *
 * Created: 9/28/2022 11:56:51 PM
 *  Author: MHamiid
 */ 


#include "Application.h"
#include <ATMega32A/MCAL/TWI/TWI.h>
#include <stdint.h>

#define DEVICE_INTERNAL_ADDRESS_MOTOR			 0x01
#define DEVICE_INTERNAL_ADDRESS_ACCELEROMETER	 0x02
#define DEVICE_INTERNAL_ADDRESS_LM35			 0x03

typedef union UN_receivedData_t
{
	uint8_t byteData;
	uint8_t byteDataArray[4];
	float floatData;	// 4 Bytes
} UN_receivedData_t;

/**
 * @brief As a TWI master address the slave and receive the slave's internal device data/status
 *
 * TWI slave after being addressed by master reads device's internal address from master and send the current status of that device to master
 *
 * TWI master handles the following frames:
 *
 * START CONDITION -> slave address + Write -> DEVICE_INTERNAL_ADDRESS_MOTOR -> ACK -> REPEATED START CONDITION
 * -> slave address + Read -> ACK -> Motor PWM duty cycle -> NACK -> STOP CONDITION
 *
 * START CONDITION -> slave address + Write -> DEVICE_INTERNAL_ADDRESS_ACCELEROMETER -> ACK -> REPEATED START CONDITION
 * -> slave address + Read -> ACK -> Accelerometer first byte -> ACK -> Accelerometer second byte -> ACK -> Accelerometer third byte -> ACK -> Accelerometer fourth/last byte -> NACK -> STOP CONDITION
 *
 * START CONDITION -> slave address + Write -> DEVICE_INTERNAL_ADDRESS_LM35 -> ACK -> REPEATED START CONDITION
 * -> slave address + Read -> ACK -> temperatue first byte -> ACK -> temperatue second byte -> ACK -> temperatue third byte -> ACK -> temperatue fourth/last byte -> NACK -> STOP CONDITION
 *
 * @param slaveAddress							Slave's 7-bit address that the master wants to start communication with
 * @param slaveInternalAddress					The internal device address that is connected to the addressed slave
 *
 * @return Data received from slave. Can be a byte or 4 bytes (of a float type)
 */
static UN_receivedData_t TWIGetSlaveInternalDeviceData(uint8_t slaveAddress, uint8_t slaveInternalAddress)
{
	UN_receivedData_t receivedData;
	
	// Send START condition. And wait for the operation to complete (status is returned)
	if(TWI_master_start(false) == TWI_START_SENT)
	{
		// Send slave address + write. And wait for the operation to complete (status is returned)
		if(TWI_master_transmitSlaveAddress(slaveAddress, TWI_WRITE_BIT, false) == TWI_SLAVE_ADDRESS_W_SENT_ACK_RECEIVED)
		{
			// Send data (slave internal address). And wait for the operation to complete (status is returned)
			if(TWI_master_transmit(slaveInternalAddress, false) == TWI_MASTER_DATA_SENT_ACK_RECEIVED)
			{
				// Send REPEATED START condition. And wait for the operation to complete (status is returned)
				if(TWI_master_repeatedStart(false) == TWI_REPEATED_START_SENT)
				{
					// Send slave address + read. And wait for the operation to complete (status is returned)
					if(TWI_master_transmitSlaveAddress(slaveAddress, TWI_READ_BIT, false) == TWI_SLAVE_ADDRESS_R_SENT_ACK_RECEIVED)
					{
						/* Handle different received data as they vary in size depending of the address internal device */
						switch(slaveInternalAddress)
						{
							// Receive one byte
							case DEVICE_INTERNAL_ADDRESS_MOTOR:
								// Receive a byte of data from slave (internal device data/status), and send ACK/NACK response. And wait for the operation to complete (status is returned)
								if(TWI_master_receive(&receivedData.byteData, TWI_NACK, false) == TWI_MASTER_DATA_RECEIVED_NACK_SENT)
								{
									// Send STOP condition. And wait for the operation to complete (status is returned)
									TWI_master_stop(false);
								}
								
								break;
							
							// Receive 4 bytes (of a float type)
							case DEVICE_INTERNAL_ADDRESS_ACCELEROMETER:
							case DEVICE_INTERNAL_ADDRESS_LM35:
								{
									// Send ACK for each byte received except the last (fourth) byte send NACK
									uint8_t dataReceptionResponse = TWI_ACK;
									for(uint8_t i = 0; i < 4; i++)
									{
										// For the last byte (fourth byte) send NACK
										if(i == 3)
										{
											dataReceptionResponse = TWI_NACK;
										}
										
										// Receive a byte of data from slave (internal device data/status), and send ACK/NACK response. And wait for the operation to complete (status is returned)
										if(TWI_master_receive(&receivedData.byteDataArray[i], dataReceptionResponse, false) != TWI_MASTER_DATA_RECEIVED_ACK_SENT)
										{
											break;
										}
									}
									
									// Send STOP condition. And wait for the operation to complete (status is returned)
									TWI_master_stop(false);
								}
								
								break;
								
							default:
								// Send STOP condition. And wait for the operation to complete (status is returned)
								TWI_master_stop(false);
								
								break;
						}
						
					}
				}
			}
		}
	}
	
	return receivedData;
}

void application_init()
{
	// Initialize TWI in master mode with the SCL frequency
	TWI_master_init(1000);
}

void application_loop()
{
	// TWIGetSlaveInternalDeviceData(0xA0, DEVICE_INTERNAL_ADDRESS_MOTOR).byteData;
	// TWIGetSlaveInternalDeviceData(0xA0, DEVICE_INTERNAL_ADDRESS_ACCELEROMETER).floatData;	
	// TWIGetSlaveInternalDeviceData(0xA0, DEVICE_INTERNAL_ADDRESS_LM35).floatData;
}