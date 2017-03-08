import QtQuick 2.0
import com.syberos.basewidgets 2.0
import Sinux.ScreenShot 1.0
import QtMultimedia 5.2

Item {
    id: videoCall
    height: 1280
    width: 720
    property var callId: "0"
    property var phoneNum: ""
    property var network: 0
    property bool isVideo: false
    property bool bigchange: true
    //orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        csDbus.setAudioMode(network,"ihf")
        csDbus.getNamebyDialnum(phoneNum)
        audio.visible = csDbus.isAudioExit() ? true : false

        findNet()
        videoCall.color = "black"
        if (mainroot.getScreen() == 2) {
            //videoCall.statusBarHoldEnabled = false
            //gScreenInfo.setStatusBar(false);
        } else {
            //videoCall.statusBarHoldEnabled = true
            //gScreenInfo.setStatusBar(true);
        }
        csDbus.setVideoPage(true)
    }
    Component.onDestruction: {
        csDbus.setVideoPage(false)
    }
//    onStatusChanged: {
//        if (status == CPageStatus.WillShow)
//        {
//            audio.visible = csDbus.isAudioExit() ? true : false
//        }
//    }

    Connections{
        target: mainroot
        onItemChanged: {
            if (ret == 2) {
                //videoCall.statusBarHoldEnabled = false
                //gScreenInfo.setStatusBar(false);
                bottomBack.height = 276
                buttonBox.anchors.top = bottomBack.top
                nametitle.anchors.horizontalCenterOffset = - (640 - 16 - nametitle.width)
                button1.scale = 0.7
                button2.scale = 0.7
                button3.scale = 0.7
                button4.scale = 0.7
                hangButton.scale = 0.7
                hangButton.anchors.topMargin = -16
            } else {
                //videoCall.statusBarHoldEnabled = true
                //gScreenInfo.setStatusBar(true);
                bottomBack.height = 493
                buttonBox.anchors.top = nametitle.bottom
                nametitle.anchors.horizontalCenterOffset = 0
                button1.scale = 1
                button2.scale = 1
                button3.scale = 1
                button4.scale = 1
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
//        onSignalVideoReturnPage: {
//            pageStack.pop()
//        }
    }

    Connections {
        target: csDbus
        onSignalUpdateTimer: {
            //if ()
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

//    contentAreaItem: Item {
      Item {

        anchors.fill: parent
        Component.onCompleted: {
            csDbus.setScreenShot(screen)
            csDbus.setScreenShotV4l2(screenV4l2)
            bigchange = true
        }
        Component.onDestruction: {
            csDbus.clearScreenShot()
        }

        CButton {
            id: audio
            width: 100
            height: 100
            anchors.left: parent.left
            anchors.bottom: bottomback.top
            backgroundEnabled: false
            iconSource: pressed ? "qrc:/video/images/changeaudio-click.png" : "qrc:/video/images/changeaudio.png"
            visible: csDbus.isAudioExit() ? true : false
            z:10
            onClicked: {
                mainroot.answerAudio()
            }

//                if (csDbus.getAudioPage())
//                    pageStack.pop()
//                else
//                    pageStack.push("qrc:/qml/Talking.qml")
//            }
        }

        ScreenShot {
            id: screenV4l2
            z: 10
            objectName: "screenShotV4l2"
            width: 172
            height: 218
            anchors.top: parent.top
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

        Rectangle {
            width: 172
            height: 40
            opacity: 0.5
            color: "#000000"
            anchors.top: screenV4l2.bottom
            anchors.left: screenV4l2.left
            z:10
            Item {
                id: v4l2Bottom
                anchors.fill: parent
                //anchors.verticalCenter: parent.verticalCenter
                Image {
                    id: netIcon1
                    source: "qrc:/main/images/net-on.png"
                    anchors.top: parent.top
                }
                Text {
                    id: netText1
                    height: 30
                    text: findNet(csDbus.getVideoNetWork())
                    anchors.top: parent.top
                    anchors.left: netIcon1.right
                    anchors.leftMargin: 8
                    font.pixelSize: 30;color: "#c9c9c9"
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
            id: bottomBack
            height: 493
            width: parent.width
            anchors.bottom: parent.bottom
            color: "#000000"
            opacity: 0.5
            CButton {
                id: hangButton
                width: 150
                height: 150
                backgroundEnabled: false
                anchors.top: buttonBox.bottom
                anchors.topMargin: 16
                anchors.horizontalCenter: parent.horizontalCenter
                iconSource: pressed ? "qrc:/video/images/video/hangup-click.png" :"qrc:/video/images/video/hangup.png"
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
                color: "#c9c9c9"

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
                    font.pixelSize: 30;color: "#c9c9c9"
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
                    text: findNet(csDbus.getVideoNetWork())
                    anchors.top: net.top
                    anchors.left: netIcon.right
                    anchors.leftMargin: 8
                    font.pixelSize: 30;color: "#c9c9c9"
                }
                Text {
                    id: name1
                    height: 30
                    text: csDbus.getContacterData("name") == "" ? videoCall.phoneNum : csDbus.getContacterData("name") + " " + csDbus.getContacterData("title");//"李军 排长"
                    anchors.top: net.bottom
                    anchors.topMargin: 16
                    font.pixelSize: 30;color: "#c9c9c9"
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
                    iconSource: pressed ? "qrc:/video/images/video/1-2.png" : "qrc:/video/images/video/1-1.png"
                    onClicked: {
                        csDbus.setMuteMicrophone(network,!csDbus.getIsMicrophoneMuted(network))
                    }
                }
                CButton {
                    id: button2
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/video/images/video/2-2.png" : "qrc:/video/images/video/2-1.png"
                    onClicked: {
                        csDbus.setVidInputChanged(network, callId)
                    }
                }
                CButton {
                    id: button3
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/video/images/video/3-2.png" : "qrc:/video/images/video/3-1.png"
                    onClicked: {
                        var temp = csDbus.getGetAudOrVideo(network, callId)
                        if (temp == 0)
                            csDbus.setVidEnabledChanged(network, callId, false)
                        else if (temp == 1)
                            csDbus.setVidEnabledChanged(network, callId, true)
                    }
                }
                CButton {
                    id: button4
                    visible: false
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/video/images/video/4-2.png" : "qrc:/video/images/video/4-1.png"
                    onClicked: {
                        var value = csDbus.getAudioMode(network);       //免提"ihf"表示免提， "earpiece"表示听筒
                        if (value == "ihf")
                            value = "earpiece"
                        else if (value == "earpiece")
                            value = "ihf"
                        csDbus.setAudioMode(network,value)
                    }
                }
            }
        }
    }
}
