/*
 * Application.c
 *
 * Created: 9/25/2022 6:26:59 PM
 *  Author: MHamiid
 */ 


#include "Application.h"
#include <ATMega32A/MCAL/TWI/TWI.h>
#include <ATMega32A/ECUAL/Motor/Motor.h>
#include <ATMega32A/ECUAL/Accelerometer/Accelerometer.h>
#include <ATMega32A/ECUAL/LM35/LM35.h>

#define DEVICE_INTERNAL_ADDRESS_MOTOR 0x01
#define DEVICE_INTERNAL_ADDRESS_ACCELEROMETER 0x02
#define DEVICE_INTERNAL_ADDRESS_LM35 0x03

static float gs_accelerometerValue = 0.0f;
static float gs_temperatureValue = 0.0f;
static uint8_t gs_currentlyAddressedDevice = 0x00;
static uint8_t* bytePtrToFloat = 0x00;	// Points to the current byte to be transmitted of the four bytes of a float type

/**
 * TWI slave after being addressed by master reads device's internal address from master and send the current status of that device to master
 * TWI slave handles the following frames:
 *
 * START CONDITION -> Own slave address + Write -> DEVICE_INTERNAL_ADDRESS_MOTOR -> ACK -> REPEATED START CONDITION/STOP + START CONDITION
 * -> Own slave address + Read -> ACK -> Motor PWM duty cycle -> NACK -> STOP CONDITION
 *
 * START CONDITION -> Own slave address + Write -> DEVICE_INTERNAL_ADDRESS_ACCELEROMETER -> ACK -> REPEATED START CONDITION/STOP + START CONDITION
 * -> Own slave address + Read -> ACK -> Accelerometer first byte -> ACK -> Accelerometer second byte -> ACK -> Accelerometer thrid byte -> ACK -> Accelerometer fourth/last byte -> NACK -> STOP CONDITION
 *
 * START CONDITION -> Own slave address + Write -> DEVICE_INTERNAL_ADDRESS_LM35 -> ACK -> REPEATED START CONDITION/STOP + START CONDITION
 * -> Own slave address + Read -> ACK -> temperatue first byte -> ACK -> temperatue second byte -> ACK -> temperatue thrid byte -> ACK -> temperatue fourth/last byte -> NACK -> STOP CONDITION
 *
*/
/* Handle TWI interrupts */
static void TWIInterruptCallback()
{
	// Get the current TWI status
	EN_TWI_EVENT_STATUS_t TWI_status = TWI_getStatus();
	
	/* Handle TWI slave logic based on the current TWI_status */
	
	// Slave receiver mode
	if(TWI_status == TWI_SLAVE_ADDRESS_W_RECEIVED_STATE)
	{
		// Receive byte of data from master and send ACK on reception. And handle TWI status in the interrupt callback function
		TWI_slave_receive(0, TWI_ACK, true);
	}
	// Slave transmitter mode
	else if(TWI_status == TWI_SLAVE_ADDRESS_R_RECEIVED_STATE)
	{
		if(gs_currentlyAddressedDevice == DEVICE_INTERNAL_ADDRESS_MOTOR)
		{
			// Transmit the current Motor duty cycle. And handle TWI status in the interrupt callback function
			TWI_slave_transmit(motor_getDutyCycle(), true);
		}
		else if(gs_currentlyAddressedDevice == DEVICE_INTERNAL_ADDRESS_ACCELEROMETER)
		{
			// Point to the first byte of the float
			bytePtrToFloat = (uint8_t*)&gs_accelerometerValue;
			
			// Transmit the first byte (of the 4 bytes of the float type) of the Accelerometer value. And handle TWI status in the interrupt callback function
			TWI_slave_transmit(*bytePtrToFloat, true);
			
			// Point to the next byte of the float to be transmitted
			bytePtrToFloat++;
		}
		else if(gs_currentlyAddressedDevice == DEVICE_INTERNAL_ADDRESS_LM35)
		{
			// Point to the first byte of the float
			bytePtrToFloat = (uint8_t*)&gs_temperatureValue;
			
			// Transmit the first byte (of the 4 bytes of the float type) of the temperature value. And handle TWI status in the interrupt callback function
			TWI_slave_transmit(*bytePtrToFloat, true);
			
			// Point to the next byte of the float to be transmitted
			bytePtrToFloat++;
		}
	}
	// For sending multiple bytes of data, we can handle in here any byte to be sent after the first byte
	else if(TWI_status == TWI_SLAVE_DATA_SENT_ACK_RECEIVED_STATE)
	{
		if(gs_currentlyAddressedDevice == DEVICE_INTERNAL_ADDRESS_ACCELEROMETER)
		{
			// Transmit the next byte (of the 4 bytes of the float type) of the Accelerometer value. And handle TWI status in the interrupt callback function
			TWI_slave_transmit(*bytePtrToFloat, true);
			
			// Point to the next byte of the float to be transmitted
			bytePtrToFloat++;
		}
		else if(gs_currentlyAddressedDevice == DEVICE_INTERNAL_ADDRESS_LM35)
		{
			// Transmit the next byte (of the 4 bytes of the float type) of the temperature value. And handle TWI status in the interrupt callback function
			TWI_slave_transmit(*bytePtrToFloat, true);
			
			// Point to the next byte of the float to be transmitted
			bytePtrToFloat++;
		}
	}
	// Received data from master
	else if(TWI_status == TWI_SLAVE_DATA_RECEIVED_ACK_SENT_STATE)
	{
		// Get the received internal device address from master
		gs_currentlyAddressedDevice = TWI_getDataRegister();
	}
	// Received STOP or REPEATED START condition
	else if(TWI_status == TWI_SLAVE_STO_RSTA_RECEIVED)
	{
		// Listen for own slave address on the TWI bus. And handle TWI status in the interrupt callback function
		TWI_slave_listen(true);
	}
	
}

void application_init()
{
	motor_init(ADC_CHANNEL_0, PWM_TIMER2);
	accelerometer_init(ADC_CHANNEL_1);
	LM35_init(ADC_CHANNEL_2);
	
	// Initialize TWI in slave mode with own slave address 0xA0
	TWI_slave_init(0xA0);
	// Set TWI interrupt callback function
	TWI_setInterruptCallback(TWIInterruptCallback);
	// Start listening for own slave address on the TWI bus. And handle TWI status in the interrupt callback function
	TWI_slave_listen(true);
}

void application_loop()
{
	motor_start(ADC_CHANNEL_0, PWM_TIMER2);
	gs_accelerometerValue = accelerometer_read(ADC_CHANNEL_1);
	gs_temperatureValue = LM35_read(ADC_CHANNEL_2);
}
