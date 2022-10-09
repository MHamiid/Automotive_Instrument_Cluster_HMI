import QtQuick 2.15
import QtQuick.Window 2.15

Window
{
    id: window
    width: 640
    height: 480
    visible: true
    title: "Automotive Instrument Cluster HMI"

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

        Text
        {
            id: speed
            anchors.top: parent.top
            anchors.topMargin: 70
            anchors.horizontalCenter: parent.horizontalCenter
            text: speedometer.speed
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
