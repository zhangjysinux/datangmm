import QtQuick 2.0
import com.syberos.basewidgets 2.0
CPage {
    id: myCall
    property alias text: mytext.text
    property int myCallId: 0
    property string handId: "0"
    property int netFlag: 0
    property string remoteIpAddr: ""
    property var myList: null

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        csDbus.getNamebyDialnum(remoteIpAddr)
        myCall.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            myCall.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            photo.anchors.topMargin = 24
        } else {
            myCall.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            photo.anchors.topMargin = 208
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                myCall.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                photo.anchors.topMargin = 24
            } else {
                myCall.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                photo.anchors.topMargin = 208
            }
        }
    }

    Connections {
        target: csDbus
        onSignalUpdateContacterInfo: {
            if (photoPath != "")
                photo.iconSource = photoPath
            else
                photo.iconSource = "qrc:/main/images/init_photo.jpg"
            if (name != "")
            {
                name1.text = name
                phoneNum.text = dialnum
            }
            else
                name1.text = dialnum

        }
    }

    contentAreaItem: Item {
        Text {
            id: mytext
            anchors.top: name1.bottom
            anchors.topMargin: 24
            anchors.horizontalCenter: parent.horizontalCenter
            text: handId
            font.pixelSize: 40
            color: "#ffffff"
        }

//        Item {
//            id: mid
//            anchors.fill: parent
//            Image {
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid.png"
//            }
//            Image {
//                fillMode: Image.Tile
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid_up.png"
//            }
//        }

        CMaskImage {
            id: photo
            width: 220
            height: 220
            iconSource: "qrc:/main/images/init_photo.jpg"
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
        }
        Text {
            id: name1
            height: 70
            text: remoteIpAddr
            //text: qsTr("李军")
            font.pixelSize: 70;color: "#29afe2"
            anchors.top: photo.bottom
            anchors.topMargin: 48;
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            id: phoneNum
            height: 70
            //text: remoteIpAddr
            font.pixelSize: 70;color: "#29afe2"
            anchors.top: name1.bottom
            anchors.topMargin: 48;
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: buttn
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 80
//            anchors.left: parent.left
//            anchors.leftMargin: 160
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 100
            CButton {
                width: 152
                height: 152
                iconSource: "qrc:/main/images/hangup.png"
                backgroundEnabled: false
                onClicked: {
                    csDbus.hangup(handId)
                    //pageStack.pop()

                    //globalApp.quit()
                }
            }
            CButton {
                width: 152
                height: 152
                iconSource: "qrc:/main/images/answer.png"
                backgroundEnabled: false
                onClicked: {
                    csDbus.answer(handId)
                    pageStack.replace("qrc:/qml/Talking.qml",{"numtemp":phoneNum,"bt1":false},true) //"netFlag":net,
                }
            }

        }
    }
}


