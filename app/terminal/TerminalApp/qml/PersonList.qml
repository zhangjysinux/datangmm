import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: groupAdd
    property string networkInfo: ""
    property string inputName: ""
    property var parentSource: null
    property var parentPage: null
    property var  searchText: ""
    property int netType: 0

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        groupAdd.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            groupAdd.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            sectionScroller.visible = false
        } else {
            groupAdd.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            sectionScroller.visible = true
        }

        if(netType != -1)
        {
            console.debug("getSearchContactors2---------" + netType)
            getData.getSearchContactors2(netType, 3, "")
        }
    }

    Component.onDestruction: {
        console.debug("Component onDestruction getSearchContactors---------")
        getData.getSearchContactors2(-1, 3, "")
    }

    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                groupAdd.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                sectionScroller.visible = false
            } else {
                groupAdd.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                sectionScroller.visible = true
            }
        }
    }

    function addNetworkType(network)
    {
        networkInfo = network
    }

    Connections {
        target: getData
        onSignalAddPersonInfo: {
            listmodel.append({"id":id, "name":name, "position":title, "size":namePinyin, "type":isNormal})
            listview.positionViewAtBeginning()
        }
    }
    contentAreaItem: Item {
        anchors.fill: parent
        CEditTitleBar {
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            leftItemEnabled: true
            leftItemSize: Qt.size(72,72)
            leftItemBackgroundEnabled: true
            leftItemComponent: leftTitle
            editView: listview
            editLeftItemLeftMargin: 2
            editLeftComponent: CButton {
                text: qsTr("取消")
                textColor: "white"
                backgroundEnabled: false
                onClicked: {
                    pageStack.pop()
                }
            }
            titlePixelSize: 40
            titleTextColor: "white"
            titleAreaCentered: true
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
                anchors.fill: parent
                source: "qrc:/main/images/back_mid.png"
            }
            Image {
                fillMode: Image.Tile
                anchors.fill: parent
                source: "qrc:/main/images/back_mid_up.png"
            }
        }
//                Loader {
//                    id:searchline
//                    anchors.top: titleBar.bottom
//                    width: parent.width
//                    height: 90
//                    source: "qrc:/qml/SearchLine.qml"
//                }

        SearchLine {
            id: searchline
            width: parent.width
            height: 90
            anchors.top: titleBar.bottom
            onSignalgetinputtext: {
                //listmodel.clear()
//                searchText = text
                getData.getSearchContactors2(netType, 3, text)
            }
        }
        ListModel {
            id: listmodel
        }
        CEditListView {
            id: listview
            anchors.top: searchline.bottom
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.bottom: parent.bottom
            width: parent.width - 16
            model: contactermodel
            clip: true
            visible: true
            editing: false
            delegate: CEditListViewDelegate {
                id: listText
                height: 142
                width: listview.width
                editLabelRightMargin: 646
                editLabelSize: Qt.size(50,50)
                editLabelComponent: Image {
                    id:checkImage
                    width: 50
                    height: 50
                    anchors.centerIn: parent
                    source: listText.checked ? "qrc:/main/images/checkTrue.png" : "qrc:/main/images/checkFalse.png"
                }
                Item {
                    id: item
                    height: 134
                    width: parent.width - 46
                    Image {
                        anchors.left: parent.left
                        //anchors.leftMargin: 98
                        height: 142
                        width: parent.width
                        source: "qrc:/main/images/list_line.png"
                        Text {
                            id: text1
                            anchors.top: parent.top
                            anchors.topMargin: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            width: 300
                            height: 40
                            font.pixelSize: 40
                            elide: Text.ElideRight
                            color: "white"
                            text: name
                        }
                        Text {
                            id: text2
                            anchors.top: text1.bottom
                            anchors.topMargin: 16
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            width: 200
                            height: 30
                            font.pixelSize: 30
                            color: "white"
                            text: type == 2 ? "" : position
                        }
                    }
                    MouseArea {
                        anchors.fill: item
                        onClicked: {
                            pageStack.pop(parentPage)
                            parentPage.contacterShareContacterSelected(id)
//                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":id,"contactertype":type,parentSource2:parentSource})
                        }
                    }
                }
            }
            section {
                property: "size";
                criteria: ViewSection.FirstCharacter
                delegate: sectionfill //searchText == "" ? sort : null
            }

            Component.onCompleted: {
                //getData.getAddPersonInfo("")
            }
        }
        MySectionScroller {
            id: sectionScroller
            listView: listview
            anchors.left: parent.left
            anchors.leftMargin: 8
            width: 30
            height: listView.height
        }
        Component {
            id: sectionfill
            Item {
                width: parent.width
                height: 62
                Rectangle {
                    width: parent.width
                    height: 62
                    color: Qt.rgba(0,0,0,0)
                    Text {
                        anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                        text: section; font.bold: true
                        font.pixelSize: 30
                        color: "#ffffff"
                    }
                }
            }
        }

        Component {
            id: leftTitle
            CButton {
                anchors.fill: parent
                pixelSize: 30
                textColor: "#ffffff"
                text: qsTr("返回")
                iconSource:"qrc:/main/images/back-pop.png"
                backgroundEnabled: false
                onClicked: {
                    pageStack.pop()
                }
            }
        }
    }

    Component {
        id: sort
        Rectangle {
            width: parent.width
            height: 84
            color: Qt.rgba(0,0,0,0)
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.top: parent.top
                anchors.topMargin: 24
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignLeft
                text: section
                font.bold: true
                font.pixelSize: 40
                color: "#ffffff"
            }
        }
    }
}
