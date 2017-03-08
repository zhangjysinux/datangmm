import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    id: mainroot
    property int myCallID: 0
    property int isCaller: 0
    pagePopGestureEnabled: false
    applicationQuitGestureEnabled: false
    signal handlerCountChanged(var count)
    function callInComing(value,remoteIpAddr)
    {
        pageStack.push("qrc:/qml/MyCall.qml", {"handId":value,"remoteIpAddr":remoteIpAddr},true)
        csDbus.setDisplayState(true)
    }

    function makeCall(net,value)
    {
        console.log("cccccccccccc")
        if (csDbus.isHandlerExist(value))
            return
        if (csDbus.getQmlState() == 2)
            return
        if (csDbus.getLocalNum(net) == value)
        {
            return;
        }
        var ret = csDbus.dial(net,value)
        if (ret == -1)
        {
            console.log(ret)
            return
        }

        if (csDbus.getIsTalking())
            pageStack.push(pageStack.currentPage,{"numtemp":value,"netFlag":net,"bt1":false},true)
        else
            pageStack.push("qrc:/qml/Talking.qml",{"numtemp":value,"netFlag":net,"bt1":false},true)
        csDbus.setIsTalking(true)
        console.log("cccccccccccc3")

    }
    function startMake(net,phoneNum)
    {
        if (!csDbus.isHostMachine()) {
            if (csDbus.getIsTalking())
                pageStack.push(pageStack.currentPage,{"numtemp":phoneNum,"netFlag":net,"bt1":false},true)
            else
                pageStack.push("qrc:/qml/Talking.qml",{"numtemp":phoneNum,"netFlag":net,"bt1":false},true)
        }

    }
    function startVideoIn(callId , phoneNum)
    {
        csDbus.initContacterData(phoneNum)
        csDbus.setDisplayState(true)
        pageStack.push("qrc:/qml/VideoIn.qml", {"callId":callId,"phoneNum":phoneNum},true)
    }
    function startVideoCall(net, phoneNum)
    {
        if (csDbus.isHandlerExist(phoneNum))
            return
        if (csDbus.isVideoExit())
            return
        csDbus.initContacterData(phoneNum)
        var callId = csDbus.makeVideo(net, phoneNum)
        pageStack.push("qrc:/qml/VideoCall.qml", {"callId":callId,"phoneNum":phoneNum,"network":net},true)
    }

    Connections {
        target: csDbus
        onSignalMakeStart: {
            startMake(net,phoneNum)
        }
    }
    Connections {
        target: csDbus
        onSignalReturnPage: {
            console.log("onSignalReturnPage")
            pageStack.pop()
        }
    }

    function entryPhoneBook()
    {
        globalApp.openUrl("sinuxbook:")
    }

    Connections {
        target: csDbus
        onSignalAlreadyAnswer: {
            //pageStack.push("qrc:/qml/Talking.qml")
            csDbus.setIsTalking(true)
        }
        onSignalVideoAlreadyAnswer: {
            pageStack.push("qrc:/qml/VideoCall.qml")
            csDbus.setIsTalking(true)
        }
    }

    Connections {
        target: globalApp
        onAboutToQuit:{
        }
    }
    respondOnBackKeyReleased: false
    Connections {
        target: mainroot
        onBackKeyReleased: {
//            pageStack.pop(); //如果显示的是CPage页，则调用，否则不需要调用
//            gAppUtils.appQuitRequest();
//            mainroot.appQuit()
//            Qt.quit();
        }
    }

    Connections {
        target: csDbus
        onSignalQuitApp: {
            mainroot.appQuit()
//            globalApp.appQuitRequest();
            Qt.quit();
            csDbus.setIsTalking(false)
        }
    }
    Connections {
        target: csDbus
        onSignalScreen_on: {
            //csDbus.myDebug("gScreenInfo.setWindowProperty")
        }
    }

    ListModel {
        id: handlerList
    }

    initialPage:CPage{
        id: main
        Component.onCompleted: {
//            gScreenInfo.setWindowProperty("CATEGORY", "dialing"); //设置锁屏状态下直接弹出
            gScreenInfo.setWindowProperty("WM_NOT_LOCK", "true"); //设置通话过程中不锁屏
        }

        contentAreaItem:  Item {
            Item {
                id: mid
                anchors.fill: parent
                Image {
                    anchors.fill: parent
                    source: "qrc:/main/images/back_mid.png"
                }
                Image {
                    sourceSize.height: 4
                    sourceSize.width: 4
                    fillMode: Image.Tile
                    anchors.fill: parent
                    source: "qrc:/main/images/back_mid_up.png"
                }
            }
            TextInput {
                id: ipText
                width: parent.width
                height: 90
                font.pixelSize: 40
                horizontalAlignment: Qt.AlignHCenter
                color: "lightgreen"
                anchors.bottom: parent.bottom
                visible: ipText.text.length > 0 ? true : false
            }

            Row {
                anchors.top: parent.top
                anchors.topMargin: 150
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 50
                CButton {
                    visible: false
                    width: 300
                    height: 200
                    text: "VOIP"
                    textColor: "#ffffff"
                    pixelSize: 30
                    onClicked: {
                        csDbus.dial(2,ipText.text)
                        pageStack.push("qrc:/qml/Talking.qml", {"netFlag":2})
                    }
                }
                CButton {
                    visible: false
                    width: 300
                    height: 200
                    text: "CS"
                    textColor: "#ffffff"
                    pixelSize: 30
                    onClicked: {
                        csDbus.dial(256,ipText.text)
                        pageStack.push("qrc:/qml/Talking.qml")
                    }
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (ipText.focus == true)
                    closeSoftKeyboard();
                else
                    ipText.focus = true
            }
        }
        Rectangle {
            id: bluedialog
            property alias dilogText: message.text
            visible: false
            anchors.centerIn: parent
            width: 492
            height: 386
            Image {
                anchors.fill: parent
                source: "qrc:/bluetooth/images/bluetoothblack.png"
            }
            Text {
                id: message
                anchors.top: parent.top
                anchors.topMargin: 24
                height: 30
                width: parent.width
                wrapMode: Text.Wrap
                lineHeight: 2
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: 30
                text: "拨号失败"
            }
            CButton {
                id: deletecancel
                anchors.top: message.bottom
                anchors.topMargin: 24
                anchors.left: parent.left
                anchors.leftMargin: 48
                width: 174
                height: 100
                visible: true
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/bluetooth/images/bluetooth_ccancel.png" :
                                      "qrc:/bluetooth/images/bluetooth_cancel.png"
                onClicked: {
                    bluedialog.visible = false
                    mainroot.appQuit()
                    Qt.quit();
                }
            }
            CButton {
                id: deletesure
                anchors.top: message.bottom
                anchors.topMargin: 76
                anchors.left: parent.left
                anchors.leftMargin: 48
                width: 174
                height: 100
                visible: true
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/history/images/sureclick.png" : "qrc:/bluetooth/images/bluetooth_copen.png"
                onClicked: {
                    bluedialog.visible = false
                    mainroot.appQuit()
                    Qt.quit();
                }
            }
        }

    }
}

