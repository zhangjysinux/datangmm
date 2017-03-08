import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: laserdata
    property string theid: ""
    Connections {
        target: myLaser
        onSignalOneLaserData: {
            listmodel.append({"headname":keyhead,"headvalue":value})
        }
    }
    contentAreaItem: Item{
        anchors.fill: parent
        CTitleBar {
            id: titlebar
            anchors.top: parent.top
            width: parent.width
            height: 96
            leftItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemComponent: CButton {
                id: leftTitle
                anchors.fill: parent
                iconSource: pressed ? "qrc:/images/images/back_click.png" : "qrc:/images/images/back.png"
                backgroundEnabled: false
                onClicked: {
                    pageStack.pop()
                }
            }
            backgroundEnabled: true
            backgroundComponent: Component {
                Image {
                    anchors.fill: parent
                    source: "qrc:/images/images/top_line.png"
                }
            }

        }
        Item {
            id: mid
            anchors.fill: parent
            Image {
                sourceSize.height: 4
                sourceSize.width: 4
                fillMode: Image.Tile
                anchors.fill: parent
                source: "qrc:/images/images/back_mid_up.png"
            }
            Image {
                anchors.fill: parent
                source: "qrc:/images/images/back_mid.png"
            }
        }
        ListModel {
            id: listmodel
        }
        ListView {
            id: listview
            anchors.top: titlebar.bottom
            anchors.left: parent.left
            anchors.leftMargin: 48
            anchors.bottom: parent.bottom
            width: parent.width - 96
            model: listmodel
            delegate: Component {
                Item {
                    width: listview.width
                    height: 130
                    Text {
                        id: thename
                        anchors.top: parent.top
                        anchors.topMargin: 24
                        anchors.left: parent.left
                        width: 200
                        height: 33
                        text: headname
                        color: "#b6b6b6"
                        font.pixelSize: 30
                    }
                    Text {
                        id: thevalue
                        anchors.top: thename.bottom
                        anchors.topMargin: 16
                        anchors.left: parent.left
                        width: 300
                        height: 33
                        text: headvalue
                        clip: true
                        color: "white"
                        font.pixelSize: 30
                    }
                    CLine {
                        id: line
                        anchors.top: thevalue.bottom
                        anchors.topMargin: 24
                        anchors.left: parent.left
                        width: parent.width
                        color: "#9b9b9b"
                    }
                }
            }
            Component.onCompleted: {
                myLaser.getLaserRang(theid);
            }
        }
    }
}
