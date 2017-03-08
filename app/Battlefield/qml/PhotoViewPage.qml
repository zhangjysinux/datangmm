import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: root

    Component.onCompleted: {
        videoView.updatePhotos();
    }

    property string text: ""

    Connections {
        target: videoView
        onSignalRemoveFile: {
            if (isRemove) {
                root.text = "删除成功";
            } else {
                root.text = "删除失败";
            }
            infoRect.sourceComponent = infoComponent;
        }
    }

    contentAreaItem: Item {
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

            Text {
                anchors.centerIn: parent
                text: filesView.state == "play" ? "播放文件" : "删除文件"
                color: "white"
                font.pointSize: 16
            }

            Item {
                width: 72; height: 72
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 32

                Text {
                    anchors.centerIn: parent
                    text: filesView.state == "play" ? "刷新" : "删除"
                    color: mouseArea.pressed ? "red" : "white"
                    font.pointSize: 16
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        if (filesView.state == "remove") {
                            videoView.removePhotos();
                        } else {
                            videoView.updatePhotos();
                        }
                    }
                }
            }
        } //end标题区域

        ListView {
            id: filesView
            anchors {
                left: parent.left; right: parent.right
                top: titleRect.bottom; bottom: parent.bottom
            }
            focus: true
            state: "play"

            states: [
                State {
                    name: "play"
                    StateChangeScript {
                        script: {
                            removeRect.visible = false;
                        }
                    }
                },
                State {
                    name: "remove"
                    StateChangeScript {
                        script: {
                            removeRect.visible = true;
                        }
                    }
                }
            ]

            model: photoModel
            delegate: Rectangle {
                width: filesView.width
                height: filesView.height / 7
                border.color: "lightgray"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (filesView.state == "play") {
                            pageStack.push("qrc:/qml/PhotoPlayPage.qml",
                                           { imgSource: "file:///data/data/com.sinux.battlefield/images/" + fileName });
                        } else {
                            remove = (remove == "1" ? "0" : "1");
                        }
                    }

                    onPressAndHold: {
                        filesView.state = "remove";
                    }
                }

                Item {
                    id: moveImgItem
                    width: parent.width / 5
                    height: parent.height - 20
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 32

                    Image {
                        anchors.fill: parent
                        source: "file:///data/data/com.sinux.battlefield/images/" + fileName
                    }
                }

                Column {
                    id: moveInfoCol
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: moveImgItem.right
                    anchors.leftMargin: 32

                    Text {
                        font.pointSize: 10
                        text: fileName
                        wrapMode: Text.WrapAnywhere
                    }

                    Text {
                        font.pointSize: 10
                        text:size + "MB"
                        wrapMode: Text.WrapAnywhere
                    }
                }

                Item {
                    width: parent.height / 4
                    height: width
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 32
                    visible: filesView.state == "remove"

                    Image {
                        anchors.fill: parent
                        source: remove == "0" ? "qrc:/res/check2.png" : "qrc:/res/check.png"
                    }
                }
            }
        }//end GridView

        //remove Rectangle
        Item {
            id: removeRect
            width: parent.width
            height: 96
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.2
            }

            Item {
                width: 96; height: 72
                anchors.centerIn: parent

                Text {
                    anchors.centerIn: parent
                    text: "取消"
                    color: "white"
                    font.pointSize: 16
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        filesView.state = "play";
                    }
                }
            }
        } // end remove rect

        Loader {
            id: infoRect
            anchors.centerIn: parent
            onLoaded: {
                item.text = root.text;
            }
        }

        Component {
            id: infoComponent
            Rectangle {
                width: 492; height: 362
                border.color: "white"
                border.width: 5

                property alias text: text.text

                Image {
                    anchors.fill: parent
                    source: "qrc:/res/infobg.png"
                }

                Text {
                    id: text
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 124
                    color: "white"
                    font.pixelSize: 30
                }

                CButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 32
                    width: 170; height: 103
                    backgroundComponent: Item {
                        width: parent.width; height: parent.height
                        Image {
                            anchors.fill: parent
                            source: "qrc:/res/confirmbtn.png"
                        }
                    }
                    onClicked: {
                        infoRect.sourceComponent = null;
                    }
                }
            }
        } // end Component
    }
}
