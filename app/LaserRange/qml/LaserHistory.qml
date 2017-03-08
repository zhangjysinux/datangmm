import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: laserhistory
    Connections {
        target: myLaser
        onSignalLaserHistory: {
            listmodel.append({"id":id,"timeinfo":time,"loninfo":lon,"latinfo":lat})
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
            anchors.bottomMargin: 10
            width: parent.width - 96
            model: listmodel
            delegate: Component {
                Item {
                    width: listview.width
                    height: 180
                    Text {
                        id: thetime
                        anchors.top: parent.top
                        anchors.topMargin: 24
                        anchors.left: parent.left
                        width: 300
                        height: 33
                        text: timeinfo
                        color: "#b6b6b6"
                        font.pixelSize: 30
                    }
                    Text {
                        id: thelon
                        anchors.top: thetime.bottom
                        anchors.topMargin: 16
                        anchors.left: parent.left
                        width: 300
                        height: 33
                        text: qsTr("经度:") + loninfo
                        clip: true
                        color: "white"
                        font.pixelSize: 30
                    }
                    Text {
                        id: thelat
                        anchors.top: thelon.bottom
                        anchors.topMargin: 16
                        anchors.left: parent.left
                        width: 300
                        height: 33
                        text: qsTr("纬度:") + latinfo
                        clip: true
                        color: "white"
                        font.pixelSize: 30
                    }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            clickeffects.opacity = 0.5
                        }
                        onReleased: {
                            clickeffects.opacity = 0
                            pageStack.push("qrc:/qml/LaserData.qml",{"theid":listmodel.get(index).id})
                        }
                        onPositionChanged: {
                            clickeffects.opacity = 0
                        }
                    }
                    CLine {
                        id: line
                        anchors.top: thelat.bottom
                        anchors.topMargin: 22
                        anchors.left: parent.left
                        width: parent.width
                        color: "#9b9b9b"
                    }
                    Rectangle {
                        id: clickeffects
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: parent.height
                        opacity: 0
                        color: "black"
                    }
                }
            }
            Component.onCompleted: {
                myLaser.getLaserList();
            }
        }
    }
}
