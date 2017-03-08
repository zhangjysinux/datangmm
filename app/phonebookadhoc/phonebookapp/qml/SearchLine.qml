import QtQuick 2.0
import com.syberos.basewidgets 2.0

Item {
    id: searchLine
    anchors.top:parent.top
    width: parent.width; height: 90
    signal signalgetinputtext(string text)
    property alias inputtext: input
    BorderImage {
        anchors.centerIn: parent
        source: "qrc:/main/images/search.png"
        width: parent.width - 32; height: 72
        Item {
            visible: !input.activeFocus && !input.text.length
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -40
            Image {
                id: searchIcon
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/main/images/search_icon.png"
            }
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: searchIcon.right
                anchors.leftMargin: 16
                font.pixelSize: 28
                color: "#ffffff"
                text: qsTr("搜索")
            }
        }
        TextInput {
            id: input
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            clip: true
            color: "#ffffff"
            font.pixelSize: 32
            //validator: RegExpValidator{regExp: /[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}/} //RegExpValidator{regExp: /[0-5][0-9][0-5][.][0-9]/}
            onTextChanged: {
                signalgetinputtext(input.text)
            }
        }
    }
}
