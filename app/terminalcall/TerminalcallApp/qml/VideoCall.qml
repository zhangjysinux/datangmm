import QtQuick 2.0
import com.syberos.basewidgets 2.0
import Sinux.ScreenShot 1.0
import QtMultimedia 5.2

CPage {
    id: videoCall
    property var callId: "0"
    property var phoneNum: ""
    property var network: 0
    property bool isVideo: false
    property bool bigchange: true
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        csDbus.setAudioMode(network,"ihf")
        csDbus.getNamebyDialnum(phoneNum)
        findNet()
        videoCall.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            videoCall.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            videoCall.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }

    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                videoCall.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                bottomBack.height = 276
                buttonBox.anchors.top = bottomBack.top
                nametitle.anchors.horizontalCenterOffset = - (640 - 16 - nametitle.width)
                button1.scale = 0.7
                button2.scale = 0.7
                button3.scale = 0.7
                //button4.scale = 0.7
                hangButton.scale = 0.7
                hangButton.anchors.topMargin = -16
            } else {
                videoCall.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                bottomBack.height = 493
                buttonBox.anchors.top = nametitle.bottom
                nametitle.anchors.horizontalCenterOffset = 0
                button1.scale = 1
                button2.scale = 1
                button3.scale = 1
                //button4.scale = 1
                hangButton.scale = 1
                hangButton.anchors.topMargin = 16
            }
        }
    }

    Connections {
        target: csDbus
        onSignalVidChanged: {
            if (isVideoHold)
                pageStack.push("qrc:/qml/VideoTransform.qml",{"handId":callId,"phoneNum":phoneNum,"network":network})
        }
        onVideoHangup: {
            pageStack.pop()
        }
    }

    Connections {
        target: csDbus
        onSignalUpdateTimer: {
            time.text = timeString
        }
    }
    function findNet(network)
    {
        var net;
        if (network == 1)
            net = "自组网"
        else if (network == 16)
            net = "LTE"
        else if (network == 256)
            net = "天通网"
        else if (network == 1048576)
            net = "WIFI"
//        netText.text = net
//        netText1.text = net
        return net;
    }

    function bigChange()
    {
        if (bigchange){
            csDbus.setScreenShot(screenV4l2)
            csDbus.setScreenShotV4l2(screen)
            bigchange = false
        }
        else {
            csDbus.setScreenShot(screen)
            csDbus.setScreenShotV4l2(screenV4l2)
            bigchange = true
        }
    }

    contentAreaItem: Item {
        anchors.fill: parent
        Component.onCompleted: {
            csDbus.setScreenShot(screen)
            csDbus.setScreenShotV4l2(screenV4l2)
            bigchange = true
        }

        Rectangle {
            id: title
            color: "#293443"
            width: parent.width
            height: 100
            anchors.top: parent.top
            z:6
            Rectangle {
                color: Qt.rgba(0,0,0,0)
                border.color: "#29afe2"
                border.width: 2
                radius: 10
                visible: true//csDbus.isAudioExit() ? true : false
                width: 300
                height: 70
                anchors.centerIn: parent
                CButton {
                    id: audio
                    width: 150
                    height: 70
                    anchors.left: parent.left
                    backgroundEnabled: false
                    text: "语音"
                    textColor: "#29afe2"
                    onClicked: {
                        pageStack.replace("qrc:/qml/Talking.qml")
                    }
                }
                CButton {
                    id: video
                    width: 150
                    height: 70
                    anchors.left: audio.right
                    backgroundEnabled: true
                    backgroundComponent: Rectangle {
                        anchors.fill: parent
                        color: "#29afe2"
                        radius: 10
                    }
                    text: "视频"
                    textColor: "#ffffff"
                }
            }
        }


        ScreenShot {
            id: screenV4l2
            z: 5
            objectName: "screenShotV4l2"
            width: 172
            height: 218
            anchors.top: title.bottom
            anchors.topMargin: 24
            anchors.right: parent.right
            anchors.rightMargin: 24
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    bigChange()
                }
            }
        }

        ScreenShot {
            id: screen
            objectName: "screenShot"
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    bigChange()
                }
            }
        }

        Rectangle {
            width: 172
            height: 40
            opacity: 0.5
            color: "#000000"
            anchors.bottom: screenV4l2.bottom
            anchors.left: screenV4l2.left
            z: 6
            Item {
                id: v4l2Bottom
                anchors.centerIn: parent
                height: 26
                width: netIcon1.width+netText1.width
                Text {
                    id: netText1
                    height: 23
                    text: findNet(network)
                    font.pixelSize: 22;color: "#29afe2"
                }
                Image {
                    id: netIcon1
                    anchors.left: netText1.right
                    anchors.leftMargin: 8
                    width: 38
                    height: 26
                    source: "qrc:/main/images/net-on.png"
                }

            }
        }

        Item {
            id: bottomBack
            height: 493
            width: parent.width
            anchors.bottom: parent.bottom
            //color: "#000000"
            opacity: 1
            CButton {
                id: hangButton
                width: 150
                height: 150
                backgroundEnabled: false
                anchors.top: buttonBox.bottom
                anchors.topMargin: 16
                anchors.horizontalCenter: parent.horizontalCenter
                iconSource: "qrc:/main/images/hangup.png"
                onClicked: {
                    csDbus.hangupVideo()
                }
            }

            Text {
                id: time
                text: "00:00"
                font.pixelSize: 30
                height: 30
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 16
                color: "#29afe2"

                Timer {
                    id: timer
                    repeat: true
                    running: true
                    interval: 1000
                    onTriggered: {
                        time.text = csDbus.getTimeString(callId)
                    }
                }
            }

            Item {
                id: nametitle
                anchors.top: time.bottom
                //anchors.topMargin: 24
                width: net.width + netIcon.width + netText.width
                anchors.horizontalCenter: parent.horizontalCenter
                visible: true
                height: 76
                property alias name: name1.text
                //property alias net: netText.text
                Text {
                    id: net
                    height: 30
                    text: qsTr("对方")
                    anchors.top: parent.top
                    anchors.topMargin: 16
                    font.pixelSize: 30;color: "#29afe2"
                }
                Image {
                    id: netIcon
                    source: "qrc:/main/images/net-on.png"
                    anchors.top: net.top
                    anchors.left: net.right
                    anchors.leftMargin: 8
                }
                Text {
                    id: netText
                    height: 30
                    text: findNet(network)
                    anchors.top: net.top
                    anchors.left: netIcon.right
                    anchors.leftMargin: 8
                    font.pixelSize: 30;color: "#29afe2"
                }
                Text {
                    id: name1
                    height: 30
                    text: csDbus.getContacterData("name") == "" ? videoCall.phoneNum : csDbus.getContacterData("name") + " " + csDbus.getContacterData("title");//"李军 排长"
                    anchors.top: net.bottom
                    anchors.topMargin: 16
                    font.pixelSize: 30;color: "#29afe2"
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }

            Row {
                id: buttonBox
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: nametitle.bottom
                anchors.topMargin: 16
                height: 150
                CButton {
                    id: button1
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/video-1.png"
                    onClicked: {
                        csDbus.setMuteMicrophone(network,!csDbus.getIsMicrophoneMuted(network))
                    }
                }
                CButton {
                    id: button2
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/video-2.png"
                    onClicked: {
                        csDbus.setVidInputChanged(network, callId)
                    }
                }
                CButton {
                    id: button3
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/video-3.png"
                    onClicked: {
                        var temp = csDbus.getGetAudOrVideo(network, callId)
                        if (temp == 0)
                            csDbus.setVidEnabledChanged(network, callId, false)
                        else if (temp == 1)
                            csDbus.setVidEnabledChanged(network, callId, true)
                    }
                }
//                CButton {
//                    id: button4
//                    visible: false
//                    width: 150
//                    height: 150
//                    backgroundEnabled: false
//                    iconSource: pressed ? "qrc:/video/images/video/4-2.png" : "qrc:/video/images/video/4-1.png"
//                    onClicked: {
//                        var value = csDbus.getAudioMode(network);       //免提"ihf"表示免提， "earpiece"表示听筒
//                        if (value == "ihf")
//                            value = "earpiece"
//                        else if (value == "earpiece")
//                            value = "ihf"
//                        csDbus.setAudioMode(network,value)
//                    }
//                }
            }
        }
    }
}
