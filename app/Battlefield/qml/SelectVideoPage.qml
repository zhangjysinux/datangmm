import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: root

    contentAreaItem: Item {

        Image {
            anchors.fill: parent
            fillMode: Image.Tile
            source: "qrc:/res/bg.png"
        }
        // 标题区域
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
                    }
                }
            }
        }

        Column {
            anchors.centerIn: parent
            spacing: 32

            CColorButton {
                width: 280; height: 260
                backgroundColor:"#272727"
                backgroundComponent: Rectangle {

                    Text {
                        anchors.centerIn: parent
                        text: "视频"
                        font.pointSize: 18
                        //color:"white"
                    }
                    border.color:"white"
                    border.width:2
                    //color: "#272727"
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#272727" }
                        GradientStop { position: 1.0; color: "#7B7B7B" }
                    }
                }

                onClicked: {
                    pageStack.push("qrc:/qml/VideoViewPage.qml");
                }
            }

            CColorButton {
                width: 280; height: 260
                backgroundComponent: Rectangle {
                    border.color:"white"
                    border.width:2
                    Text {
                        anchors.centerIn: parent
                        text: "照片"
                        font.pointSize: 18
                    }
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#272727" }
                        GradientStop { position: 1.0; color: "#7B7B7B" }
                    }
                }

                onClicked: {
                    pageStack.push("qrc:/qml/PhotoViewPage.qml");
                }
            }

        }
    }
}
