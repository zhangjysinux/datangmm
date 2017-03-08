import QtQuick 2.0
import QtMultimedia 5.2
import com.syberos.basewidgets 2.0

CPage {
    id: root

    property alias source: mediaPlayer.source

    contentAreaItem: Item {

        MediaPlayer {
            id: mediaPlayer

            onStatusChanged: {
                if (mediaPlayer.status === MediaPlayer.EndOfMedia) {
                    mediaPlayer.stop();
                    playRect.visible = true;
                }
            }
        }

        VideoOutput {
            anchors.fill: parent
            source: mediaPlayer
            fillMode: VideoOutput.Stretch

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mediaPlayer.pause();
                    playRect.visible = true;
                }
            }
        }

        //TitleRect
        Item {
            id: titleRect
            width: parent.width
            height: 96
            anchors.left: parent.left
            anchors.top: parent.top

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }

            Item {
                width: 96; height: 72
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 32

                Text {
                    anchors.centerIn: parent
                    text: "<返回"
                    color: "white"
                    font.pointSize: 16
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        pageStack.pop();
                    }
                }
            }
        } //end titleRect

        //bottomRect
        Item {
            id: bottomRect
            width: parent.width
            height: 236
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }

            // 进度条
            Item {
                id: pos
                width: parent.width - 64
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 16

                Rectangle {
                    width: parent.width
                    height: 3
                    anchors.centerIn: parent
                    color: "white"
                }

                Rectangle {
                    width: 50
                    height: 50
                    radius: width
                    x: mediaPlayer.duration > 0 ? parent.width * mediaPlayer.position / mediaPlayer.duration : 0
                    y: 0
                }

            }

            Item {
                width: 72; height: 72
                anchors.top: pos.bottom
                anchors.left: parent.left
                anchors.topMargin: 16
                anchors.leftMargin: 32

                Rectangle {
                    anchors.fill: parent
                    color: "#E0E0E0"
                    opacity: 0.5
                }

                Image {
                    anchors.fill: parent
                    source: "qrc:/res/remove.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }

            Item {
                width: 72; height: 72
                anchors.top: pos.bottom
                anchors.right: parent.right
                anchors.topMargin: 16
                anchors.rightMargin: 32

                Rectangle {
                    anchors.fill: parent
                    color: "#E0E0E0"
                    opacity: 0.5
                }

                Image {
                    anchors.fill: parent
                    source: "qrc:/res/export.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }
        } // end bottomRect

        // playRect
        Item {
            id: playRect
            anchors.left: parent.left
            anchors.top: titleRect.bottom
            anchors.right: parent.right
            anchors.bottom: bottomRect.top

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }

            Item {
                width: 240; height: 230
                anchors.centerIn: parent
                Image {
                    anchors.fill: parent
                    source: "qrc:/res/play.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        mediaPlayer.play();
                        playRect.visible = false;
                    }
                }
            }
        }

    }
}
