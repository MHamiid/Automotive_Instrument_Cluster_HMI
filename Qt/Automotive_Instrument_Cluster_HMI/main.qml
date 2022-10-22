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
        property real velocity: 0
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
            property real accelerometer: 0
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

                    if(deviceAddress === Serial.DEVICE_INTERNAL_ADDRESS_ACCELEROMETER)
                    {
                        // Set the data
                        accelerometer = deviceData
                    }
                }
        }

        Timer {
            id: timer
            interval: 1000 // Update every 1 second
            repeat: true
            running: true

            onTriggered:
            {
                // Convert (G)s to (km/h)/s
                var KMHPerS = serial.accelerometer * 35.30394

                // Update the current velocity while keeping the max (speed) to 180, if exceeded we just ignore the accelerometer value
                // Set a cap for min/max velocity to -180/180 for both directions of the velocity
                speedometer.velocity = speedometer.velocity >= 0 ? Math.min(180, speedometer.velocity + KMHPerS) : Math.max(-180, speedometer.velocity + KMHPerS)
            }
        }

        Text
        {
            // Convert the velocity to speed (get the absolute value) and round the result to int
            property int speedValue: Math.abs(speedometer.velocity)
            id: speed
            anchors.top: parent.top
            anchors.topMargin: 70
            anchors.horizontalCenter: parent.horizontalCenter
            text: speedValue
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
