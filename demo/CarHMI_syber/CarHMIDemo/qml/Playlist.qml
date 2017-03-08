import QtQuick 2.0

Rectangle {
    id: view
    width: 350
    property variant playlist: undefined
    anchors.margins: 5

    color: "transparent"
    border.color: "gray"
    border.width: 1
    radius: 10

    Text {
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "Select\na\nPlaylist"
        font.pointSize: 72
        color: "white"
        style: Text.Sunken
        wrapMode: Text.Wrap
        visible: view.playlist == undefined
    }

    PlaylistThumbnail {
        id: thumbnail
        covers: playlist ? playlist.albumArtsForPlaylistImage() : []
        anchors.top: view.top
        anchors.margins: view.anchors.margins * 2
        anchors.horizontalCenter: view.horizontalCenter
    }

    Text {
        id: heading

        anchors.top: thumbnail.bottom
        anchors.left: view.left
        anchors.right: view.right
        anchors.margins: 2 * view.anchors.margins

        text: playlist ? playlist.name : ""
        color: "white"
        font.pixelSize: 30
        wrapMode: Text.Wrap
        style: Text.Sunken
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
    }

    ListView {
        id: trackList

        anchors.top: heading.bottom
        anchors.left: view.left
        anchors.right: view.right
        anchors.bottom: view.bottom
        anchors.margins: 2 * view.anchors.margins

        model: playlist ? playlist.tracks : []
        clip: true
        delegate: trackDelegate
        focus: true

        Component {
            id: trackDelegate
            Text {
                id: wrapper
                width: trackList.width
                height: 20
                text: (index + 1) + ". " + modelData.artists + " - " + modelData.name
                color: "white"
                font.bold: true
                style: Text.Sunken
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight

                Rectangle {
                    anchors.fill: parent
                    visible: spotify.currentTrack === modelData
                    color: "transparent"
                    border.color: "blue"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.debug("PLAYING " + modelData.name);
                        modelData.play();
                    }
                }
            }
        }
    }
}
