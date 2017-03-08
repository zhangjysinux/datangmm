import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Dialogs 1.0
// 主界面
Item {
    width: parent.width
    height: parent.height
    Connections {
        target: realtimeVideo
        onSignalStartVideoCall: {
            pageStack.push("qrc:/qml/RealtimeVideoPage.qml")
            waitconfirm.visible = false
            airMessageBoxId.visible = false
        }
    }
    Connections {
        target: realtimeVideo
        onSignalCloseCallInComing: {
            airMessageBoxId.visible = false
        }
    }
    Connections {
        target: realtimeVideo
        onSignalCallInComing: {
            //pageStack.push("qrc:/qml/RealtimeVideoPage.qml")
            airMessageBoxId.visible = true
//            "headname":keyhead,"headvalue":value
            }
        }
        property string ip
        property bool waitVisable
        property bool incomintVisable
        Image {
            anchors.fill: parent
            fillMode: Image.Tile
            source: "qrc:/res/bg.png"
        }

        Image {
            anchors.fill: parent
            source: "qrc:/res/bgshadow.png"
        }

        MessageBox{
            id:airMessageBoxId
            x:500;y:300
            visible: realtimeVideo.getIsComming()
            incomingip:realtimeVideo.callIP
        }
        WaitConfirm{
            //anchors.fill: parent
            id:waitconfirm
            x:500
            y:300
            ipview:ip
            visible:waitVisable
        }
        Grid {
            anchors.centerIn: parent
            rows: 2
            columns: 2
            rowSpacing: 65
            columnSpacing: 65

            MainButton {
                iconSource: "qrc:/res/realtimesend.png"
                onClicked: {
//                    pageStack.push("qrc:/qml/PersonList.qml",{parentSource:talking})
                    pageStack.push("qrc:/qml/PersonList.qml",{type:0})
                }
            }

            MainButton {
                iconSource: "qrc:/res/filesend.png"
                onClicked: {
                    pageStack.push("qrc:/qml/SelectContactsPage.qml");
                }
            }

            MainButton {
                iconSource: "qrc:/res/videorecording.png"
                onClicked: {
                    pageStack.push("qrc:/qml/VideoRecordPage.qml");
                }
            }

            MainButton {
                iconSource: "qrc:/res/videoview.png"
                onClicked: {
                    pageStack.push("qrc:/qml/VideoViewPage.qml",{type:0});
                }
            }
        }
    }
