import QtQuick 2.0
import com.syberos.basewidgets 2.0
CPage {
    id: personInfo
    property alias fromText: fromText.text
    default property var userId
    property string contactertype: "1"
    property var m_name: "value"
    property bool bluetoothShare: false
    property bool bIsNormalPerson: false
    property var parentSource2: null
    property int sendtype;//0:real 1:file send
    orientationPolicy: CPageOrientation.Automatic

    Component.onCompleted:	{
        personInfo.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personInfo.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            personInfo.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
        if (fromText.text == "local")
        {
            listView.delegate = myLine
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personInfo.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                personInfo.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }

    function test()
    {
        console.log("moreinfo to  info")
    }
    function addNetworkType(network)
    {
        listModel.insert(listModel.count - 2,{"textTitle":network})
    }

    function setBluetoothShare(bIsShareing)
    {
        bluetoothShare = bIsShareing
        deleteback.opacity = 0.7
        bluedialog.visible = true
    }

    Connections {
        target: realtimeVideo
        onSignalContacterInfo: {
            if (contactertype ==  2) {
                bIsNormalPerson = true
            }
            if (contactertype ==  2 && key == "设备序列号")
                return
            listModel.append({"textTitle":key,"value":value})
        }
    }
    Connections {
        target: realtimeVideo
        onSignalSendNameToPersonInfo: {
            myName.text = name
            m_name = name
            console.log(name)
            if (photoPath == "null" || photoPath == ""){
                addPhoto.item.myIconSource = "qrc:/main/images/init_photo.png"
            }else {
                addPhoto.item.myIconSource = photoPath
            }
        }
    }
    contentAreaItem: Item {
        anchors.fill: parent
        Text {
            id: fromText
            text: qsTr("text")
            visible: false
        }

        CEditTitleBar {
            id:titleBar
            anchors.top: parent.top
            height: 96
            width: parent.width
            leftItemSize: Qt.size(72,72)
            leftItemComponent: leftTitleBar
            titlePixelSize: 30
            titleTextColor: "#f9f9f9"
            titleAreaCentered: true
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
            backgroundEnabled: true
            backgroundComponent: Component {
                Image {
                    anchors.fill: parent
                    source: "qrc:/main/images/top_line.png"
                }
            }
            Component.onCompleted: {
                if (fromText.text == "local"){
                    titleBar.rightItemEnabled = false
                }
            }
        }
        Item {
            id: mid
            width: parent.width
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
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
        Loader {
            id: addPhoto
            width: 150
            height: 150
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            anchors.left: parent.left
            anchors.leftMargin: 48
            source: "qrc:/qml/Photo.qml"
            onLoaded: addPhoto.item.myIconSource = "qrc:/main/images/init_photo.png"
        }

        Text {
            id: myName
            text: qsTr("text ttt")
            color: "#ffffff"
            font.pixelSize: 40
            anchors.left: addPhoto.right
            anchors.leftMargin: 48
            anchors.top: addPhoto.top
            anchors.topMargin: 55
            height: 40
            width: 300
            elide: Text.ElideRight
        }
        Item {
            id: setMyName
            anchors.top: titleBar.bottom
            anchors.topMargin: 61
            anchors.left: myName.left
            anchors.right: parent.right
            anchors.rightMargin: 48
            visible: false
            Text {
                id: firstName
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 16
                font.pixelSize: 40
                color: "#b6b6b6"
                text: qsTr("姓")
            }
            TextInput {
                id: firstNameEdit
                anchors.left: firstName.right
                anchors.leftMargin: 24
                anchors.right: parent.right
                anchors.top: firstName.top
                font.pixelSize: 40
                color: "#ffffff"
                text: myName.text.split(" ")[0]
                onTextChanged: {
                    m_name = firstNameEdit.text + " " + secondNameEdit.text
                }
            }

            CLine {
                id: lineFirst
                anchors.top: firstName.bottom
                anchors.topMargin: 24
                width: parent.width
                color: "#9b9b9b"
            }
            Text {
                id: secondName
                anchors.top: lineFirst.bottom
                anchors.topMargin: 24
                anchors.left: firstName.left
                font.pixelSize: 40
                color: "#b6b6b6"
                text: qsTr("名")

            }
            TextInput {
                id: secondNameEdit
                anchors.left: secondName.right
                anchors.leftMargin: 24
                anchors.right: parent.right
                anchors.top: secondName.top
                font.pixelSize: 40
                color: "#ffffff"
                text: myName.text.split(" ")[1]
                onTextChanged: {
                    m_name = firstNameEdit.text + " " + secondNameEdit.text
                }
            }

            CLine {
                id: lineSecond
                anchors.top: secondName.bottom
                anchors.topMargin: 24
                width: parent.width
                color: "#9b9b9b"
            }
        }

        ListModel {
            id: listModel
        }
        ListView {
            id: listView
            model: listModel
            anchors.left: parent.left
            width: parent.width
            anchors.top: addPhoto.bottom
            anchors.topMargin: 24
            anchors.bottom: parent.bottom
            delegate: myLine
            clip: true
            //interactive: false
            Component.onCompleted: {
                if ((fromText.text == "network") || (fromText.text == "group")){
                    realtimeVideo.getContacter(userId)
                }
            }
        }
        Rectangle {
            id: deleteback
            anchors.fill: parent
            color: "black"
            opacity: 0
        }
    }

    Component {
        id: myLine
        Item {
            id: tempItem
            height: 124
            width: listView.width
            Text {
                id: lineHead
                color: "#b6b6b6"
                text: textTitle
                font.pixelSize: 30
                height: 30
                anchors.top: parent.top
                anchors.topMargin: 24
                anchors.left: parent.left
                anchors.leftMargin: 48
                onTextChanged: {
                    if (text == "自组网" || text == "战互网" || text == "天通网" || text == "LTE" || text == "数字对讲" || text == "WIFI")
                    {
                        if (fromText.text == "local")
                            loadButtonBox1.sourceComponent = null
                        if (bIsNormalPerson) {
                            lineHead.anchors.topMargin = -100
                        }
                        network.visible = true
                    } else if (bIsNormalPerson && text == "编号") {
                        lineHead.visible = false
                        lineValue.visible = false
                        line.visible = false
                        loadButtonBox1.sourceComponent = null
                    }
                    else if (text == "设备序列号")
                    {
                        if (bIsNormalPerson) {
                            lineHead.anchors.topMargin = -100
                            lineValue.visible = false
                            line.visible = false
                        }
                        loadButtonBox1.sourceComponent = null
                    }
                    else
                    {
                        loadButtonBox1.sourceComponent = null
                    }
                }
            }
            Image {
                id: network
                anchors.top: parent.top
                anchors.topMargin: 24
                anchors.left: lineHead.right
                anchors.leftMargin: 16
                visible: false
                //source: "qrc:/main/images/unnetwork.png"
            }
            Text {
                id: lineValue
                text: (contactertype == 2 && lineHead.text == "职位") ? "" : value
                color: "#ffffff"
                font.pixelSize: 30
                anchors.top: lineHead.bottom
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 48
                height: 30
                width: 300
                //elide: Text.ElideRight
            }
            CLine {
                id: line
                anchors.left: parent.left
                anchors.leftMargin: 48
                width: parent.width - 96
                anchors.top: lineValue.bottom
                anchors.topMargin: 24
                color: "#9b9b9b"
            }
            Loader {
                id: loadButtonBox1
                anchors.top: lineHead.top
                anchors.topMargin: -24
                anchors.right: line.right
                anchors.rightMargin: 24
                //anchors.verticalCenter: parent.verticalCenter
                sourceComponent: buttonBox
                onLoaded: {
                    loadButtonBox1.item.index = index
                }
            }
        }

    }

    Component {
        id: leftTitleBar
        CButton {
            anchors.fill: parent
            pixelSize: 30
            textColor: "#ffffff"
            text: qsTr("返回")
            iconSource:"qrc:/main/images/back-pop.png"
            backgroundEnabled: false
            onClicked: {
                pageStack.pop()
            }
        }
    }

    Component {
        id: rightTitleBar
        Item {
            Text {
                id: rightText
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 30
                color: "#ffffff"
                text: qsTr("确定")
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {

                }
            }
        }
    }

    Component {
        id: buttonBox
        Item {
            property int index: -1
            width: 72
            height: 124
            CButton{
                id: dial
                width: 72
                height: 72
                anchors.right: parent.right
                anchors.leftMargin: 72
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "qrc:/main/images/dial.png"
                backgroundEnabled: false
                onClicked: {
                    //csDbus.dials(listModel.get(index).textTitle, listModel.get(index).value)
                    //pageStack.pop(parentSource2)
                    if(sendtype==0)
                    {
                        realtimeVideo.callRemote(listModel.get(index).value);
                        pageStack.push("qrc:/qml/MainPage.qml",{waitVisable:true,ip:listModel.get(index).value});
                    }else if(sendtype==1)
                    {
                        filesSendModel.setRemoteIp(listModel.get(index).value)
                        pageStack.push("qrc:/qml/VideoViewPage.qml",{type:1,sendtype:1,ip:listModel.get(index).value});
                    }
                }
            }
        }
    }
    Loader {
        id:dialog
    }
}
