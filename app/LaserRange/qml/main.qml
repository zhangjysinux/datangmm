import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        id: main
        Component.onCompleted:	{
            gScreenInfo.setStatusBar(true);
            gScreenInfo.setStatusBarStyle("black");
        }

        contentAreaItem: Item {
            anchors.fill: parent
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
            Image {
                id: laser
                anchors.top: parent.top
                anchors.topMargin: 100
                anchors.horizontalCenter: parent.horizontalCenter
                height: 92
                source: "qrc:/images/images/laser.png"
            }
            Image {
                id: laserstart
                anchors.top: laser.bottom
                anchors.topMargin: 100
                anchors.horizontalCenter: parent.horizontalCenter
                height: 262
                source: "qrc:/images/images/laserstart.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        timecount.running = true
                        lasering.visible = true
                    }
                }
            }
            Image {
                id: laserhistory
                anchors.top: laserstart.bottom
                anchors.topMargin: 110
                anchors.horizontalCenter: parent.horizontalCenter
                height: 262
                source: "qrc:/images/images/laserhistory.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        pageStack.push("qrc:/qml/LaserHistory.qml")
                    }
                }
            }
            Rectangle {
                id:lasering
                anchors.fill: parent
                visible: false
                color: Qt.rgba(0,0,0,0.5)
                AnimatedImage {
                    anchors.centerIn: parent
                    source: "qrc:/images/images/laser.gif"
                }
            }
            Timer {
                id: timecount
                interval: 3000
                onTriggered: {
                    if (timecount.running == false) {
                        lasering.visible = false
                        pageStack.push("qrc:/qml/LaserData.qml",{"theid":myLaser.startAddLaserRangData()})
                    }
                }
            }
        }
    }
}
