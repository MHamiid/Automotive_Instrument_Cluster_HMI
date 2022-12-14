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
        readonly property double fillPosition: 0.3
        readonly property int maxSpeed: 180
        id: speedometer
        width: 300
        height: 300
        radius: width * 0.5      // Half the width (or height). As a circle is width = height and radius is half of that
        antialiasing: true
        anchors.centerIn: parent
        color: "transparent"
        border.color: "lightgreen"
        border.width: 5
        gradient: Gradient
                  {
                    /**
                      * The (+/-)0.0001 to ensure the color gradiant has the min value when (speed.speedValue = 0),
                      * otherwise this GradientStop mostly would be ignored and give incorrect result
                     */
                    GradientStop { position: 0.0; color: speedometer.border.color }
                    GradientStop { position: ((speed.speedValue * speedometer.fillPosition) / speedometer.maxSpeed) + 0.0001; color: "black" }
                    GradientStop { position: (1.0 - ((speed.speedValue * speedometer.fillPosition) / speedometer.maxSpeed)) - 0.0001; color: "black" }
                    GradientStop { position: 1.0; color: speedometer.border.color }
                  }

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

                // Update the current velocity while keeping the max (speed) to speedometer.maxSpeed, if exceeded we just ignore the accelerometer value
                // Set a cap for min/max velocity to -speedometer.maxSpeed/speedometer.maxSpeed for both directions of the velocity
                speedometer.velocity = speedometer.velocity >= 0 ? Math.min(speedometer.maxSpeed, speedometer.velocity + KMHPerS) : Math.max(-speedometer.maxSpeed, speedometer.velocity + KMHPerS)
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

            Behavior on speedValue
            {
                NumberAnimation { duration: 1000 }
            }

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
