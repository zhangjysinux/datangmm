import QtQuick 2.0

Item {
    property string activeImage: ""
    property string inactiveImage: ""
    property bool toggled: false

    width: image.width
    height: image.height

    Image {
        id: image
        anchors.centerIn: parent
        source: parent.toggled ? parent.activeImage : parent.inactiveImage
        asynchronous: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: parent.toggled = !parent.toggled
    }
}
