import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: personInfoMoreInfo
    property int selectenum : -1
    property var filesize: null
    property var fileurl: null
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        personInfoMoreInfo.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personInfoMoreInfo.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            personInfoMoreInfo.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personInfoMoreInfo.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                personInfoMoreInfo.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }

    contentAreaItem: Item {
        anchors.fill: parent
        CEditTitleBar{
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            leftItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemBackgroundEnabled: true
            leftItemComponent: Component {
                id: leftTitleBar
                CButton {
                    anchors.fill: parent
                    iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                    backgroundEnabled: false
                    onClicked: {
                        console.log("back pre page")
                        pageStack.pop()
                    }
                }
            }
            titleText: qsTr("传输文件")
            titlePixelSize: 30
            titleTextColor: "white"
            titleAreaCentered: true
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
            anchors.horizontalCenter: parent.horizontalCenter
            backgroundEnabled: true
            backgroundComponent: Component{
                Image {
                    anchors.fill: parent
                    source: "qrc:/main/images/top_line.png"
                }
            }
        }

        Item{
            id: mid
            width: parent.width
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
            Image {
                fillMode: Image.Tile
                anchors.fill: parent
                source: "qrc:/main/images/back_mid_up.png"
            }
            Image {
                anchors.fill: parent
                source: "qrc:/main/images/back_mid.png"
            }
        }
        Component {
            id: buttonAndLine
            Item {
                width: listview.width
                height: 89
                Image {
                    id: fileicon
                    source: "qrc:/main/images/fileicon.png"
                }
                Text {
                    id: filename
                    anchors.left: fileicon.right
                    anchors.leftMargin: 24
                    width: 206
                    height: 40
                    horizontalAlignment: Qt.AlignLeft
                    color: "#ffffff"
                    text: "nameinfo"
                    font.pixelSize: 40
                    elide: Text.ElideRight
                }
                CProgressBar {
                    id: progress
                    height: 8
                    width: 244
//                    anchors.right: parent.right
//                    anchors.rightMargin: 40
                    anchors.left: filename.right
                    anchors.leftMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    value: 20
                    minimum: 0
                    maximum: 100
                    anchors.verticalCenterOffset: -10
                    progressColor: "green"
                    backgroundComponent: Item{
                        Image {
                            id: back
                            source: "qrc:/main/images/fileprogress.png"
                        }
                    }
                }
                CButton {
                    id: filestart
                    width: 55;height: 55
                    anchors.left: progress.right
                    iconSource: "qrc:/main/images/filestart.png"
                    backgroundEnabled: false
                }
                CButton {
                    id: filesdelete
                    width: 55;height: 55
                    anchors.left: filestart.right
                    iconSource: "qrc:/main/images/filedelete.png"
                    backgroundEnabled: false
                }

                CLine {
                    id: line
                    anchors.top: fileicon.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#9b9b9b"
                }
                Connections{
                    target: gScreenInfo
                    onCurrentOrientationChanged:{
                        if (gScreenInfo.currentOrientation == 2 ||
                                gScreenInfo.currentOrientation == 8) {
//                            filename.width = filename.width + 310
//                            progress.width = progress.width + 250
                        } else {
//                            filename.width = filename.width - 310
//                            progress.width = progress.width - 250
                        }
                    }
                }

            }
        }

        ListModel {
            id: listmodel
        }
        ListView {
            id: listview
            anchors.top: titleBar.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 32
            width: parent.width - 64
            anchors.bottom: parent.bottom
            clip: true
            model: 3
            delegate: buttonAndLine
            Component.onCompleted: {

            }
        }
        Rectangle {
            id: sureback
            anchors.fill: parent
            visible: false
            color: "black"
            opacity: 0
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    sureback.visible = false
                    sureback.opacity = 0
                    suredialog.visible = false
                }
            }
        }
        Rectangle {
            id: suredialog
            visible: false
            anchors.centerIn: parent
            width: 492
            height: 386
            Image {
                anchors.fill: parent
                source: "qrc:/bluetooth/images/bluetoothblack.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        suredialog.visible = false
                    }
                }
            }
            Text {
                id: message
                anchors.centerIn: parent
                height: 80
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: 40
                text: qsTr("传输失败传输成功")
            }
        }
    }
}
