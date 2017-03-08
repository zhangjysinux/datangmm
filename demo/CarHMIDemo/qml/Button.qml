import QtQuick 2.0
import QtGraphicalEffects 1.0

Text {
    id: wrapper

    width: contentWidth + 10
    height: parent.height

    signal clicked()

    color: "white"

    font.family: styles.normalFont
    font.pixelSize: 15

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter

    Rectangle {
        anchors.fill: parent
        border.width: 1
        border.color: "steel blue"
        radius: 10
        color: "transparent"
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: wrapper.clicked()
    }

    Glow {
        id: highlightEffect

        anchors.fill: parent
        radius: 20
        spread: 1.0
        color: "white"
        visible: mouse.pressed
        source: parent
    }
}
