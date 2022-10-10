#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>

class Serial : public QSerialPort
{
    Q_OBJECT
public:
    Serial(QObject* parent = nullptr);
};

#endif // SERIAL_H
