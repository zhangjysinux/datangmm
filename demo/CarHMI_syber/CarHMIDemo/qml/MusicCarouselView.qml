import QtQuick 2.0

Item {
    Image {
        id: appIcon
        source: "res/imgs/button_music.png"
        anchors.centerIn: parent
    }

    Row {
        anchors.horizontalCenter: appIcon.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 80
        spacing: 10

        Image {
            id: button_prev
            source: "res/imgs/button_prev.png"

            MouseArea {
                anchors.fill: parent
                onPressed: setHighlight(parent)
                onReleased: removeHighlight(parent)
            }

        }

        Image {
            id: button_play
            source: "res/imgs/button_play.png"

            MouseArea {
                anchors.fill: parent
                onPressed: setHighlight(parent)
                onReleased: removeHighlight(parent)
            }
        }

        Image {
            id: button_next
            source: "res/imgs/button_next.png"

            MouseArea {
                anchors.fill: parent
                onPressed: setHighlight(parent)
                onReleased: removeHighlight(parent)
            }
        }
    }
}
