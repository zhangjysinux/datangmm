import QtQuick 2.0
Rectangle {
    id: messagewaitconfirm
    //visible: false
    anchors.fill: parent
    color: "#80000000"
    property int fontSize : 0.03 * parent.height
    property  string ipview
    //对话框
    Rectangle {
        id: messageBox
        color: "#4A4A4A"
        width: 0.7 * parent.width
        height: 0.2 * parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        //anchors.fill: parent
        Text
        {
            id:textText
            width: parent.width
            height:0.7 * parent.height
            horizontalAlignment:Text.AlignHCenter

            verticalAlignment:Text.AlignVCenter
            font.pixelSize: fontSize
            wrapMode:Text.WordWrap
            text:"等待"+ipview+"确认....."
            color: "white"
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {

        }
        onPressedChanged: {

        }
    }
    Component.onCompleted:
    {
        var docRoot = messagewaitconfirm.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        messagewaitconfirm.parent = docRoot;
    }

}
