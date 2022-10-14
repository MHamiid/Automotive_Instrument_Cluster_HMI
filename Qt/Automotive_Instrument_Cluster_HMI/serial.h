#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>

class Serial : public QSerialPort
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QIODeviceBase::OpenMode openMode READ openMode WRITE open NOTIFY openModeChanged)

public:
    Serial(QObject* parent = nullptr);

Q_SIGNALS:
    /* These signals are currently not emitted/used, they are mostly here to avoid Qt warnings */
    void portNameChanged(QString portName);
    void openModeChanged(QIODeviceBase::OpenMode openMode);
};

#endif // SERIAL_H
