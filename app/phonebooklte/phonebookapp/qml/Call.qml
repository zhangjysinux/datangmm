import QtQuick 2.0
import com.syberos.basewidgets 2.0
CPage {
    id: call
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
            text: qsTr("IP:")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: textip.top
            color: "#ffffff"
            font.pixelSize: 30
        }
        BorderImage {
            source: "qrc:/main/images/search.png"
            id: textip
            anchors.bottom: button1.top
            anchors.bottomMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            height: 90
            width: 500
            property alias text: myip.text
            TextInput {
                id: myip
                anchors.fill: parent
                font.pixelSize: 40
                color: "#ffffff"
                anchors.centerIn: parent
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                text: mainroot.sendip
            }
        }
        CButton {
            id: button1
            anchors.centerIn: parent
            width: 300
            height: 150
            text: "Dial"
            pixelSize: 40
            textColor: "#ffffff"
            onClicked: {
//                var dialip = getData.dial(textip.text)
//                mainroot.sendip = textip.text
//                mytext.text = "Already dial to " + dialip
                var url = "sinuxtel:callInComing?net=voip&value=" + textip.text
                qApp.openUrl(url)
            }
        }

        CButton {
            id: button
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: button1.bottom
            anchors.topMargin: 20
            width: 300
            height: 150
            text: "Hang up"
            pixelSize: 40
            textColor: "#ffffff"
            onClicked: {
                getData.hangUp()
                pageStack.pop()

            }
        }
    }
}


