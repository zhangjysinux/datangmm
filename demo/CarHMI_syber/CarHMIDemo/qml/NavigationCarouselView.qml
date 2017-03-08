import QtQuick 2.0

Item {
    anchors.fill: parent

    Image {
        id: appIcon

        source: "res/imgs/button_navigator.png"
        anchors.centerIn: parent
    }

    Image {
        source: "res/imgs/arrow_right.png"
        anchors.top: parent.top
        anchors.topMargin: 60
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        text: "Turn right in 500 meters"
        font.family: styles.normalFont
        font.pixelSize: styles.normalSize
        color: styles.lowColor
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
                text: "Distance:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "10.7 km"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "ETA:"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.highlightColor
            }

            Text {
                text: "16:14"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
            }
        }
    }
}
