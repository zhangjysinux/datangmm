import QtQuick 2.0

Item {
    anchors.fill: parent

    Image {
        id: appIcon

        source: "res/imgs/button_my_car.png"
        anchors.centerIn: parent
    }

    Image {
        source: "res/imgs/icon_warning_oil.png"
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        text: "Check oil level"
        font.family: styles.normalFont
        font.pixelSize: styles.normalSize
        color: styles.orange
        anchors.horizontalCenter: appIcon.horizontalCenter
        anchors.bottom: appIcon.top
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
                text: "Outside temperature:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "21 °C"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Next service:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "3,000 km"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }
    }
}
