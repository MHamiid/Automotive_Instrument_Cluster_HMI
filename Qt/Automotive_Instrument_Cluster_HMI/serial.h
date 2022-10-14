#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>
#include <QVariant>
#include <QList>

class Serial : public QSerialPort
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QIODeviceBase::OpenMode openMode READ openMode WRITE open NOTIFY openModeChanged)

public:
    enum DeviceInternalAddress
    {
        DEVICE_INTERNAL_ADDRESS_MOTOR			=     0x01,
        DEVICE_INTERNAL_ADDRESS_ACCELEROMETER	=     0x02,
        DEVICE_INTERNAL_ADDRESS_LM35			=     0x03
    };
    // Add Q_ENUM to make it callable in the QML side
    Q_ENUM(DeviceInternalAddress)

    Serial(QObject* parent = nullptr);

    /*
     * @brief [SLOT] Read and parse device date frame from the serial port
     *
     * This slot is connected to [SIGNAL] QIODevice::readyRead()
     * And emits [SIGNAL] deviceDataAvailable() when a complete device data frame is received
     *
     * @return void
    */
    void readAndParseDeviceData();

Q_SIGNALS:
    /* These signals (portNameChanged, openModeChanged) are currently not emitted/used, they are mostly here to avoid Qt warnings */
    void portNameChanged(QString portName);
    void openModeChanged(QIODeviceBase::OpenMode openMode);

    /*
     * @brief [SIGNAL] Emitted/Called whenever a complete device data frame is read
     *
     * @param deviceData { DeviceAddress, DeviceData }. Where DeviceData can be a byte or a float depending on which DeviceAddress is it
     *
     * @return void
    */
    void deviceDataAvailable(QList<QVariant> deviceData);

private:
    /*
     * Buffer size = Max device data frame size:
     *          1 byte for the device address, 4 bytes max for devices's data(as float is 4 bytes) ,or 1 byte, and 1 byte for the MESSAGE_END_DELIMITER
     *
     * Note that DEVICE_DATA_FRAME_START_DELIMITER gets always overridden in the buffer by the first
     * byte of the device data, as such there is no space allocated in the buffer for it.
    */
    char receivedDataBuffer[6] = { 0 };
    size_t receivedDataBufferIndex = 0;
    // Indication when true that data is being written to receivedDataBuffer
    bool startOfDeviceDataFrameReceieved = false;
};

#endif // SERIAL_H
