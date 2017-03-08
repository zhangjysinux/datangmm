import QtQuick 2.0
import com.syberos.basewidgets 2.0

Item {
    id: searchLine
    anchors.top:parent.top
    width: parent.width; height: 90
    signal signalgetinputtext(string text)
    property alias inputtext: input
    Rectangle {
        radius: 30
        color: "#ffffff"
        visible: true//!input.activeFocus && !input.text.length
        anchors.centerIn: parent
        width: parent.width - 32; height: 72
        Image {
            id: searchIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 36
            source: "qrc:/main/images/search_icon.png"
            visible: !input.activeFocus && !input.text.length
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: searchIcon.right
            anchors.leftMargin: 16
            font.pixelSize: 26
            color: "#d2d2d2"
            visible: !input.activeFocus && !input.text.length
            text: qsTr("搜索")
        }
    }
    TextInput {
        id: input
        anchors.fill: parent
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
        clip: true
        color: "black"
        font.pixelSize: 32
        //validator: RegExpValidator{regExp: /[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}/} //RegExpValidator{regExp: /[0-5][0-9][0-5][.][0-9]/}
        onTextChanged: {
            signalgetinputtext(input.text)
        }
    }

}
