import QtQuick 2.0

Flickable {
    anchors.fill: parent
    contentWidth: childrenRect.width
    contentHeight: childrenRect.height
     clip: true
     //boundsBehavior: Flickable.StopAtBounds

    Image {
        source: "res/map/nuremberg.png"
    }
}
