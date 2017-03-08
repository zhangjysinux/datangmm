import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: personInfoSearch
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        personInfoSearch.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personInfoSearch.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            personInfoSearch.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personInfoSearch.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                personInfoSearch.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }

    contentAreaItem: Item {
        anchors.fill: parent
        CTitleBar {
            id: titlebar
            anchors.top: parent.top
            width: parent.width
            height: 92
            leftItemEnabled: true
            rightItemEnabled: false
            leftItemSize: Qt.size(93,74)
            leftItemComponent: Component {
                id: leftTitleBar
                CButton {
                    anchors.fill: parent
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                    onClicked: {
                        console.log("back press")
                        pageStack.pop()
                    }
                }
            }
            titleText: qsTr("状态查询")
            titlePixelSize: 30
            titleTextColor: "#29afe2"
            titleAreaCentered: true
            spacingBetweenLeftBorderAndLeftItem: 16
            anchors.horizontalCenter: parent.horizontalCenter
            backgroundEnabled: true
            backgroundComponent: Component {
                Image {
                    anchors.fill: parent
                    source: "qrc:/main/images/top_line.png"
                }
            }
        }
        Item {
            id: mid
            anchors.top: titlebar.bottom
            anchors.bottom: parent.bottom
            width: parent.width
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
            id: infoText
            Item {
                id: itemname
                height: 125
                width: listview.width
                Text {
                    id: previousText
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    horizontalAlignment: Qt.AlignLeft
                    text: name
                    color: "#ffffff"
                    font.pixelSize: 30
                    onTextChanged: {
                        if (name == "在网情况" || name == "位置信息") {
                            previousText.color = "#29afe2"
                            itemname.height = 55
                        } else if (name == "经度" || name == "纬度") {
                            network.visible = false
                            itemname.height = 125
                            nextText.visible = true
                            line.visible = true
                        }else {
                            itemname.height = 125
                            nextText.visible = true
                            network.visible = true
                            line.visible = true
                        }
                    }
                }
                Image {
                    id: network
                    anchors.top: previousText.top
                    anchors.topMargin: 4
                    anchors.left: previousText.right
                    anchors.leftMargin: 16
                    visible: false
                    source: info == "在线" ? "qrc:/main/images/online.png" : "qrc:/main/images/missline.png"
                }
                Text {
                    id: nextText
                    anchors.top: previousText.bottom
                    anchors.topMargin: 16
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignVCenter
                    text: info
                    visible: false
                    color: "#ffffff"
                    font.pixelSize: 30
                }
                CLine {
                    id: line
                    anchors.top: nextText.bottom
                    anchors.topMargin: 24
                    width: parent.width - 48
                    color: "#36465a"
                    visible: false
                }
            }
        }
        ListModel {
            id: listmodel
        }
        ListView {
            id: listview
            anchors.top: titlebar.bottom
            anchors.topMargin: 24
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 48
            width: parent.width - 48
            clip: true
            model: listmodel
            delegate: infoText
            Component.onCompleted: {
                listmodel.append({"name":"在网情况","info":""})
                listmodel.append({"name":"自组网","info":"在线"})
                listmodel.append({"name":"天通网","info":"在线"})
                listmodel.append({"name":"战互网","info":"离线"})
                listmodel.append({"name":"LTE","info":"离线"})
                listmodel.append({"name":"模块数字对讲","info":"离线"})
                listmodel.append({"name":"位置信息","info":""})
                listmodel.append({"name":"经度","info":"124.52"})
                listmodel.append({"name":"纬度","info":"89.24"})
            }
        }
    }
}
