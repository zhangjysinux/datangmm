import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: callRecordInfo
    property bool bIsStranger: false/*是否陌生人*/
    property string personName: ""
    property string personPost: ""
    property string usrid: ""
    property string calledPhone: "qrc:/main/images/called.png"
    property string dialedPhone: "qrc:/main/images/dialed.png"
    property string uncallPhone: "qrc:/main/images/unCall.png"
    property bool bIsDoubleClick: false
    property bool bIsFirst: true/*是否第一次*/
    property bool bIsFromPerson: false/*是否从联系人详情界面迁移过来*/
    property var myIndex: 0
    Connections {
        target: getData
        onSignalContacterChanged:{
            if (usrid == id){
                myName.text = historyTable.getCallHistoryValue(myIndex,"num")
            }
        }
    }

    Connections {
        target: getData
        onSignalSendHistoryInfo: {
            listmodel.append({"id":id,"positioninfo":"","postinfo":"","dialflag":type,"networkflag":network,
                                 "timeinfo":time,"netinfo":netinfo,"numinfo":phonenum,"phonetimeinfo":phonetime})
        }
        onSignalCallHistoryChanged: {
            if (type == 1) {
                if (bIsStranger) {
                    for (var i = listmodel.count; i > 1; i--)
                        listmodel.remove(i-1,1)
                    getData.getCallHistoryBytarget(personName,true)
                } else {
                    if (getData.bIsOrganization(usrid)) {
                        for (var i = listmodel.count; i > 2; i--)
                            listmodel.remove(i-1,1)
                    } else {
                        for (var i = listmodel.count; i > 1; i--)
                            listmodel.remove(i-1,1)
                    }
                    getData.getCallHistoryBytarget(usrid,false)
                }
            }
        }

        onSignalSendNameToPersonInfo: {
            personName = name
            if (photoPath == "null" || photoPath == ""){
                addPhoto.item.myIconSource = "qrc:/main/images/init_photo.jpg"
            }else {
                addPhoto.item.myIconSource = photoPath
            }
        }
    }
    function addPersonBackUp(name,photopath)
    {
        bIsStranger = false
        personName = name
        addPhoto.item.myIconSource = photopath
    }
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        callRecordInfo.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            callRecordInfo.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            callRecordInfo.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                callRecordInfo.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                callRecordInfo.statusBarHoldEnabled = true
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
                textColor: "#29afe2"
                backgroundEnabled: false
                text: qsTr("清空")
                iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                spacing: -68
                onClicked: {
                    if (!bIsStranger) {
                        if ((!getData.bIsOrganization(usrid) && listmodel.count > 1) ||
                                (getData.bIsOrganization(usrid) && listmodel.count > 2)) {
                            sureback.visible = true
                            sureback.opacity = 0.7
                            suredialog.visible = true
                        }
                    } else {
                        if (listmodel.count > 1) {
                            sureback.visible = true
                            sureback.opacity = 0.7
                            suredialog.visible = true
                        }
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
                text: "返回"
                textColor: "#29afe2"
                spacing: 0
                iconSource: "qrc:/main/images/back.png"
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
//        Item {
//            id: mid
//            width: parent.width
//            anchors.top: titleBar.bottom
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
        Loader {
            id: addPhoto
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            anchors.left: parent.left
            anchors.leftMargin: 48
            width: 150
            height: 150
            source: "qrc:/qml/Photo.qml"
            onLoaded: addPhoto.item.myIconSource
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!bIsFromPerson) {
                        if (!bIsStranger) {
                            if (getData.bIsOrganization(usrid) ) {
                                pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":usrid,"contactertype":"1","bIsFromHistory":true})
                            } else {
                                pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":usrid,"contactertype":"2","bIsFromHistory":true})
                            }
                        }
                    }
                }
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
            text: personName
            color: "white"
            font.pixelSize: 32
            elide: Text.ElideRight
        }
        Component {
            id: myhistory
            Item {
                width: listview.width
                height: 112
                Text {
                    id: position
                    anchors.top: parent.top
                    anchors.topMargin: 29
                    anchors.left: parent.left
                    width: 120
                    height: 30
                    text: positioninfo
                    color: "#ffffff"
                    visible: false
                    font.pixelSize: 22
                    onTextChanged: {
                        position.visible = true
                        post.visible = true
//                        line.anchors.top = post.bottom
//                        line.anchors.topMargin = 24
                        if (bIsFirst) {
                            bIsFirst = false
                            if (!bIsStranger){
                                if (!getData.bIsOrganization(usrid) && listmodel.count == 1){
                                    titleBar.rightItem.enabled = false
                                    titleBar.rightItem.opacity = 0.5
                                }
                                else if (getData.bIsOrganization(usrid) && listmodel.count == 2){
                                    titleBar.rightItem.opacity = 0.5
                                    titleBar.rightItem.enabled = false
                                }
                            } else {
                                if (listmodel.count == 1){
                                    titleBar.rightItem.opacity = 0.5
                                    titleBar.rightItem.enabled = false
                                }
                            }
                        }
                    }
                }
                Text {
                    id: post
                    anchors.top: position.bottom
                    anchors.topMargin: 12
                    anchors.left: parent.left
                    width: 120
                    height: 30
                    text: postinfo
                    font.pixelSize: 32
                    visible: false
                    color: "#ffffff"
                }
                Image {
                    id: dial
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    width: 28
                    height: 28
                    visible: false
                    source: ""
                }
                Image {
                    id: voice
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.left: dial.right
                    anchors.leftMargin: 8
                    height: 28
                    width: 28
                    visible: false
                    source: ""
                }
                Image {
                    id: video
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.left: voice.right
                    anchors.leftMargin: 20
                    height: 28
                    width: 28
                    visible: false
                    source: ""
                }
                Text {
                    id: network
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.left: video.right
                    anchors.leftMargin: 20
                    text: netinfo
                    width: 90
                    height: 30
                    visible: false
                    clip: true
                    color: "#ffffff"
                    font.pixelSize: 22
                }
                Text {
                    id: phonenum
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.left: network.right
                    anchors.leftMargin: 20
                    width: 220
                    height: 30
                    clip: true
                    visible: false
                    text: numinfo
                    color: "#ffffff"
                    font.pixelSize: 22
                    elide: Text.ElideRight
                }
                Text {
                    id: time
                    anchors.top: dial.bottom
                    anchors.topMargin: 21
                    anchors.left: parent.left
                    width: 400
                    horizontalAlignment: Qt.AlignLeft
                    height: 30
                    clip: true
                    color: "#ffffff"
                    text: timeinfo
                    visible: false
                    font.pixelSize: 22
                    onTextChanged: {
                        dial.visible = true
                        voice.visible = true
                        video.visible = true
                        time.visible = true
                        network.visible = true
                        phonenum.visible = true
                        phonetime.visible = true
                        if (listmodel.get(index).dialflag == 1) {
                            dial.source = calledPhone
                        } else if (listmodel.get(index).dialflag == 2) {
                            dial.source = dialedPhone
                        } else if (listmodel.get(index).dialflag == 3) {
                            dial.source = uncallPhone
                        }
                        if (listmodel.get(index).networkflag == 1 ||
                                listmodel.get(index).networkflag == 3 ||
                                listmodel.get(index).networkflag == 4 ||
                                listmodel.get(index).networkflag == 5 ||
                                listmodel.get(index).networkflag == 6 ||
                                listmodel.get(index).networkflag == 10 ||
                                listmodel.get(index).networkflag == 11 ||
                                listmodel.get(index).networkflag == 13 ||
                                listmodel.get(index).networkflag == 8) {
                            video.source = "qrc:/main/images/video.png"
                            voice.source = "qrc:/main/images/voiceing.png"
                        } else if (listmodel.get(index).networkflag == 2 ||
                                   listmodel.get(index).networkflag == 7 ||
                                   listmodel.get(index).networkflag == 9 ||
                                   listmodel.get(index).networkflag == 12 ) {
                            voice.source = "qrc:/main/images/voice.png"
                            video.source = "qrc:/main/images/videoing.png"
                        }
                    }
                }
                Text {
                    id: phonetime
                    anchors.top: parent.top
                    anchors.topMargin: 55
                    anchors.right: parent.right
                    font.pixelSize: 22
                    color: "white"
                    text: phonetimeinfo
                    visible: false
                    clip: true
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (bIsStranger) {
                            if (index > 0) {
                                clickeffects.opacity = 0.5
                            }
                        } else {
                            if ((!getData.bIsOrganization(usrid) && index > 0) ||
                                    (getData.bIsOrganization(usrid) && index > 1)) {
                                clickeffects.opacity = 0.5
                            }
                        }
                    }
                    onReleased: {
                        if (clickeffects.opacity == 0.5) {
                            clickeffects.opacity = 0
                            if (!bIsDoubleClick){

                                var url
                                if (listmodel.get(index).networkflag == 7)
                                    url = "sinuxtel:makeVideoCall?phoneNum="+numinfo
                                else
                                    url = "sinuxtel:makeCall?net=" + netinfo + "&value=" + numinfo

                                timer.start()
                                qApp.openUrl(url)
                            }
                        }
                    }
                    onPositionChanged: {
                        clickeffects.opacity = 0
                    }
                }
                CLine {
                    id: line
                    anchors.top: parent.bottom
                    width: parent.width
                    color: "#36465a"
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
            id: listmodel
        }
        ListView {
            id: listview
            anchors.top: addPhoto.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 48
            anchors.bottom: bIsStranger ? newPerson.top : parent.bottom
            anchors.bottomMargin: 15
            width: parent.width - 96
            clip: true
            model: listmodel
            delegate: myhistory
            Component.onCompleted: {
                listmodel.append({"id":"","positioninfo":"职位","postinfo":personPost,"timeinfo":""})
                if (!bIsStranger) {
                    if (getData.bIsOrganization(usrid)) {
                        listmodel.append({"id":"","positioninfo":"编号","postinfo":usrid,"timeinfo":""})
                    }
                }
            }
        }
        Item {
            id: newPerson
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 48
            height: 136
            width: parent.width - 96
            visible: bIsStranger
            CButton {
                id: newbutton
                anchors.top: parent.top
                anchors.topMargin: 24
                anchors.left: parent.left
                pixelSize: 40
                backgroundEnabled: false
                text: qsTr("新建联系人")
                textColor: "#31bd2c"
                onClicked: {
                    pageStack.push("qrc:/qml/AddPerson.qml",{"thenetinfo":listmodel.get(listmodel.count-1).netinfo,
                                       "thenuminfo":listmodel.get(listmodel.count-1).numinfo,
                                       "containerqml":callRecordInfo})
                }
            }
            CLine {
                anchors.top: newbutton.bottom
                anchors.topMargin: 24
                width: parent.width
                color: "#36465a"
            }
        }
    }
    Rectangle {
        id: sureback
        anchors.fill: parent
        visible: false
        color: "black"
        opacity: 0.3
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
        width: 504
        height: 296
        radius: 10
        z: 4
        color: "#ffffff"
        Image {
            anchors.fill: parent
            source: "qrc:/bluetooth/images/bluetoothblack.png"
        }
        Text {
            id: message
            anchors.top: parent.top
            anchors.topMargin: 79
            height: 32
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            color: "black"
            font.pixelSize: 32
            text: qsTr("要删除所有通话记录吗？")
        }
        CButton {
            id: surecancel
            anchors.top: message.bottom
            anchors.topMargin: 69
            anchors.left: parent.left
            anchors.leftMargin: 62
            width: 150
            height: 67
            backgroundEnabled: true
            textColor: "#ffffff"
            text: "取消"
            backgroundComponent: Rectangle {
                color: "#29afe2"
                radius: 10
            }

            onClicked: {
                sureback.visible = false
                sureback.opacity = 0
                suredialog.visible = false
            }
        }
        CButton {
            id: surebutton
            anchors.top: message.bottom
            anchors.topMargin: 69
            anchors.left: surecancel.right
            anchors.leftMargin: 80
            width: 150
            height: 67
            backgroundEnabled: true
            textColor: "#ffffff"
            text: "确认"
            backgroundComponent: Rectangle {
                color: "#29afe2"
                radius: 10
            }
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
