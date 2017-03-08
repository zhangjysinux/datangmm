import QtQuick 2.0
import com.syberos.basewidgets 2.0

Item {
    id: videoIn
    height: 1280
    width: 720
    property var callId: "0"
    property var phoneNum: ""

    //orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        videoIn.color = "black"
        if (mainroot.getScreen() == 2) {
            //videoIn.statusBarHoldEnabled = false
            //gScreenInfo.setStatusBar(false);
            photo.anchors.topMargin = 24
        } else {
            //videoIn.statusBarHoldEnabled = true
            //gScreenInfo.setStatusBar(true);
            photo.anchors.topMargin = 208
        }
    }
    Connections{
        target: mainroot
        onItemChanged: {
            if (ret == 2) {
                //videoIn.statusBarHoldEnabled = false
                //gScreenInfo.setStatusBar(false);
                photo.anchors.topMargin = 24
            } else {
                //videoIn.statusBarHoldEnabled = true
                //gScreenInfo.setStatusBar(true);
                photo.anchors.topMargin = 208
            }
        }
    }

    Connections{
        target: csDbus
        onVideoHangup: {
            pageStack.pop()
        }
    }

    //contentAreaItem: Item {
    Item {
        anchors.fill: parent
        Item {
            id: mid
            anchors.fill: parent
            Image {
                anchors.fill: parent
                source: "qrc:/main/images/back_mid.png"
            }
            Image {
                fillMode: Image.Tile
                anchors.fill: parent
                source: "qrc:/main/images/back_mid_up.png"

            }
        }

        CMaskImage {
            id: photo
            width: 220
            height: 220
            iconSource: csDbus.getContacterData("photo") == "" ? "qrc:/main/images/init_photo.png" : csDbus.getContacterData("photo")
            anchors.top: parent.top
            anchors.topMargin: 208
            anchors.horizontalCenter: parent.horizontalCenter
            maskComponent: Rectangle{
                id: msk
                parent: photo
                radius: photo.width  / 2
                anchors.fill: parent
                smooth: true
                visible: false
            }
            Image {
                id: videophoto
                source: "qrc:/video/images/video/videoin-photo.png"
                opacity: 0.8
                anchors.fill: parent
            }
        }
        Text {
            id: name1
            height: 70
            text: csDbus.getContacterData("name")
            font.pixelSize: 70;color: "#ffffff"
            anchors.top: photo.bottom
            anchors.topMargin: 48;
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id: phoneNum
            height: 70
            text: videoIn.phoneNum
            font.pixelSize: 70;color: "#ffffff"
            anchors.top: name1.bottom
            anchors.topMargin: 48;
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 80
            spacing: 100
            anchors.horizontalCenter: parent.horizontalCenter
            CButton {
                id: answer
                width: 152
                height: 152
                iconSource: "qrc:/main/images/reject.png"
                backgroundEnabled: false
                onClicked: {
                    csDbus.hangupVideo()

                }
            }
            CButton {
                id: refuse
                width: 152
                height: 152
                iconSource: "qrc:/main/images/accept.png"
                backgroundEnabled: false
                onClicked: {
                    //pageStack.replace("qrc:/qml/VideoCall.qml",{"callId":callId,"phoneNum":phoneNum},true)
                    csDbus.answerVideo()
                    mainroot.answerVideo()
                }
            }
        }
    }
}
