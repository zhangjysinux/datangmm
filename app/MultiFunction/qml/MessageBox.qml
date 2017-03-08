import QtQuick 2.0
Rectangle {
    id: messageBox
    property string prompt:"是否查看发送的实时多功能战场信息"
    property int fontSize : 0.02 * parent.height
    property string incomingip
    visible: false
    anchors.fill: parent
    color: "#80000000"

    //对话框
    Rectangle {
        id: messageBoxd
        color: "white"
        width: 0.7 * parent.width
        height: 0.2 * parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Column
        {
            anchors.fill: parent
            Text
            {
                id:textText
                width: parent.width
                height:0.7 * parent.height
                horizontalAlignment:Text.AlignHCenter

                verticalAlignment:Text.AlignVCenter
                font.pixelSize: fontSize
                wrapMode:Text.WordWrap
                text:"是否查看"+incomingip+"发送的实时多功能战场信息"
                color: "black"
            }

            Row
            {
                width:parent.width
                height:0.2*parent.height
                Rectangle
                {
                    width: 0.15 * parent.width
                    height:parent.height
                }
                Rectangle
                {
                    id: cancl
                    width: 0.2*parent.width
                    height:parent.height
                    color: "#3399db"
                    radius: 20
                    Text
                    {
                        anchors.fill: parent
                        horizontalAlignment:Text.AlignHCenter
                        verticalAlignment:Text.AlignVCenter
                        font.pixelSize: fontSize
                        text:"取消"
                    }
                    MouseArea
                    {
                        anchors.fill: parent//
                        onClicked:
                        {
                            messageBox.visible = false;
                            realtimeVideo.callRefuse();
                        }
                    }
                }
                Rectangle
                {
                    width: 0.3 * parent.width
                    height:parent.height
                }
                Rectangle
                {
                    id:ok
                    width: 0.2*parent.width
                    height:parent.height
                    color: "#3399db"
                    radius: 20
                    Text
                    {
                        anchors.fill: parent
                        horizontalAlignment:Text.AlignHCenter
                        verticalAlignment:Text.AlignVCenter
                        font.pixelSize: fontSize
                        text:"确定"

                    }
                    MouseArea
                    {
                        anchors.fill: parent//
                        onClicked:
                        {
                            messageBox.visible = false;
                            realtimeVideo.callAccept();
                            pageStack.push("qrc:/qml/RealtimeVideoPage.qml");
                        }
                    }
                }
                Rectangle
                {
                    width: 0.05 * parent.width
                    height:parent.height
                }
            }
            Rectangle
            {
                width: parent.width
                height:0.1 * parent.height
            }
        }
    }

    //    states: State
    //    {
    //        name: "visible"
    //        PropertyChanges {target: messageBox; visible: true}
    //    }
    //    transitions: Transition
    //    {
    //        NumberAnimation
    //        {
    //            target:  messageBox
    //            properties: "visible"
    //            duration: 250
    //        }
    //    }
    Component.onCompleted:
    {
        var docRoot = messageBox.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        messageBox.parent = docRoot;
    }
}
