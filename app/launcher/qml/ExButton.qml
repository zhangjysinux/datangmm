import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: root
    width: 180; height: 180
    color: "black"

    property string pressedImage
    property string normalImage
    property alias count: countText.text

    signal clicked()

    Button {
        id: button
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 170
                implicitHeight: 170
                radius: 10
                Image {
                    source: control.pressed ? root.pressedImage : root.normalImage
                }
            }
        }
        onClicked: {
            root.clicked()
        }
    }

    Rectangle {
        id: countRect
        width: Math.max(50, countText.width + 20)
        height: 50
        anchors.top: parent.top
        anchors.right: parent.right
        radius: height
        border.width: 2
        border.color: "white"
        color: "red"
        visible: countText.text !== "0"
        Text {
            id: countText
            anchors.centerIn: parent
            text: "0"
            color: "white"
        }
    }
}

