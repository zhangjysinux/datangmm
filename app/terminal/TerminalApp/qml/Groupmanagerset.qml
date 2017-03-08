import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage{
    id: groupmanagerset
    property bool bIschange: false/*是否横屏*/
    property string groupid: ""
    property string groupname: ""
    property bool bIsFirst: false/*是否首次*/
    property bool bIsFinish: true/*是否变更完成*/

    function groupManagerSetStart()
    {
        if (bIsFirst) {
            bIsFirst = false
        }
        listview.editing = true
    }

    Connections {
        target: getData
        onSignalSendGroupLeaders: {
            listLeaderModel.append({"id":id,"nameinfo":name,"positioninfo":position})
        }
        onSignalSendGroupMembers: {
            listModel.append({"id":id,"nameinfo":name,"positioninfo":position})
        }
    }

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        bIsFirst = true
        groupmanagerset.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            groupmanagerset.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            bIschange = true
        } else {
            groupmanagerset.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            bIsChange = false
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                groupmanagerset.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                bIschange = true
            } else {
                groupmanagerset.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                bIschange = false
            }
        }
    }
    contentAreaItem: Item {
        anchors.fill: parent
        CEditTitleBar {
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            rightItemEnabled: true
            rightItemBackgroundEnabled: true
            rightItemSize: Qt.size(73,73)
            rightItemComponent: Component {
                id: rightTitle
                CButton {
                    anchors.fill: parent
                    pixelSize: 30
                    textColor: "#29afe2"
                    backgroundEnabled: false
                    text: qsTr("确认")
                    //iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                    //spacing: -68
                    onClicked: {
                        getData.clearGroupLeaders()
                        listLeaderModel.remove(listLeaderModel.count-1,1)
                        for (var i = 0; i < listLeaderModel.count; i++) {
                            getData.setGroupLeaders(listLeaderModel.get(i).id)
                        }
                        getData.clearGroupMembers()
                        for (var i = 0; i < listModel.count; i++) {
                            getData.setGroupMembers(listModel.get(i).id)
                        }
                        getData.updateGroup(groupid,groupname)
                        pageStack.pop()
                    }
                }
            }
            leftItemEnabled: true
            leftItemBackgroundEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemComponent: Component {
                id: leftTitle
                CButton {
                    anchors.fill: parent
                    iconSource: "qrc:/main/images/back.png"
                    text: "返回"
                    textColor: "#29afe2"
                    backgroundEnabled: false
                    onClicked: {
                        deleteback.visible = true
                        deleteback.opacity = 0.5
                        dialog.visible = true
                    }
                }
            }
            editView: listview
            editLeftItemLeftMargin: 2
            editLeftComponent: CButton {
                iconSource: "qrc:/main/images/back.png"
                text: "返回"
                textColor: "#29afe2"
                backgroundEnabled: false
                onClicked: {
                    for (var i = 0; i < listview.selectedCount; i++) {
                        listLeaderModel.remove(listLeaderModel.count-2,1)
                    }
                    bIsFinish = false
                    listview.editing = false
                    bIsFinish = true
                }
            }
            editRightComponent: CButton {
                id: rightCheck
                property bool checked: false
                anchors.fill: parent
                pixelSize: 30
                textColor: "#29afe2"
                backgroundEnabled: false
                text: "确认"
                //iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                //spacing: -68
                onClicked: {
                    bIsFinish = false
                    var indexs = listview.selectedIndexes
                    for (var i = indexs.length - 1; i >= 0; i--) {
                        listModel.remove(indexs[i],1)
                    }
                    listview.editing = false
                    bIsFinish = true
                }
            }
            spacingBetweenLeftItemAndTitleItem: 16
            spacingBetweenLeftBorderAndLeftItem: 16
            titleAreaCentered: true
            titlePixelSize: 30
            titleTextColor: "#29afe2"
            titleText: qsTr("设置管理员")
            backgroundEnabled: true
//            backgroundComponent: Component{
//                Image {
//                    anchors.fill: parent
//                    source: "qrc:/main/images/top_line.png"
//                }
//            }
        }
//        Item{
//            id: mid
//            width: parent.width
//            anchors.top: titleBar.bottom
//            anchors.bottom: parent.bottom
//            Image {
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid.png"
//            }
//            Image {
//                fillMode: Image.Tile
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid_up.png"
//            }
//        }
        Component {
            id: groupLsit
            Item {
                height: 133
                width: bIsChange ? parent.width - 106 : parent.width - 120
                Text {
                    id: personname
                    anchors.top: parent.top
                    anchors.topMargin: 22
                    anchors.left: line.left
                    width: 300
                    height: 32
                    font.pixelSize: 29
                    color: "white"
                    elide: Text.ElideRight
                    text: nameinfo
                    onTextChanged: {
                        if (nameinfo == "添加") {
                            editAdd.visible = true
                            line.anchors.top = personname.bottom
                        } else {
                            positionname.visible = true
                            editDelete.visible = true
                        }
                    }
                }
                Text {
                    id: positionname
                    anchors.top: personname.bottom
                    anchors.topMargin: 24
                    anchors.left: line.left
                    width: 300
                    height: 30
                    visible: false
                    font.pixelSize: 30
                    color: "white"
                    elide: Text.ElideRight
                    text: positioninfo
                }
                CLine {
                    id:line
                    anchors.top: positionname.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 74
                    width: parent.width
                    color: "#36465a"
                }
                CButton {
                    id: editDelete
                    anchors.right: line.left
                    anchors.rightMargin: 24
                    anchors.top: personname.top
                    anchors.topMargin: 20
                    width: 50
                    height: 50
                    visible: false
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/edit_delete.png"
                    onClicked: {
                        listModel.append({"id":id,"nameinfo":nameinfo,"positioninfo":positioninfo})
                        listLeaderModel.remove(index,1)
                    }
                }
                CButton {
                    id: editAdd
                    anchors.right: line.left
                    anchors.rightMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 18
                    width: 50
                    height: 50
                    visible: false
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/edit_add_s.png"
                    onClicked: {
                        groupManagerSetStart()
                    }
                }
            }
        }
        Component {
            id: listmanager
            CEditListViewDelegate {
                id:checkImage
                height: 133
                width: listview.width
                editLabelRightMargin: bIsChange ? 1182 : 622
                editLabelSize: Qt.size(50,50)
                editLabelComponent: Image {
                    width: 50
                    height: 50
                    source: (checkImage.checked) ? "qrc:/main/images/checkTrue.png" : "qrc:/main/images/checkFalse.png"
                }

                onCheckedChanged: {
                    if (bIsFinish) {
                        if (listview.isSelected(index)) {
                            for (var i = 0;i < listLeaderModel.count; i++)
                            {
                                if (listLeaderModel.get(i).id == listModel.get(index).id)
                                    return;
                            }
                            listLeaderModel.insert(listLeaderModel.count-1,{"id":listModel.get(index).id,
                                                       "nameinfo":listModel.get(index).nameinfo,
                                                       "positioninfo":listModel.get(index).positioninfo})
                        } else {
                            for (var i = 0; i < listLeaderModel.count - 1; i++) {
                                if (listLeaderModel.get(i).id == listModel.get(index).id) {
                                    listLeaderModel.remove(i,1)
                                }
                            }
                        }
                    }
                }
                Item {
                    id: temp
                    height: 133
                    width: bIsChange ? parent.width - 106 : parent.width - 120
                    Text {
                        id: personname
                        anchors.top: parent.top
                        anchors.topMargin: 22
                        anchors.left: line.left
                        width: 300
                        height: 32
                        font.pixelSize: 28
                        color: "white"
                        elide: Text.ElideRight
                        text: nameinfo
                    }
                    Text {
                        id: positionname
                        anchors.top: personname.bottom
                        anchors.topMargin: 24
                        anchors.left: line.left
                        width: 300
                        height: 30
                        font.pixelSize: 28
                        color: "white"
                        elide: Text.ElideRight
                        text: positioninfo
                    }
                    CLine {
                        id:line
                        anchors.top: positionname.bottom
                        anchors.topMargin: 24
                        anchors.left: parent.left
                        anchors.leftMargin: 74
                        width: parent.width
                        color: "#36465a"
                    }
                }
            }
        }
        ListModel {
            id: listModel
        }
        ListModel {
            id: listLeaderModel
        }
        CEditListView {
            id: listview
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.leftMargin: 24
            width: parent.width - 48
            anchors.bottom: parent.bottom
            model: listview.editing ? listModel : listLeaderModel
            clip: true
            delegate: listview.editing ? listmanager : groupLsit
            Component.onCompleted: {
                getData.getGroupLeaders(groupid)
                getData.getGroupMembers(groupid)
                listLeaderModel.append({"id":"","nameinfo":"添加","positioninfo":""})
            }
        }
    }
    Rectangle {
        id: deleteback
        anchors.fill: parent
        color: "black"
        visible: false
        opacity: 0
        z: 4
        MouseArea {
            anchors.fill: parent
            onClicked: {
                deleteback.visible = false
                dialog.visible = false
                deleteback.opacity = 0
            }
        }
    }
    Rectangle {
        id: dialog
        visible: false
        anchors.centerIn: parent
        width: 504
        height: 296
        z: 4
        radius: 10
        Text {
            id: message
            anchors.top: parent.top
            anchors.topMargin: 79
            height: 33
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            color: "black"
            font.pixelSize: 32
            wrapMode: Text.Wrap
            lineHeight: 1.2
            text: qsTr("是否保存当前管理员的\n设置?")
        }
        CButton {
            id: deletecancel
            anchors.top: message.bottom
            anchors.topMargin: 69
            anchors.left: parent.left
            anchors.leftMargin: 62
            backgroundEnabled: true
            text: "取消"
            textColor: "#ffffff"
            backgroundComponent: Rectangle {
                anchors.fill: parent
                color: "#29afe2"
                radius: 10
            }
            width: 150
            height: 67

            onClicked: {
                dialog.visible = false
                deleteback.visible = false
                deleteback.opacity = 0
            }
        }
        CButton {
            id: deletesure
            anchors.top: message.bottom
            anchors.topMargin: 69
            anchors.left: deletecancel.right
            anchors.leftMargin: 80
            width: 150
            height: 67
            backgroundEnabled: true
            text: "确认"
            textColor: "#ffffff"
            backgroundComponent: Rectangle {
                anchors.fill: parent
                color: "#29afe2"
                radius: 10
            }
            onClicked: {
                getData.clearGroupLeaders()
                listLeaderModel.remove(listLeaderModel.count-1,1)
                for (var i = 0; i < listLeaderModel.count; i++) {
                    getData.setGroupLeaders(listLeaderModel.get(i).id)
                }
                getData.clearGroupMembers()
                for (var i = 0; i < listModel.count; i++) {
                    getData.setGroupMembers(listModel.get(i).id)
                }
                getData.updateGroup(groupid,groupname)
                pageStack.pop()
                dialog.visible = false
                deleteback.opacity = 0
                deleteback.visible = false
            }
        }
    }
}
