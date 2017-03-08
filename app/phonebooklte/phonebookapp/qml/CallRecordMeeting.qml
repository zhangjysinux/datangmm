import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: callRecordMeeting
    property string meetingPhone: "qrc:/history/images/meeting.png"
    property bool bIsDoubleClick: false
    property bool bIsFirst: true/*是否第一次*/
    property var myIndex: 0
    property var historyId: ""

    orientationPolicy: CPageOrientation.Automatic
    onStateChanged: {
        if (status == CPageStatus.WillShow)
        {
            historyTable.getMeetingTime(historyId)
            memberModel.clear()
        }
    }
    Connections {
        target: historyTable
        onSignalMeetingMember: {
            memberModel.append({"id":id})
        }
        onSignalMeetingStart: {
            listmodel.set(2,{"value":startTime})
        }
        onSignalMeetingEnd: {
            listmodel.set(3,{"value":endTime})
        }
    }

    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            callRecordMeeting.orientationPolicy = CPageOrientation.LockLandscape
        callRecordMeeting.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            callRecordMeeting.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            callRecordMeeting.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                callRecordMeeting.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                callRecordMeeting.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }
    contentAreaItem: Item {
        CTitleBar {
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            rightItemEnabled: true
            rightItemSize: Qt.size(74,74)
            rightItemBackgroundEnabled: true
            rightItemComponent: CButton {
                id: rightTitle
                property alias enabled: rightTitle.enabled
                anchors.fill: parent
                pixelSize: 30
                textColor: "white"
                backgroundEnabled: false
                text: qsTr("清空")
                iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                spacing: -68
                onClicked: {
                    if (listmodel.count > 1) {
                        sureback.visible = true
                        sureback.opacity = 0.7
                        suredialog.visible = true
                    }
                }
            }
            leftItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemBackgroundEnabled: true
            leftItemComponent: CButton {
                id: leftTitle
                anchors.fill: parent
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                onClicked: {
                    pageStack.pop()
                }
            }
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
            backgroundEnabled: true
            backgroundComponent: Component{
                Image {
                    anchors.fill: parent
                    source: "qrc:/main/images/top_line.png"
                }
            }
        }
        Item {
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
        Loader {
            id: addPhoto
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            anchors.left: parent.left
            anchors.leftMargin: 48
            width: 150
            height: 150
            source: "qrc:/qml/Photo.qml"
            onLoaded: {
                if (getData.getLocalPhoto() != "" && getData.getLocalPhoto() != "null")
                    addPhoto.item.myIconSource = getData.getLocalPhoto()
            }

        }
        Text {
            id: myName
            anchors.top: addPhoto.top
            anchors.topMargin: 55
            anchors.left: addPhoto.right
            anchors.leftMargin: 48
            width: 400
            height: 40
            text: getData.getLocalPersonName()//"personName"
            color: "white"
            font.pixelSize: 40
            elide: Text.ElideRight
        }
        Component {
            id: myhistory
            Item {
                id: lineHistory
                width: listview.width
                height: 132

                Text {
                    id: lineHead
                    color: "#b6b6b6"
                    text: textTitle
                    font.pixelSize: 30
                    height: 30
                    anchors.top: parent.top
//                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    onTextChanged: {
                        if (lineHead.text == "参与成员")
                        {
                            lineValue.visible = false
                            line.visible = false
                            members.visible = true
                        }
                    }
                }
                Text {
                    id: lineValue
                    text: value
                    color: "#ffffff"
                    font.pixelSize: 30
                    anchors.top: lineHead.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    width: 300
                    height: 30
                    elide: Text.ElideRight
                }
                Column {
                    id: members
                    spacing: 16
                    visible: false
                    anchors.top: lineHead.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    Repeater {
                        model: memberModel.count
                        Text {
                            width: 300
                            height: 30
                            color: "#ffffff"
                            font.pixelSize: 30
                            text: contactermodel.getContacterName(memberModel.get(index).id)
                        }
                    }
                    Component.onCompleted: {
                        if (lineValue.text == "参与成员") {
                            lineHistory.height = lineHistory.height + (memberModel.count - 1) * 46
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onPositionChanged: {
                        clickeffects.opacity = 0
                    }
                }

                CLine {
                    id: line
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    width: parent.width - 96
                    anchors.bottom: lineHistory.bottom
                    anchors.bottomMargin: 24
                    color: "#9b9b9b"
                }
                Rectangle {
                    id: clickeffects
                    anchors.fill: parent
                    opacity: 0
                    color: "black"
                }
            }
        }
        Timer {
            id: timer
            interval: 500; running: true; repeat: false
            onTriggered: bIsDoubleClick = false
        }
        ListModel {
            id: memberModel
            Component.onCompleted: {
                memberModel.append({"id":"22"})
                memberModel.append({"id":"33"})

                memberModel.append({"id":"44"})

            }
        }

        ListModel {
            id: listmodel
        }
        ListView {
            id: listview
            anchors.top: addPhoto.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 48
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
            width: parent.width - 96
            clip: true
            model: listmodel
            delegate: myhistory
            Component.onCompleted: {
                listmodel.append({"textTitle":"职位","value":"personPost"})
                listmodel.append({"textTitle":"编号","value":"usrid"})
                listmodel.append({"textTitle":"开始时间","value":"personPost"})
                listmodel.append({"textTitle":"结束时间","value":"usrid"})
                listmodel.append({"textTitle":"参与成员","value":"usrid"})

            }
        }
    }
    Rectangle {
        id: sureback
        anchors.fill: parent
        visible: false
        color: "black"
        opacity: 0
        z: 4
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
        z: 4
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
            text: qsTr("是否清空该通话记录?")
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
                sureback.opacity = 0
                suredialog.visible = false
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
                sureback.opacity = 0
                suredialog.visible = false
                if (bIsStranger) {
                    getData.removeCallHistory(personName,true)
                } else {
                    getData.removeCallHistory(usrid,false)
                }
                pageStack.pop()
            }
        }
    }
}
