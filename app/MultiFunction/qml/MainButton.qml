import QtQuick 2.0

Item {
    id: root
    width: 280
    height: 260

    property alias iconSource: img.source

    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked();
        }
        onPressedChanged: {
            if (pressed) {
                pressedRect.visible = true;
            } else {
                pressedRect.visible = false;
            }
        }
    }

    Image {
        id: img
        anchors.fill: parent
    }

    Rectangle {
        id: pressedRect
        anchors.fill: parent
        color: "#000"
        opacity: 0.5
        visible: false
    }
}
