import QtQuick 2.0

Item {
    anchors.fill: parent

    Image {
        id: appIcon

        source: "res/imgs/button_drive.png"
        anchors.centerIn: parent
    }

    Row {
        id: speedInd

        anchors.horizontalCenter: appIcon.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 70
        spacing: 10

        Text {
            text: currentSpeed
            font.family: styles.normalFont
            font.pixelSize: styles.hugeNumberSize
            font.bold: true
            color: styles.lowColor
        }

        Text {
            text: "km/h"
            font.family: styles.normalFont
            font.pixelSize: styles.normalSize
            color: styles.lowColor
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Text {
        text: "Speed"
        font.family: styles.normalFont
        font.pixelSize: styles.normalSize
        color: styles.highlightColor
        anchors.horizontalCenter: speedInd.horizontalCenter
        anchors.top: speedInd.bottom
    }

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: appIcon.bottom
        anchors.topMargin: 50
        spacing: 10

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Temp:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "22.5 °C"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Trip:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "2.46 km"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "AVG Speed:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "104 km/h"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Fuel range:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "170 km"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }
    }
}
