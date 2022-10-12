#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>

class Serial : public QSerialPort
{
    Q_OBJECT
    Q_PROPERTY(QString portName READ portName WRITE setPortName)
    Q_PROPERTY(OpenMode openMode WRITE open)

public:
    Serial(QObject* parent = nullptr);
};

#endif // SERIAL_H
