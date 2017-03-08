import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: talking
    property int netFlag: 0
    property string handId: ""
    property int qmlState: 0
    property alias numtemp: name1.text
    property alias bt1: button1.enabled
    property bool bIsChange: false
    property var phoneNum: ""
    property var network: ""

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        console.log("make new talking transform")
        setStatus()
        talking.color = "black"
        if (mainroot.getScreen() == 2) {
            bIsChange = true
            //talking.statusBarHoldEnabled = false
            //gScreenInfo.setStatusBar(false);
            bottomback.height = 328
            row1.anchors.topMargin = 0;
            button4.anchors.topMargin = 0;
            photo.anchors.topMargin = 18
            photo.width = 245
            photo.height = 245
            time.anchors.top = netText1.bottom
            time.anchors.bottom = netText1.bottom
            time.anchors.bottomMargin = -80
            time.anchors.horizontalCenter = parent.horizontalCenter
            time.anchors.horizontalCenterOffset = parent.width / 2 - 70 + 16

        } else {
            bIsChange = false
            //talking.statusBarHoldEnabled = true
            //gScreenInfo.setStatusBar(true);
            bottomback.height = 432
            row1.anchors.topMargin = 48;
            button4.anchors.topMargin = 48;
            photo.anchors.topMargin = 48
            photo.width = 320
            photo.height = 320

            time.anchors.top =  nametitle.bottom
            time.anchors.bottom = bottomback.top
            time.anchors.horizontalCenter = parent.horizontalCenter
            time.anchors.horizontalCenterOffset = 0
        }
    }
    Connections{
        target: mainroot
        onItemChanged: {
            if (ret == 2) {
                bIsChange = true
                //talking.statusBarHoldEnabled = false
                //gScreenInfo.setStatusBar(false);
                bottomback.height = 328
                row1.anchors.topMargin = 0;
                button4.anchors.topMargin = 0;
                photo.anchors.topMargin = 18
                photo.width = 245
                photo.height = 245

                time.anchors.top = netText1.bottom
                time.anchors.bottom = netText1.bottom
                time.anchors.bottomMargin = -80
                time.anchors.horizontalCenter = parent.horizontalCenter
                time.anchors.horizontalCenterOffset = parent.width / 2 - time.width + 16


            } else {
                bIsChange = false
                //talking.statusBarHoldEnabled = true
                //gScreenInfo.setStatusBar(true);
                bottomback.height = 432
                row1.anchors.topMargin = 48;
                button4.anchors.topMargin = 48;
                photo.anchors.topMargin = 48
                photo.width = 320
                photo.height = 320

                time.anchors.top =  nametitle.bottom
                time.anchors.bottom = bottomback.top
                time.anchors.horizontalCenter = parent.horizontalCenter
                time.anchors.horizontalCenterOffset = 0

            }
        }
    }

    ListModel {
        id: myHandlerList
    }
    Connections {
        target: csDbus
        onSignalVidChanged: {
            if (!isVideoHold)
                pageStack.pop()
        }
        onVideoHangup: {
            pageStack.pop()
        }
    }

    Connections {
        target: csDbus
        onSignalUpdateTimer: {
            timetext.text = timeString
        }
    }

    function setStatus()
    {

//        name1.text = viewModel.getContacterData("name")
//        photo.iconSource = viewModel.getContacterData("photo") == "" ? "qrc:/main/images/init_photo.png" : viewModel.getContacterData("photo")

        console.log(name1.text,csDbus.getContacterData("name"))
//        if (myHandlerList.get(0).isHandsfree == true){
//            button3.iconSource = "qrc:/main/images/3-2.png"
//            loader.anchors.right = row1.right
//            loader.anchors.top = row1.top
//            loader.visible = true
//        }else{
//            loader.visible = false
//            button3.iconSource = button3.pressed ? "qrc:/main/images/3-1.png" : "qrc:/main/images/3.png"
//        }

//        if (myHandlerList.get(0).isMuted == true){
//            button2.iconSource = "qrc:/main/images/2-2.png"
//            loader2.anchors.right = row1.right
//            loader2.anchors.rightMargin = 220
//            loader2.anchors.top = row1.top
//            loader2.visible = true
//        }else{
//            loader2.visible = false
//            button2.iconSource = button2.pressed ? "qrc:/main/images/2-1.png" : "qrc:/main/images/2.png"
//        }

//        if (myHandlerList.get(0).isRecord == true){
//            button6.iconSource = "qrc:/main/images/6-2.png"
//            loader6.x = button6.x
//            loader6.y = button6.y
//            loader6.visible = true
//        }else{
//            loader6.visible = false
//            button6.iconSource = button6.pressed ? "qrc:/main/images/6-1.png" : "qrc:/main/images/6.png"
//        }
    }

    function findNetText()
    {
        var myNetText;
        if (myHandlerList.get(0).network == 1)
            myNetText = "自组网"
        else if (myHandlerList.get(0).network == 16)
            myNetText = "LTE"
        else if (myHandlerList.get(0).network == 256)
            myNetText = "天通网"
        netText.text = myNetText
    }

    function findNet(myNetText)
    {
        var net;
        if (myNetText == "自组网")
            net = 1
        else if (myNetText == "LTE")
            net = 16
        else if (myNetText == "天通网")
            net = 256
        else if (myNetText == "WIFI")
            net = 1048576
        return net;
    }

    contentAreaItem: Item {
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
            width: 320
            height: 320
            iconSource: csDbus.getContacterData("photo") == "" ? "qrc:/main/images/init_photo.png" : csDbus.getContacterData("photo")
            anchors.top: parent.top
            anchors.topMargin: 48
            anchors.horizontalCenter: parent.horizontalCenter
            maskComponent: Rectangle{
                id: msk
                parent: photo
                radius: photo.width  / 2
                anchors.fill: parent
                smooth: true
                visible: false
            }
        }

        Item {
            id: nametitle
            anchors.top: photo.bottom
            anchors.topMargin: 16
            width: net.width + netIcon.width + netText.width
            anchors.horizontalCenter: parent.horizontalCenter
            visible: true
            height: name.height + 24 + net.height
            property alias name: name1.text
            //property alias net: netText.text
            Text {
                id: name1
                height: 40
                text: csDbus.getContacterData("name")
                font.pixelSize: 40;color: "#ffffff"
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Qt.AlignHCenter
                width: 400
                elide: Text.ElideRight

            }
            Text {
                id: net
                height: 40
                text: qsTr("对方")
                anchors.top: name1.bottom
                anchors.topMargin: 16
                font.pixelSize: 40;color: "#c9c9c9"
                //anchors.horizontalCenter: parent.horizontalCenter
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
                height: 40
                text: "LTE"
                anchors.top: net.top
                anchors.left: netIcon.right
                anchors.leftMargin: 8
                font.pixelSize: 40;color: "#c9c9c9"
                //anchors.horizontalCenter: parent.horizontalCenter
                onTextChanged: {
                    netText1.text = netText.text
                }
            }
            Component.onCompleted: {
                if (csDbus.getContacterData("name") == "")
                    name1.text = phoneNum
            }
        }
        Image {
            id: netIcon1
            source: "qrc:/main/images/net-on.png"
            anchors.top: parent.top
            anchors.topMargin: 16
            anchors.right: netText1.left
            anchors.rightMargin: 8
        }
        Text {
            id: netText1
            height: 40
            //width: 100
            text: ""
            anchors.top: parent.top
            anchors.topMargin: 16
            anchors.right: parent.right
            anchors.rightMargin: 16
            font.pixelSize: 40;color: "#c9c9c9"
            //anchors.horizontalCenter: parent.horizontalCenter
        }
        Item {
            id: time
            anchors.top: nametitle.bottom
            anchors.bottom: bottomback.top
            width: timetext.width
            anchors.horizontalCenter: parent.horizontalCenter
            property alias text: timetext.text
            Text {
                id: timetext
                height: 30
                text: ""
                font.pixelSize: 30
                color: "#ffffff"
                anchors.centerIn: parent
                horizontalAlignment: Qt.AlignRight
                Timer {
                    id: timer
                    repeat: true
                    running: true
                    interval: 1000
                    onTriggered: {
                        timetext.text = csDbus.getTimeString()
                    }
                }
            }
        }

        Image {
            id: bottomback
            anchors.bottom: parent.bottom
            height: 432
            width: parent.width
            source: "qrc:/main/images/bottom-back.png"
            //visible: false
        }

        Row {
            id: row1
            height: 100
            width: 660
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: bottomback.top
            anchors.topMargin: 48
            CButton {
                id: button1
                height: 100;width: 220
                iconSource: pressed ? "qrc:/video/images/video/2-2.png" : "qrc:/video/images/video/2-1.png"
                backgroundEnabled: false
                onClicked: {
                    csDbus.setVidEnabledChanged(network, callId, false)
                    //csDbus.setVidInputChanged(network, handId)
                }
            }
            CButton {
                id: button2
                height: 100;width: 220
                iconSource: pressed? "qrc:/main/images/2-1.png" : "qrc:/main/images/2.png"
                backgroundEnabled: false
                onClicked: {
                    csDbus.setMuteMicrophone(network,!csDbus.getIsMicrophoneMuted(network))
                }
            }
            CButton {
                id: button3
                height: 100;width: 220
                iconSource: pressed ? "qrc:/main/images/3-1.png" : "qrc:/main/images/3.png"
                backgroundEnabled: false
                onClicked: {
                    var value = csDbus.getAudioMode(findNet(network));       //免提"ihf"表示免提， "earpiece"表示听筒
                    if (value == "ihf")
                        value = "earpiece"
                    else if (value == "earpiece")
                        value = "ihf"
                    csDbus.setAudioMode(network,value)
                }
            }
        }

        CButton {
            id: button4
            height: 100;width: 220
            iconSource: button4.pressed ? "qrc:/main/images/4-1.png" : "qrc:/main/images/4.png"
            //iconSource: pressed ? "qrc:/video/images/video/3-2.png" : "qrc:/video/images/video/3-1.png"
            backgroundEnabled: false
            anchors.top: row1.bottom
            anchors.topMargin: 48
            anchors.left: row1.left
            onClicked: {
                csDbus.hold(handler1, true)

                //切换
//                var temp = csDbus.getGetAudOrVideo(network, handId)
//                if (temp == 0)
//                    csDbus.setVidEnabledChanged(network, handId, false)
//                else if (temp == 1)
//                    csDbus.setVidEnabledChanged(network, handId, true)
            }
        }
        CButton {
            id: button5
            height: 100;width: 220
            iconSource: pressed? "qrc:/main/images/5-1.png" : "qrc:/main/images/5.png"
            backgroundEnabled: false
            anchors.left: button4.right
            anchors.top: button4.top
            onClicked: {
                loaderr.visible = true
            }
        }
        CButton {
            id: button6
            height: 100;width: 220
            iconSource: pressed? "qrc:/main/images/6-1.png" : "qrc:/main/images/6.png"
            backgroundEnabled: false
            anchors.left: button5.right
            anchors.top: button4.top
            onClicked: {
                csDbus.record(true)
            }
        }


        CButton {
            id: hangUp
            backgroundEnabled: false
            width: 300; height: 100
            iconSource: pressed ? "qrc:/main/images/hangup-click.png" : "qrc:/main/images/hangup.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: button6.bottom
            anchors.topMargin: 18
            onClicked: {
                csDbus.hangupVideo()
            }
        }

        Loader {
            id: loader
            z: 3
            sourceComponent: buttonBack
            width: 220
            height: 100
            visible: false
        }
        Loader {
            id: loader2
            z: 3
            sourceComponent: buttonBack
            width: 220
            height: 100
            visible: false
        }
        Loader {
            id: loader6
            z: 3
            sourceComponent: buttonBack
            width: 220
            height: 100
            visible: false
        }

        Component {
            id: buttonBack
            Rectangle {
                color: Qt.rgba(0,0,0,0)
                AnimatedImage {
                    anchors.fill: parent
                    source: "qrc:/main/images/gif.gif"
                }
            }
        }

        ListModel{
            id: listModel
        }
        Loader {
            id: loaderr
            z: 4
            visible: false
            anchors.bottom: parent.bottom
            sourceComponent: keyboard
            Component.onCompleted: {
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/1-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/1.png","value":"1"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/2-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/2.png","value":"2"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/3-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/3.png","value":"3"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/4-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/4.png","value":"4"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/5-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/5.png","value":"5"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/6-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/6.png","value":"6"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/7-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/7.png","value":"7"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/8-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/8.png","value":"8"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/9-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/9.png","value":"9"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/xing_click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/xing.png","value":"*"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/0-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/0.png","value":"0"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/jing-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/jing.png","value":"#"})
            }
        }

        Component {
            id:keyboard
            Item {
                id: backitem
                width: bIsChange ? 1280 : 720
                height: bIsChange ? 400 : 631
                property alias recta: rect
                Image {
                    id: background
                    source: "qrc:/keyboard/images/keyboard/num_back.png"
                    width: backitem.width
                    height: backitem.height
                    MouseArea {
                        anchors.fill: parent
                    }
                    Connections {
                        target:gScreenInfo
                        onCurrentOrientationChanged: {
                            if (inputText.text != "") {
                                if (bIsChange) {
                                    backitem.height = 492
                                    //line.anchors.topMargin = 145
                                    inputText.visible = true
                                    key.anchors.top = inputText.bottom
                                } else {
                                    backitem.height = 718
                                    //line.anchors.topMargin = 186
                                }
                                key.anchors.top = inputText.bottom
                            } else {
                                if (bIsChange) {
                                    backitem.height = 400
                                    //line.anchors.topMargin = 145
                                    inputText.visible = true
                                    key.anchors.top = backitem.top
                                } else {
                                    backitem.height = 631
                                    mousearea.visible = false
                                    inputLine.visible = false
                                    inputText.visible = false
                                    key.anchors.top = backitem.top
                                    //line.anchors.top = background.top
                                    //line.anchors.topMargin = 100
                                }
                            }
                        }
                    }
                    Text {
                        id: inputText
                        anchors.left: parent.left
                        anchors.leftMargin: 48
                        width: 562
                        height: 86
                        clip: true
                        font.pixelSize: 50
                        visible: false
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        color: "black"
                        text: ""
                        onTextChanged: {
                            inputText.visible = true
                            mousearea.visible = true
                            inputLine.visible = true
                        }
                    }
                    MouseArea {
                        id: mousearea
                        anchors.left: inputText.right
                        anchors.leftMargin: 19
                        width: 61
                        height: 86
                        visible: false
                        onClicked: {
                            inputText.text = inputText.text.substring(0,inputText.text.length - 1)
                            if (inputText.text == "") {
                                inputText.visible = false
                                mousearea.visible = false
                                inputLine.visible = false
                                backitem.height = bIsChange ? 400 : 631
                                key.anchors.top = parent.top
                                line.anchors.top = parent.top
                                line.anchors.topMargin = 100
                            }
                        }
                        onPressAndHold: {
                            inputText.text = ""
                            inputText.visible = false
                            mousearea.visible = false
                            inputLine.visible = false
                            backitem.height = bIsChange ? 400 : 631
                            key.anchors.top = parent.top
                            line.anchors.top = parent.top
                            line.anchors.topMargin = 100
                        }
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/keyboard/images/keyboard/input_delete.png"
                        }
                    }
                    CLine {
                        id: inputLine
                        anchors.top: inputText.bottom
                        width: 720
                        color: "#121212"
                        visible: false
                    }
                    Column {
                        id: line
                        anchors.top: parent.top
                        anchors.topMargin: 100
                        spacing: 100
                        Repeater {
                            model: 3
                            delegate: twoLine
                        }
                    }
                    Grid {
                        id: key
                        columns: 3
                        rows: 4
                        spacing: 2
                        Repeater {
                            model: listModel
                            CButton {
                                id: num
                                width: 240
                                height: 100
                                backgroundEnabled: false
                                iconSource: pressed == true ? pressadr : numadr
                                onClicked: {
                                    inputText.text += listModel.get(index).value
                                    backitem.height = bIsChange ? 492 : 718
                                    key.anchors.top = inputText.bottom
                                    line.anchors.topMargin = 186
                                }
                            }
                        }
                    }
                    Item {
                        id: rect
                        width: 300
                        height: 185
                        anchors.top: bIsChange ? parent.top : key.bottom
                        anchors.topMargin: bIsChange ? 150 : 20
                        anchors.right: parent.right
                        anchors.rightMargin: bIsChange ? 130 : 210
                        anchors.bottom: parent.bottom

                        Image {
                            id: numkey
                            anchors.top: rect.top
                            width: 70
                            height: 70
                            source: "qrc:/keyboard/images/keyboard/numkey.png"
                            anchors.horizontalCenter: parent.horizontalCenter
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    loaderr.visible = false
                                }
                            }
                        }
                        Image {
                            id: call
                            anchors.top: numkey.bottom
                            anchors.topMargin: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 300
                            height: 99
                            source: "qrc:/main/images/hangup.png"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    csDbus.hangup(handId)

                                }
                                onPressed: {
                                    call.source = "qrc:/main/images/hangup-click.png"
                                }
                                onReleased: {
                                    call.source = "qrc:/main/images/hangup.png"
                                }
                            }
                        }
                    }
                }
            }
        }
        Component {
            id: twoLine
            Item {
                width: 720
                height: 2
                CLine {
                    id: line1
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    width: parent.width - 60
                    color: "#cbcbcb"
                }
                CLine {
                    id: line2
                    anchors.top: line1.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    width: parent.width - 60
                    color: "#121212"
                }
            }
        }
    }
}

