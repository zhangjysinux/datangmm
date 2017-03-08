import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: personLable
    property var containerqml: null
    property int selectenum : -1
    property string networkInfo: ""
    property string networkname: ""
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            personLable.orientationPolicy = CPageOrientation.LockLandscape
        personLable.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personLable.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            personLable.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }

    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personLable.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                personLable.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }
    Connections {
        target: getData
        onSignalSendNetworkInfo: {
            listModel.append({"name":netinfo})
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
            rightItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemComponent: Component {
                id: leftTitleBar
                CButton {
                    id: leftButton
                    anchors.fill: parent
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                    onClicked: {
                        pageStack.pop()
                    }
                }
            }
            rightItemSize: Qt.size(74,74)
            rightItemComponent: Component {
                id: rightTitleBar
                CButton {
                    anchors.fill: parent
                    pixelSize: 30
                    textColor: "white"
                    backgroundEnabled: false
                    text: qsTr("确认")
                    iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                    spacing: -68
                    onClicked: {
                        console.log("sure")
                        if (networkInfo != "") {
                            containerqml.addNetworkType(networkInfo)
                            pageStack.pop()
                        }
                    }
                }
            }
            titleText: qsTr("标签")
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
                height: 78
                Text {
                    id: text1
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    width: parent.width
                    height: 30
                    color: "white"
                    font.pixelSize: 30
                    text: name
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var bIsCurrentNet = false
                        for (var i = 0; i < listModel.count; i++) {
                            if (listModel.get(i).name == listmodel.get(index).name)
                                bIsCurrentNet = true
                        }
                        if (!bIsCurrentNet) {
                            if (selectenum == -1) {
                                selectenum = index
                                listmodel.set(selectenum,{"opacitynum":"0.3"})
                            } else {
                                listmodel.set(selectenum,{"opacitynum":"0"})
                                listmodel.set(index,{"opacitynum":"0.3"})
                                selectenum = index
                            }
                            networkInfo = text1.text
                        }
                    }
                }
                Rectangle {
                    id: rec
                    width: parent.width
                    height: parent.height
                    opacity: opacitynum
                }
                CLine {
                    id: line
                    anchors.top: text1.bottom
                    anchors.topMargin: 24
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#9b9b9b"
                }
            }
        }
        ListModel {
            id: listmodel
        }
        ListModel {
            id: listModel
        }
        ListView {
            id: listview
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            anchors.left: parent.left
            anchors.leftMargin: 100
            anchors.bottom: parent.bottom
            width: parent.width - 148
            interactive: false
            delegate:buttonAndLine
            model: listmodel
            Component.onCompleted: {
                listmodel.append({"name":"自组网","opacitynum":"0","isExist":"0"})
                listmodel.append({"name":"天通网","opacitynum":"0","isExist":"0"})
                listmodel.append({"name":"战互网","opacitynum":"0","isExist":"0"})
                listmodel.append({"name":"模块数字对讲","opacitynum":"0","isExist":"0"})
                listmodel.append({"name":"LTE","opacitynum":"0","isExist":"0"})
                listmodel.append({"name":"WIFI","opacitynum":"0","isExist":"0"})
            }
        }
    }
}
