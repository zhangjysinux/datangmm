import QtQuick 2.0
import com.syberos.basewidgets 2.0

Rectangle {
    id: root
    width: parent.width
    height: 200
    visible: false

    Connections {
        target: callReminderVm
        onSignalShowScreen: {
            root.visible = true;
        }
        onSignalHideScreen: {
            root.visible = false;
        }
    }

    Rectangle {
        id: backgraund
        anchors.fill: parent
        color: "black"
        opacity: 0.3
    }

    Rectangle {
        id: backgraund2
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        width: parent.width
        height: 140
        color: "black"
        opacity: 0.6
    }

    CButton {
        id: acceptButton
        anchors.left: parent.left
        anchors.leftMargin: 32
        anchors.verticalCenter: parent.verticalCenter
        width: 152; height: 152
        backgroundComponent: Rectangle {
            radius: parent.width
            Image {
                source: "qrc:/res/bg.png"
                anchors.fill: parent
            }
            Image {
                source: "qrc:/res/accept.png"
                anchors.centerIn: parent
            }
        }
        onClicked: {
            callReminderVm.callAccept();
        }
    }

    CButton {
        id: resumeButton
        anchors.right: parent.right
        anchors.rightMargin: 32
        anchors.verticalCenter: parent.verticalCenter
        width: 152; height: 152
        backgroundComponent: Rectangle {
            radius: parent.width
            Image {
                source: "qrc:/res/bg.png"
                anchors.fill: parent
            }
            Image {
                source:"qrc:/res/resume.png"
                anchors.centerIn: parent
            }
        }
        onClicked: {
            callReminderVm.callResume();
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: 193; height: 189
        color: "transparent"

        CMaskImage {
            id: head
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 8
            width: 110; height: 110
            iconSource: {
                if (callReminderVm.headImg === "" || callReminderVm.headImg === "null") {
                    return "qrc:/res/defaulthead.png";
                } else {
                    return callReminderVm.headImg;
                }
            }
        }

        Text {
            id: name
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: head.bottom
            anchors.topMargin: 4
            text: callReminderVm.callInName
            font.pixelSize: 20
            color: "white"
        }

        Text {
            id: tel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: name.bottom
            text: callReminderVm.telphoneNumber
            font.pixelSize: 20
            color: "white"
        }
    }
}
