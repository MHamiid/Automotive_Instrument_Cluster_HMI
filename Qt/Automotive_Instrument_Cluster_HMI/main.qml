import QtQuick 2.15
import QtQuick.Window 2.15
import Serial

Window
{
    id: window
    width: 640
    height: 480
    visible: true
    title: "Automotive Instrument Cluster HMI"
    color: "black"

    Rectangle
    {
        property int speed: 120
        id: speedometer
        width: 300
        height: 300
        radius: width * 0.5      // Half the width (or height). As a circle is width = height and radius is half of that
        antialiasing: true
        anchors.centerIn: parent
        color: "transparent"
        border.color: "lightgreen"
        border.width: 5

        Serial
        {
            id: serial
            baudRate: Serial.Baud4800
            dataBits: Serial.Data8
            parity: Serial.NoParity
            stopBits: Serial.OneStop
            portName:"COM1"
            openMode: 0x0001 | 0x0002  // Open in ReadWrite mode
            onDeviceDataAvailable:
                function (device)
                {
                    var deviceAddress = device[0]
                    var deviceData = device[1]
                }
        }

        Text
        {
            id: speed
            anchors.top: parent.top
            anchors.topMargin: 70
            anchors.horizontalCenter: parent.horizontalCenter
            text: speedometer.speed
            color: "white"
            font.pixelSize: 80
            antialiasing: true

            Text
            {
                id: speedUnit
                anchors.top: parent.top
                anchors.topMargin: 100
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Km/h"
                color: speedometer.border.color
                font.pixelSize: 30
                font.bold: true
                antialiasing: true
            }
        }
    }
}
