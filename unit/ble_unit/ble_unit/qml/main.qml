import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        width:parent.width
        height:parent.height

        contentAreaItem: Item {
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10

            Rectangle {
                id: iprectangle
                width: parent.width
                height: 80

                radius: 5
                color: "white"
                border.width: 2
                border.color: "black"
                anchors.top: parent.top
                anchors.left: parent.left

                Text{
                    id: iptitle
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter

                    text: "IP:"
                }

                TextInput{
                    id: ipaddress
                    text: "127.0.0.1"
                    width: parent.width - iptitle.width - 20

                    anchors.left: iptitle.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Rectangle {
                id: textinputrectangle
                width: parent.width
                height: 200

                radius: 5
                color: "white"
                border.width: 2
                border.color: "black"
                anchors.top: iprectangle.bottom
                anchors.topMargin: 5
                anchors.left: parent.left

                TextEdit {
                    id: textinput
                    text: "hello world"

                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                }
            }

            Rectangle {
                id: textbrowserrectangle
                width: parent.width
                height: 600

                radius: 5
                color: "white"
                border.width: 2
                border.color: "black"
                anchors.top: textinputrectangle.bottom
                anchors.topMargin: 5
                anchors.left: parent.left

                CTextArea {
                    id: textbrowser
                    readOnly: true

                    anchors.fill: parent
                    anchors.margins: 5
                }
            }

            CButton {
                id: sendbutton
                text: "Send"
                pixelSize: 48

                anchors.top: textbrowserrectangle.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    sender.sendText(textinput.text, ipaddress.text)
                }
            }
        }
    }

    function onReceive(text)
    {
        textbrowser.text = textbrowser.text + text;
    }
}

