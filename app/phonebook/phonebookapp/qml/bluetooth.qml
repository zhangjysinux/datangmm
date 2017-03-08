import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: bluetooth
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            bluetooth.orientationPolicy = CPageOrientation.LockLandscape
        bluetooth.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            bluetooth.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            bluetooth.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                bluetooth.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                bluetooth.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }

    property var containerqml: null

    Connections {
        target: getData

        onSignalSendBlueToothInfo: {
            //positionname, personname
            listmodel.append({ "name": name, "address": address, "positionname": contactTitle, "personname": contactName })
        }
        onSignalSendLocalDeviceInfo: {
            myselfBluetooth.phoneNameText = bluetoothName
            myselfBluetooth.positionText = title
            myselfBluetooth.personText = name
        }
    }

    contentAreaItem: Item {
        anchors.fill: parent
        CTitleBar {
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            rightItemEnabled: false
            leftItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemComponent: CButton {
                id: leftTitle
                anchors.fill: parent
                iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                backgroundEnabled: false
                onClicked: {
                    pageStack.pop()
                }
            }
            titleAreaCentered: true
            titlePixelSize: 30
            titleText: qsTr("蓝牙设备选择")
            titleTextColor: "#f9f9f9"
            spacingBetweenLeftBorderAndLeftItem: 16
            backgroundEnabled: true
            backgroundComponent: Component{
                Image {
                    anchors.fill: parent
                    source: "qrc:/bluetooth/images/bluebackground.png"
                }
            }
        }
        Item {
            id: mid
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
            Image {
                fillMode: Image.Tile
                anchors.fill: parent
                source: "qrc:/main/images/back_mid_up.png"
            }            width: parent.width
            Image {
                anchors.fill: parent
                source: "qrc:/main/images/back_mid.png"
            }
        }
        ListModel {
            id: listmodel
        }

        //显示自己设备
        Item {
            id: myselfBluetooth
            anchors {
                top: titleBar.bottom; topMargin: 16
                left: parent.left; leftMargin: 48
            }
            width: parent.width - 80
            height: 83

            property alias phoneNameText: phoneName.text
            property alias positionText: position.text
            property alias personText: person.text

            Image {
                id: phoneImage
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.left: parent.left
                width: 50
                height: 50
                source: "qrc:/bluetooth/images/phone.png"
            }

            Text {
                id: phoneName
                anchors.left: phoneImage.right
                anchors.leftMargin: 24
                anchors.top: parent.top
                anchors.topMargin: 28
                width: Math.min(implicitWidth, 400)
                height: 30
                color: "white"
                font.pixelSize: 30
                clip: true
                text: "unknow device"
            }

            Text {
                id: position
                anchors.right: person.left
                anchors.rightMargin: 24
                anchors.top: parent.top
                anchors.topMargin: 28
                width: 60
                height: 30
                clip: true
                color: "white"
                text: "unknow"
            }

            Text {
                id: person
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 28
                horizontalAlignment: Qt.AlignRight
                width: 150
                height: 30
                clip: true
                color: "white"
                text: "unknow"
            }

            CLine {
                id: line
                anchors.top: phoneName.bottom
                anchors.topMargin: 24
                width: parent.width
                color: "#9b9b9b"
            }

            Component.onCompleted: {
                getData.getLocalDeviceInfo()
            }
        }

        Text {
            id: titleText
            anchors.left: parent.left
            anchors.leftMargin: 48
            anchors.top: myselfBluetooth.bottom
            anchors.topMargin: 16
            text: "设备"
            color: "lightGray"
        }

        ListView {
            id: listview
            anchors.top: titleText.bottom
            anchors.topMargin: 16
            anchors.left: parent.left
            anchors.leftMargin: 48
            anchors.bottom: toolBar.top
            width: parent.width - 80
            clip: true
            model: listmodel
            delegate: Component {
                id: bluetoothlist
                Item {
                    width: listview.width
                    property int heightchange: 83
                    height: heightchange
                    //点击蓝牙设备列表项
                    MouseArea {
                        id: mousearea
                        anchors.fill: parent
                        visible: false
                        onClicked: {
                            containerqml.setBluetoothShare(true, name, address)
                            pageStack.pop(containerqml)
                        }
                    }

                    Image {
                        id: phone
                        anchors.top: parent.top
                        anchors.topMargin: 16
                        anchors.left: parent.left
                        width: 50
                        height: 50
                        source: "qrc:/bluetooth/images/phone.png"
                    }
                    Text {
                        id: phonename
                        anchors.left: phone.right
                        anchors.leftMargin: 24
                        anchors.top: parent.top
                        anchors.topMargin: 28
                        width: Math.min(implicitWidth, 400)
                        height: 30
                        color: "white"
                        font.pixelSize: 30
                        clip: true
                        text: name
                        onTextChanged: {
                            if (name == "小米") {
                                heightchange = 137
                                device.visible = true
                            } else {
                                mousearea.visible = true
                            }
                        }
                    }

                    Text {
                        id: position
                        anchors.right: person.left
                        anchors.rightMargin: 24
                        anchors.top: parent.top
                        anchors.topMargin: 28
                        width: 60
                        height: 30
                        clip: true
                        color: "white"
                        text: positionname
                    }
                    Text {
                        id: person
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.topMargin: 28
                        horizontalAlignment: Qt.AlignRight
                        width: 150
                        height: 30
                        clip: true
                        color: "white"
                        text: personname
                    }
                    CLine {
                        id: line
                        anchors.top: phonename.bottom
                        anchors.topMargin: 24
                        width: parent.width
                        color: "#9b9b9b"
                    }
                    Text {
                        id: device
                        anchors.top: line.bottom
                        anchors.topMargin: 24
                        anchors.left: parent.left
                        width: 60
                        height: 30
                        font.pixelSize: 30
                        visible: false
                        color: "white"
                        text: qsTr("设备")
                    }
                }
            }
        }

        CToolBar {
            id: toolBar
            anchors.bottom: parent.bottom
            width: parent.width
            height: 120
            backgroundEnabled: true
            backgroundComponent: Component {
                Image {
                    anchors.fill: parent
                    source: "qrc:/bluetooth/images/bluebackground.png"
                }
            }
            CButton {
                width: 200
                height: 80
                iconSource: "qrc:/bluetooth/images/searchphone.png"
                onClicked: {
                    listmodel.clear()
                    getData.findblueToothDevice()
                }
            }
        }
    }
}
