#include "serial.h"

#define DEVICE_DATA_FRAME_START_DELIMITER '|'
#define DEVICE_DATA_FRAME_END_DELIMITER '\r'

Serial::Serial(QObject* parent) : QSerialPort(parent)
{
    // Create signal slot connection to make readAndParseDeviceData() called whenever there is a new data ready to be read
    QObject::connect(this, &QIODevice::readyRead, this, &Serial::readAndParseDeviceData);
}

void Serial::readAndParseDeviceData()
{

    if(startOfDeviceDataFrameReceieved)
    {
        // Read a byte from the received device data frame into the receivedDataBuffer at the receivedDataBufferIndex position
        if(read((receivedDataBuffer + receivedDataBufferIndex), 1))
        {
            /*
             * If reached the end of the frame, then parse the device data and set it up in the correct
             * device output format to be called in [SIGNAL] deviceDataAvailable(deviceOutput)
            */
            if(receivedDataBuffer[receivedDataBufferIndex] == DEVICE_DATA_FRAME_END_DELIMITER)
            {
                /* Process the received device data frame in the buffer */
                QList<QVariant> deviceDataOut(2);
                // First byte in the device data frame is the device address
                uint8_t deviceAddress = receivedDataBuffer[0];

                switch (deviceAddress)
                {
                    case DEVICE_INTERNAL_ADDRESS_MOTOR:
                        deviceDataOut[0] = DEVICE_INTERNAL_ADDRESS_MOTOR;
                        deviceDataOut[1] = receivedDataBuffer[1];
                        break;
                    case DEVICE_INTERNAL_ADDRESS_ACCELEROMETER:
                        deviceDataOut[0] = DEVICE_INTERNAL_ADDRESS_ACCELEROMETER;

                        // Convert the received four data bytes back to float
                        float accelerometerValue;
                        memcpy (&accelerometerValue, (receivedDataBuffer + 1), 4);
                        deviceDataOut[1] = accelerometerValue;
                        break;
                    case DEVICE_INTERNAL_ADDRESS_LM35:
                        deviceDataOut[0] = DEVICE_INTERNAL_ADDRESS_LM35;

                        // Convert the received four data bytes back to float
                        float tempratureValue;
                        memcpy (&tempratureValue, (receivedDataBuffer + 1), 4);
                        deviceDataOut[1] = tempratureValue;
                        break;
                    default:
                        // Error Handing: Unknown device address
                        break;
                }

                // Emit device data available signal with the device data
                 emit deviceDataAvailable(deviceDataOut);

                // Reset the start of frame received status
                startOfDeviceDataFrameReceieved = false;
                // Reset the buffer index
                receivedDataBufferIndex = 0;
            }
            /*
             * If DEVICE_DATA_FRAME_END_DELIMITER hasn't been reached yet, then move to the next index in the buffer to write the next byte
            */
            else
            {
                receivedDataBufferIndex++;
            }
        }
    }
    /*
     * Check if the received byte is the DEVICE_DATA_FRAME_START_DELIMITER, so we can start
     * reading the device data into the buffer on the next readAndParseDeviceData() call
    */
    else if(read(receivedDataBuffer, 1) && receivedDataBuffer[0] == DEVICE_DATA_FRAME_START_DELIMITER)
    {
        startOfDeviceDataFrameReceieved = true;
    }

}
