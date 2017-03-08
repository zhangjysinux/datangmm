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

    property string groupid: "0"
    property string groupname: "红色尖兵"
    property string groupnum: "15"
    property string netinfo: ""
    property var prepage: null

    Connections {
        target: getData
        onSignalPttGettedChanged: {
            console.log("onSignalPttGettedChanged",isGetted)
            if (isGetted){

            }
        }
    }

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
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        diagital.color = "#293443"
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
//        Item{
//            id: mid
//            width: parent.width
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            Image {
//                fillMode: Image.Tile
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid_up.png"
//            }
//            Image {
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid.png"
//            }
//        }
        CButton {
            id: close
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 20
            iconSource: "qrc:/main/images/back.png"
            backgroundEnabled: false
            onClicked: {
                pageStack.pop()
            }
        }
        Text {
            id: name
            anchors.top: parent.top
            anchors.topMargin: 32
            font.pixelSize: 30
            text: qsTr("400MHZ-直通脱网对讲")
            color: "#29afe2"
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !bIsGroup
        }

        Rectangle {
            width: 244
            height: 50
            anchors.top: parent.top
            anchors.topMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter
            visible: bIsGroup
            border.color: "#29afe2"
            border.width: 2
            radius: 10
            color: Qt.rgba(0,0,0,0)
            CButton {
                id: button1
                width: 122
                height: 50
                text: "对讲"
                textColor: "#ffffff"
                backgroundEnabled: true
                backgroundComponent: Component {
                    Rectangle {
                        id: titlerect
                        anchors.fill: parent
                        radius: 10
                        color: "#29afe2"
                    }
                }
                onClicked: {

                }
            }
            CButton {
                id: button2
                anchors.left: button1.right
                width: 122
                height: 50
                text: "文本"
                textColor: "#ffffff"
                backgroundEnabled: true
                backgroundComponent: Component {
                    Rectangle {
                        id: titlerect
                        anchors.fill: parent
                        radius: 10
                        color: "#29afe2"
                    }
                }
                onClicked: {
                    if (isFromText)
                        pageStack.pop()
                    else
                        pageStack.push("qrc:/qml/DigitalGroup.qml","isFromTalk",true)
                }
            }
        }

//        Item{
//            id: person
//            width: 672
//            height: 196
//            visible: bIsChange ? false : true
//            anchors.top: name.bottom
//            anchors.topMargin: 50
//            anchors.horizontalCenter: parent.horizontalCenter
//            Image {
//                anchors.fill: parent
//                source: "qrc:/digital/images/digital-1.png"
//            }
//            Image {
//                anchors.fill: parent
//                source: "qrc:/digital/images/digital-2.png"
//            }
//        }
        Loader {
            id: addPhoto
            width: 180
            height: 180
            source: "qrc:/qml/Photo.qml"
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.horizontalCenter: parent.horizontalCenter
            onLoaded: addPhoto.item.myIconSource = "qrc:/main/images/init_photo.jpg"
        }

        Timer {
            id: timecount
            interval: 1000
            //running: true
            repeat: true
            onTriggered: {
                var hourstring,minutestring,secondstring
                second+=1
                if (second == 10) {
                    tip.visible = true
                    bIsFailure = true
                    tipanimation.start()
                } else if(second == 1) {
                    tip.visible = true
                    bIsFailure = false
                    tipanimation.start()
                }

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
            anchors.top: time.bottom//bIsChange ? image.top : image.bottom
            anchors.topMargin: 28
//            anchors.left: parent.left
//            anchors.leftMargin: bIsChange ? 540 : 260
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            height: 80
            visible: false
            color: "#2e3b4c"
            opacity: 1
            Text {
                anchors.centerIn: parent
                text: bIsFailure ? qsTr("抢麦失败") : qsTr("抢麦成功")
                color: "white"
                font.pixelSize: 40
            }
        }
        Text {
            id: time
            anchors.top: addPhoto.bottom //bIsChange ? image.bottom : person.bottom
            anchors.topMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter
            text: ""
            font.pixelSize: 40
            color: "green"
        }
//        Image {
//            id: image
//            anchors.top: bIsChange ? name.bottom : time.bottom
//            anchors.topMargin: bIsChange ? 24 : 16
//            width: bIsChange ? 1216 : 663
//            height: 70
//            anchors.horizontalCenter: parent.horizontalCenter
//            source: bIsChange ? "qrc:/digital/images/h-digital-voice.png" : "qrc:/digital/images/digital-6.png"
//        }

//        Image {
//            id: clickvoice
//            width: bIsChange ? 434 : 500
//            height: width
//            z: 4
//            anchors.top: bIsChange ? time.bottom : image.bottom
//            anchors.topMargin: bIsChange ? 14 : 78
//            anchors.horizontalCenter: parent.horizontalCenter
//            source: getDigitalIcon()
//        }
        Rectangle {
            id: qiangmai
            property bool bIspress: false
            width: 220//bIsChange ? 334 : 400
            height: width
            radius: width/2
            anchors.bottom: parent.bottom//bIsChange ? time.bottom : image.bottom
            anchors.bottomMargin: 54
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#2e3b4c"
            Image {
                id: buttonimage
                width: 69 //bIsChange ? 434 : 500
                height: 99
                anchors.centerIn: qiangmai
    //            anchors.top: parent.bottom //bIsChange ? time.bottom : image.bottom
    //            anchors.topMargin: 54//bIsChange ? 16 : 80
    //            anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/main/images/icon_microphone.png" //bIsChange ? "qrc:/digital/images/h-digital-back.png" : "qrc:/digital/images/digital-5.png"
            }
            MouseArea {
                id: button
                anchors.fill: parent
                onPressed: {
                    qiangmai.bIspress = true
                    timecount.running = true
                    time.visible = true
                    //clickvoice.source = getDigitalIcon()
                    getData.pttSet(true)
                    buttonimage.source = "qrc:/main/images/icon_microphone_s.png"
                }
                onReleased: {
                    bIsFailure = false
                    tip.visible = false
                    qiangmai.bIspress = false
                    //clickvoice.source = getDigitalIcon()
                    second = 0
                    minute = 0
                    hour = 0
                    time.text = ""
                    timecount.running = false
                    time.visible = false
                    buttonimage.source = "qrc:/main/images/icon_microphone.png"
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
