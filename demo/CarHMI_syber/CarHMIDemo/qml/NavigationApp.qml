import QtQuick 2.0

AppBase {
    carouselView: NavigationCarouselView {
        anchors.fill: parent
        anchors.centerIn: parent
    }

    detailsView:
        Rectangle {
            anchors.left: parent.left
            anchors.leftMargin: parent.width*0.04
            anchors.right: parent.right
            anchors.rightMargin: parent.width*0.04
            anchors.top: parent.top
            anchors.topMargin: 140
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.width*0.04
            border.width: 1
            border.color: styles.highlightColor
            color: "transparent"

            Text {
                id: loadingText

                anchors.centerIn: map
                text: "Map loading..."
                font.family: styles.normalFont
                color: styles.highlightColor
                font.pixelSize: 60
            }

            Loader {
                id: map

                source: useWebkit ? "MapViewWebkit.qml" : "MapViewNoWebkit.qml"
                anchors.fill: parent
            }
        }
}
