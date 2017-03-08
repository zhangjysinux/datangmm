import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: personInfoMoreInfo
    property int selectenum : -1
    property string thename: ""
    property var myip: ""
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        personInfoMoreInfo.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personInfoMoreInfo.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            personInfoMoreInfo.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personInfoMoreInfo.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                personInfoMoreInfo.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }

    contentAreaItem: Item {
        anchors.fill: parent
        CEditTitleBar{
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            leftItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemBackgroundEnabled: true
            leftItemComponent: Component {
                id: leftTitleBar
                CButton {
                    anchors.fill: parent
                    iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                    backgroundEnabled: false
                    onClicked: {
                        console.log("back pre page")
                        pageStack.pop()
                    }
                }
            }
            titleText: qsTr("LTE")
            titlePixelSize: 30
            titleTextColor: "white"
            titleAreaCentered: true
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
            anchors.horizontalCenter: parent.horizontalCenter
            backgroundEnabled: true
            backgroundComponent: Component{
                Image {
                    anchors.fill: parent
                    source: "qrc:/main/images/top_line.png"
                }
            }
        }

        Item{
            id: mid
            width: parent.width
            anchors.top: titleBar.bottom
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
        Component {
            id: buttonAndLine
            Item {
                width: listview.width
                height: 89
                Text {
                    id: name
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    width: 140
                    height: 40
                    horizontalAlignment: Qt.AlignLeft
                    color: "#ffffff"
                    text: nameinfo
                    font.pixelSize: 40
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (selectenum == -1) {
                            selectenum = index
                            listmodel.set(selectenum,{"opacitynum":"0.3"})
                        } else {
                            listmodel.set(selectenum,{"opacitynum":"0"})
                            selectenum = index
                            listmodel.set(selectenum,{"opacitynum":"0.3"})
                        }
                    }
                    onReleased: {
                        if (listmodel.get(selectenum).nameinfo == "PTT") {
                            pageStack.push("qrc:/qml/PptChatRoom.qml",{"groupid":"","groupnum":"",
                                               "groupname":thename,"bIsPtt":true,
                                               "prepage":personInfoMoreInfo})

                        }
                    }
                    onClicked: {
                        qApp.openUrl("sinuxtel:makeVideoCall?phoneNum="+myip)
                    }
                }
                Rectangle {
                    id: rec
                    width: parent.width
                    height: parent.height
                    opacity: opacitynum
                }
                CLine {
                    id: line
                    anchors.top: name.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#36465a"
                }

            }
        }
        ListModel {
            id: listmodel
        }
        ListView {
            id: listview
            anchors.top: titleBar.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 100
            width: parent.width - 148
            anchors.bottom: parent.bottom
            clip: true
            model: listmodel
            delegate: buttonAndLine
            Component.onCompleted: {
                listmodel.append({"nameinfo":"PTT","opacitynum":"0"})
                listmodel.append({"nameinfo":"语音通话","opacitynum":"0"})
                listmodel.append({"nameinfo":"视频通话","opacitynum":"0"})
                listmodel.append({"nameinfo":"短信通话","opacitynum":"0"})
            }
        }
        Rectangle {
            id: sureback
            anchors.fill: parent
            visible: false
            color: "black"
            opacity: 0
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    sureback.visible = false
                    sureback.opacity = 0
                    suredialog.visible = false
                }
            }
        }
        Rectangle {
            id: suredialog
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
                anchors.topMargin: 52
                height: 80
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: 40
                text: qsTr("是否进入400MHZ-直通脱网对讲模式?")
            }
            CButton {
                id: surecancel
                anchors.top: message.bottom
                anchors.topMargin: 76
                anchors.left: parent.left
                anchors.leftMargin: 48
                width: 174
                height: 100
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/bluetooth/images/bluetooth_ccancel.png" :
                                      "qrc:/bluetooth/images/bluetooth_cancel.png"
                onClicked: {
                    sureback.visible = false
                    suredialog.visible = false
                    sureback.opacity = 0
                    if (listmodel.get(selectenum).nameinfo == "PTT") {
                        pageStack.push("qrc:/qml/DigitalTalk.qml",{bIs400MHZ:false,personname:thename})
                    }
                }
            }
            CButton {
                id: surebutton
                anchors.top: message.bottom
                anchors.topMargin: 76
                anchors.left: surecancel.right
                anchors.leftMargin: 48
                width: 174
                height: 100
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/history/images/sureclick.png" :
                                      "qrc:/history/images/suredelete.png"
                onClicked: {
                    sureback.visible = false
                    suredialog.visible = false
                    sureback.opacity = 0
                    if (listmodel.get(selectenum).nameinfo == "PTT") {
                        pageStack.push("qrc:/qml/DigitalTalk.qml",{bIs400MHZ:true})
                    }
                }
            }
        }
    }
}
