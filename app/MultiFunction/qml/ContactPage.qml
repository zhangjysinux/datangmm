import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: root
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
                text: "选择联系人"
                color: "white"
                font.pointSize: 16
            }
        } //end标题区域

        ListView {
            id: contactView
            anchors {
                left: parent.left; right: parent.right
                top: titleRect.bottom; bottom: parent.bottom
            }
            model: contactModel
            delegate: Rectangle {
                width: contactView.width
                height: contactView.height / 7
                border.color: "lightgray"
                Text {
                    anchors.centerIn: parent
                    text: ip
                    font.pointSize: 13
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        realtimeVideo.callRemote(ip);
                        pageStack.push("qrc:/qml/MainPage.qml",{waitVisable:true,ip:ip});
                    }
                }
            }
        }

        ListModel {
            id: contactModel
            ListElement { ip: "192.168.1.2" }
            ListElement { ip: "192.168.1.3" }
            ListElement { ip: "192.168.1.4" }
            ListElement { ip: "192.168.100.50" }
        }
    }
}
