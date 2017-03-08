import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: personSettingMemory
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            personSettingMemory.orientationPolicy = CPageOrientation.LockLandscape
        personSettingMemory.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personSettingMemory.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            personSettingMemory.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personSettingMemory.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                personSettingMemory.statusBarHoldEnabled = true
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
            rightItemEnabled: false
            leftItemSize: Qt.size(93,74)
            leftItemBackgroundEnabled: true
            leftItemComponent: Component {
                id: leftTitleBar
                CButton {
                    anchors.fill: parent
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                    onClicked: {
                        console.log("back pre page")
                        pageStack.pop()
                    }
                }
            }
            titleText: qsTr("存储情况")
            titlePixelSize: 30
            titleTextColor: "white"
            titleAreaCentered: true
            spacingBetweenLeftBorderAndLeftItem: 16
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
            id: memoryinfo
            Item {
                property alias textname: textName.text
                property alias textnum: textNum.text
                anchors.fill: parent
                Text {
                    id: textName
                    anchors.top: parent.top
                    anchors.left: parent.left
                    width: parent.width
                    text: ""
                    color: "#b6b6b6"
                    font.pixelSize: 30
                }
                Text {
                    id: textNum
                    anchors.top: textName.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    width: parent.width
                    text: ""
                    color: "#ffffff"
                    font.pixelSize: 30
                }
                CLine {
                    anchors.top: textNum.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    width: parent.width - 48
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#9b9b9b"
                }
            }
        }

        Column {
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            anchors.left: parent.left
            anchors.leftMargin: 100
            anchors.bottom: parent.bottom
            width: parent.width - 148
            spacing: 24
            Loader {
                id: siminfo
                width: parent.width
                height: 124
                sourceComponent: memoryinfo
                onLoaded: {
                    item.textname = "手机"
                    item.textnum = getData.getContacterNum() + "/2000"
                }
            }
        }
    }
}
