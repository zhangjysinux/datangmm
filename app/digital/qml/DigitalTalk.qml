import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Dialogs 1.0

CPage {
    id: diagital
    property int second: 0
    property int minute: 0
    property int hour: 0
    property bool bIsChange: false
    property bool bIsFailure: false
    property bool bIsGroup: false
    property bool isFromText: false
    property bool isSigNo: false

    property string groupid: "0"
    property string groupname: ""
    property string groupnum: "15"
    property string netinfo: ""
    property var prepage: null

    function getDigitalIcon()
    {
        if (bIsChange) {
            if (bIsFailure) {
                return "qrc:/digital/images/h-digital-failure.png"
            } else if (qiangmai.bIspress) {
                return "qrc:/digital/images/h-digital-click.png"
            } else {
                return "qrc:/digital/images/h-digital.png"
            }
        } else {
            if (bIsFailure) {
                return "qrc:/digital/images/digital-failure.png"
            } else if (qiangmai.bIspress) {
                return "qrc:/digital/images/digital-4.png"
            } else {
                return "qrc:/digital/images/digital-3.png"
            }
        }
    }

     function getSigOffIcon()
     {
        if(sigalOff.bIspress)
         {
             return "qrc:/digital/images/sigmode_open.png"
         }
        else
        {
            return "qrc:/digital/images/sigMode_close.png"
        }
     }
     function getSigPicIcon()
     {
        if(sigalpicclick.bIspress)
         {
             return "qrc:/digital/images/sigvoice_open.png"
         }
        else
        {
            return "qrc:/digital/images/sigvoice_close.png"
        }
     }
    orientationPolicy: CPageOrientation.LockLandscape
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            diagital.orientationPolicy = CPageOrientation.LockLandscape
        diagital.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            diagital.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            bIsChange = true
        } else {
            diagital.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            bIsChange = false
        }
    }
    Component.onDestruction: {
        getData.baoTongHangUp()
    }

    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                diagital.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                bIsChange = true
            } else {
                diagital.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                bIsChange = false
            }
        }
    }

    contentAreaItem: Item {
        anchors.fill: parent
        Item{
            id: mid
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            Image {
                fillMode: Image.Tile
                anchors.fill: parent
                source: "qrc:/main/images/back_mid_up.png"
            }
            Image {
                anchors.fill: parent
                source: "qrc:/main/images/back_mid.png"
            }
        }
//        CButton {
//            id: close
//            anchors.left: parent.left
//            anchors.top: parent.top
//            anchors.topMargin: 20
//            iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
//            backgroundEnabled: false
//            onClicked: {
//                pageStack.pop()
//            }
//        }
        Text {
            id: name
            anchors.top: parent.top
            anchors.topMargin: 32
            font.pixelSize: 30
            text: qsTr("战互网通话")
            color: "#ffffff"
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !bIsGroup
        }
        Row {
            anchors.top: parent.top
            anchors.topMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter
            visible: bIsGroup
            spacing: 0
            CButton {
                id: button1
                width: 122
                height: 50
                backgroundEnabled: false
                iconSource: "qrc:/digital/images/digital-button1.png"
                onClicked: {

                }
            }
            CButton {
                id: button2
                width: 122
                height: 50
                backgroundEnabled: false
                iconSource: "qrc:/digital/images/digital-button2.png"
                onClicked: {
                    if (isFromText)
                        pageStack.pop()
                    else
                        pageStack.push("qrc:/qml/DigitalGroup.qml","isFromTalk",true)
                }
            }
        }

        Item{
            id: person
            width: 672
            height: 196
            visible: bIsChange ? false : true
            anchors.top: name.bottom
            anchors.topMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            Image {
                anchors.fill: parent
                source: "qrc:/digital/images/digital-1.png"
            }
            Image {
                anchors.fill: parent
                source: "qrc:/digital/images/digital-2.png"
            }
//            Loader {
//                id: addPhoto
//                width: 150
//                height: 150
//                anchors.centerIn: parent
//                source: "qrc:/qml/Photo.qml"
//                onLoaded: addPhoto.item.myIconSource = "qrc:/main/images/init_photo.png"
//            }
        }
        Timer {
            id: timecount
            interval: 1000
            //running: true
            repeat: true
            onTriggered: {
                var hourstring,minutestring,secondstring
                second+=1
//                if (second == 10) {
//                    tip.visible = true
//                    bIsFailure = true
//                    tipanimation.start()
//                } else if(second == 1) {
//                    tip.visible = true
//                    bIsFailure = false
//                    tipanimation.start()
//                }

                if (second == 60) {
                    minute+=1
                    second = 0
                }
                if (minute == 60) {
                    hour+=1
                    minute = 0
                }
                if (minute < 10) {
                    minutestring = "0" + minute.toString()
                } else {
                    minutestring = minute.toString()
                }
                if (second < 10) {
                    secondstring = "0" + second.toString()
                } else {
                    secondstring = second.toString()
                }
                if (hour == 0) {
                    time.text = minutestring + ":" + secondstring
                } else {
                    if (hour < 10) {
                        hourstring = "0" + hour.toString()
                    } else {
                        hourstring = hour.toString()
                    }
                    time.text = hourstring + ":" + minutestring + ":" + secondstring
                }
            }
        }
        Rectangle {
            id: tip
            property alias mytext : qiangText.text
            anchors.top: bIsChange ? image.top : image.bottom
            anchors.left: parent.left
            anchors.leftMargin: bIsChange ? 540 : 260
            width: 200
            height: 88
            visible: false
            color: "black"
            opacity: 1
            z: 10
            Text {
                id: qiangText
                anchors.centerIn: parent
                //text: mbIsFailure ? qsTr("抢麦失败") : qsTr("抢麦成功")
                color: "white"
                font.pixelSize: 40
            }
        }
        Text {
            id: time
            anchors.top: bIsChange ? image.bottom : person.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin :  bIsChange ? 315: 481
            text: ""
            font.pixelSize: 40
            color: "green"
        }
        Image {
            id: image
            anchors.top: bIsChange ? name.bottom : time.bottom
            anchors.topMargin: bIsChange ? 24 : 16
            width: bIsChange ? 1216 : 663
            height: 70
            anchors.horizontalCenter: parent.horizontalCenter
            source: bIsChange ? "qrc:/digital/images/h-digital-voice.png" : "qrc:/digital/images/digital-6.png"
        }

        Image {
            width: bIsChange ? 434 : 500
            height: width
            anchors.top: bIsChange ? time.bottom : image.bottom
            anchors.topMargin: bIsChange ? 16 : 80
            anchors.left: parent.left
            anchors.leftMargin:  bIsChange ? 150 : 216
            source: bIsChange ? "qrc:/digital/images/h-digital-back.png" : "qrc:/digital/images/digital-5.png"
        }

        Image {
            id: clickSigoff
            width: bIsChange ? 270 : 336
            height: width*0.5
            z: 5
            anchors.top: image.bottom
            anchors.topMargin: bIsChange ? 50 : 114
            anchors.left: parent.horizontalCenter
            anchors.leftMargin :  bIsChange ? 60: 126
            source: "qrc:/digital/images/sigMode_close.png"
        }

        Text {
            id: sigModeState
            anchors.top: clickSigoff.top
            anchors.topMargin: clickSigoff.height*0.5
            anchors.left: clickSigoff.right
            anchors.leftMargin :  bIsChange ? 50: 116
            text: "单音模式 关"
            font.pixelSize: 40
            color: "white"
        }

        Text {
            id: sigPicState
            anchors.top: sigPic.top
            anchors.topMargin: sigPic.height*0.5
            anchors.left: sigModeState.left
            text: "单音发送 关"
            font.pixelSize: 40
            color: "white"
        }

        Image {
            id: sigPic
            width: bIsChange ? 208 : 374
            height: width
            z: 6
            anchors.top: clickSigoff.bottom
            anchors.topMargin: bIsChange ? 50 : 114
            anchors.horizontalCenter: clickSigoff.horizontalCenter
            source: "qrc:/digital/images/sigvoice_close.png"
        }

        Image {
            id: clickvoice
            width: bIsChange ? 434 : 500
            height: width
            z: 4
            anchors.top: bIsChange ? time.bottom : image.bottom
            anchors.topMargin: bIsChange ? 14 : 78
            anchors.left: parent.left
            anchors.leftMargin :  bIsChange ? 150 : 216
            source: getDigitalIcon()
        }
        Rectangle {
            id: sigalOff
            property bool bIspress: false
            width: bIsChange ? 270 : 336
            height: width*0.5
            radius: width/2
            anchors.top: image.bottom
            anchors.topMargin: bIsChange ? 50 : 114
            anchors.left: parent.horizontalCenter
            anchors.leftMargin :  bIsChange ? 60: 126

            MouseArea {
                id: buttonsigalOff
                anchors.fill: parent
                onPressed: {
                    sigalOff.bIspress = !sigalOff.bIspress
                    clickSigoff.source = getSigOffIcon()
                    if(true == sigalOff.bIspress)
                    {
                        PTT.setTestMode()
                        sigModeState.text = "单音模式 开"
                        buttonsigalpic.enabled = true;
                        button.enabled = false;
                    }
                    else{
                        sigModeState.text = "单音模式 关"
                        PTT.pttEnd()
                        PTT.setTestMode()
                        buttonsigalpic.enabled = false;
                        button.enabled = true;
                        sigalpicclick.bIspress = false
                        sigPic.source = getSigPicIcon();
                    }
                }
            }
        }

        Rectangle {
            id: sigalpicclick
            property bool bIspress: false
            width: bIsChange ? 200: 366
            radius: width/2
            height: width
            anchors.top: clickSigoff.bottom
            anchors.topMargin: bIsChange ? 58 : 122
            anchors.horizontalCenter: clickSigoff.horizontalCenter

            MouseArea {
                id: buttonsigalpic
                anchors.fill: parent
                enabled: false
                onPressed: {
                    sigalpicclick.bIspress = !sigalpicclick.bIspress
                    sigPic.source = getSigPicIcon();
                    if(true == sigalpicclick.bIspress)
                    {
                        PTT.pttStart();
                        sigPicState.text = "单音发送 开"
                    }
                    else{
                        PTT.pttEnd();
                        sigPicState.text = "单音发送 关"
                    }
                }
            }
        }

        Rectangle {
            id: qiangmai
            property bool bIspress: false
            width: bIsChange ? 334 : 400
            height: width
            radius: width/2
            anchors.top: bIsChange ? time.bottom : image.bottom
            anchors.topMargin: bIsChange ? 70 : 134
            anchors.left: parent.left
            anchors.leftMargin :  bIsChange ? 200 : 266
            MouseArea {
                id: button
                anchors.fill: parent
                enabled: true
                onPressed: {
                    qiangmai.bIspress = true
                    timecount.running = true
                    time.visible = true
                    clickvoice.source = getDigitalIcon()
                    PTT.pttStart()
                }
                onReleased: {
                    bIsFailure = false
                    tip.visible = false
                    qiangmai.bIspress = false
                    clickvoice.source = getDigitalIcon()
                    second = 0
                    minute = 0
                    hour = 0
                    time.text = ""
                    timecount.running = false
                    time.visible = false
                    PTT.pttEnd()
                }
            }
        }
        NumberAnimation {
            id: tipanimation
            target: tip
            property: "opacity"
            to: 0
            duration: 2000
            onStopped: {
                bIsFailure = false
                tip.visible = false
                tip.opacity = 1
            }
        }
    }
}
