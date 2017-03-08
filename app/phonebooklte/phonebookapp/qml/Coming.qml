import QtQuick 2.0
import com.syberos.basewidgets 2.0
CPage {
    id: coming
    anchors.fill: parent
    Item {
        id: mid
        anchors.fill: parent
        signal buttonClick()
        Image {
            anchors.fill: parent
            source: "qrc:/main/images/back_mid_up.png"
        }
        Image {
            anchors.fill: parent
            source: "qrc:/main/images/back_mid.png"
        }
        Text {
            id: mytext
            text: qsTr("A Phone Coming:")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: button1.top
            color: "#ffffff"
            font.pixelSize: 32
        }
        CButton {
            id: button1
            anchors.centerIn: parent
            width: 300
            height: 150
            text: "接听"
            pixelSize: 40
            textColor: "#ffffff"
            onClicked: {
                getData.accept()
            }
        }

        CButton {
            id: button
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: button1.bottom
            anchors.topMargin: 20
            width: 300
            height: 150
            text: "拒接"
            pixelSize: 40
            textColor: "#ffffff"
            onClicked: {
                getData.decline();
                pageStack.pop()
            }
        }
        CButton {
            id: button3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: button.bottom
            anchors.topMargin: 20
            width: 300
            height: 150
            text: "返回"
            pixelSize: 40
            textColor: "#ffffff"
            onClicked: {
                pageStack.pop()
            }
        }

    }
}
