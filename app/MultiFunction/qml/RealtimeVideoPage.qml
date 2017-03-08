import QtQuick 2.0
import QtMultimedia 5.2
import com.syberos.basewidgets 2.0
import Sinux.ScreenShot 1.0

CPage {
    id: root
    property string ip
    Component.onCompleted: {
        realtimeVideo.setScreenShot(screen)
    }

    contentAreaItem: Item {
        Item {
            id: titleRect
            width: parent.width
            height: 96
            anchors.left: parent.left
            anchors.top: parent.top
            z: 2

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }


            // 返回按钮
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
                        pageStack.pop();
                        realtimeVideo.callRefuse();
                    }
                }
            }

            Item {
                width: 72; height: 72
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 32

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/res/switch.png"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }
        }
          ScreenShot {
            id: screen
            objectName: "screenShot"
//            anchors.top: titleRect.Bottom
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.bottom: parent.bottom
            anchors.fill: parent
        }
    }
}
