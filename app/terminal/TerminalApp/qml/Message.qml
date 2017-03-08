import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: message
    contentAreaItem: Item {

        Component.onCompleted:	{
            //getData.getImageMessageList()
            message.color = "#293443"
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                message.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                textBorder.width = 1097
            } else {
                message.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                textBorder.width = 537
            }
        }

        Connections{
            target: gScreenInfo
            onCurrentOrientationChanged: {
                if (gScreenInfo.currentOrientation == 2 ||
                        gScreenInfo.currentOrientation == 8) {
                    chatRoom.statusBarHoldEnabled = false
                    gScreenInfo.setStatusBar(false);
                    textBorder.width = 1097
                } else {
                    chatRoom.statusBarHoldEnabled = true
                    gScreenInfo.setStatusBar(true);
                    textBorder.width = 537
                }
            }
        }

        Loader {
            id: photo
            width: 150
            height: 150
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 26
            source: "qrc:/qml/Photo.qml"
            onLoaded: photo.item.myIconSource = "qrc:/main/images/message-photo.png"
            MouseArea {
                id: mousearea
                visible: false
                anchors.fill: parent
                onClicked: {

                }
            }
        }

        Text {
            id: personName
            height: 32
            text: qsTr("1388888888")
            color: "#29afe2"
            font.pixelSize: 32
            anchors.verticalCenter: photo.verticalCenter
            anchors.left: photo.right
            anchors.leftMargin: 24
        }
        ListModel {
            id: listModel
            Component.onCompleted: {
                listModel.append({"direction":"left","text":"tweqwerqwerqewrqewrqerqerqer"})
                listModel.append({"direction":"left","text":"tweqwerqwerqewrqewrqerqerqer"})
                listModel.append({"direction":"right","text":"tweqwerqwerqewrqewrqerqerqer"})
            }
        }

        ListView {
            id: listeView
            width: parent.width
            anchors.top: personName.bottom
            anchors.topMargin: 65
            anchors.bottom: bottomLine.bottom
            delegate: lineDelegate
            model: listModel
            spacing: 22
        }

        Rectangle {
            id: bottomLine
            color: "#546e8f"
            height: 100
            width: parent.width
            opacity: 0.3
            anchors.bottom: parent.bottom
            Rectangle {
                id: textBorder
                color: "#293443"
                radius: 10
                height: 66
                anchors.left: parent.left
                anchors.leftMargin: 36
                anchors.verticalCenter: parent.verticalCenter

                CTextArea {
                    textColor: "#ffffff"
                    cursorColor: "#ffffff"
                    anchors.fill: parent
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                }
            }
            CButton {
                id: send
                text: "发送"
                textColor: "#29afe2"
                backgroundEnabled: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: textBorder.right
            }
        }


        Component {
            id: lineDelegate
            Item {
                id: lineItem

                Loader {
                    id: lineLoader
                    onLoaded: {
                    }
                }

                Component.onCompleted: {
                    if (direction == "left"){
                        lineLoader.sourceComponent = textComponentLeft
                    }
                    else {
                        lineLoader.sourceComponent = textComponentRight
                    }
                    lineLoader.item.text = text
                    lineItem.height = lineLoader.item.myHeight + 100
                }
            }
        }


        Component {
            id: textComponentLeft
            Item {
                id: itemtl
                property alias text: myTexttl.text
                property alias myHeight: bubbletl.height
                property alias bubbleSource: bubbletl.source
                BorderImage {
                    id: bubbletl
                    width: 150; height: 92
                    border.left: 20; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.left: parent.right
                    anchors.leftMargin: 36
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_left.png"

                    Text {
                        id: myTexttl
                        text: qsTr("")
                        color: "black"
                        font.pixelSize: 40
                        wrapMode: Text.Wrap
                        anchors.margins: 24
                        anchors.fill: parent
                        onContentSizeChanged: {
                            if (contentWidth < 410){
                                bubbletl.width = contentWidth + 48
                            }else {
                                bubbletl.width = 458
                                myTexttl.width = 410
                            }
                            bubbletl.height = myTexttl.contentHeight + 48
                        }
                    }
                }
            }
        }

        Component {
            id: textComponentRight
            Item {
                id: itemtr
                width: listView.width
                property alias text: myTexttr.text
                property alias myHeight: bubbletr.height
                property alias bubbleSource: bubbletr.source
                BorderImage {
                    id: bubbletr
                    width: 150; height: 92
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 36
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_right.png"

                    Text {
                        id: myTexttr
                        text: qsTr("")
                        color: "#ffffff"
                        font.pixelSize: 40
                        wrapMode: Text.Wrap
                        anchors.margins: 24
                        anchors.fill: parent
                        onContentSizeChanged: {
                            if (contentWidth < 410){
                                bubbletr.width = myTexttr.contentWidth + 48
                            }else {
                                bubbletr.width = 458
                                myTexttr.width = 410
                            }
                            bubbletr.height = myTexttr.contentHeight + 48
                        }
                    }
                }
                Component.onCompleted: {
                    itemtr.height = bubbletr.height
                    bubbletr.height = myTexttr.contentHeight + 48
                }
            }
        }

    }
}
