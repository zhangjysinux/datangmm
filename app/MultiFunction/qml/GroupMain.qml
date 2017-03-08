import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id:groupmain
    property bool bIsConversation: true/*是否会话群*/
    Connections {
        target: getData
        onSignalGroupChanged: {
            if (type == 1) {/*add*/
                conversationmodel.append({"id":id,"groupName":name, "networkType":network, "personNum":num})
            }
            //            else if (type == 2) {/*delete*/
            //                for (var i = 0;i < conversationmodel.count;i++){
            //                    if (conversationmodel.get(i).id == id)
            //                    {
            //                        conversationmodel.remove(i,1);
            //                        break;
            //                    }
            //                }
            //            }
//            else if (type == 3) {/*update*/
//                for (var i = 0;i < conversationmodel.count;i++){
//                    if (conversationmodel.get(i).id == id)
//                    {
//                        conversationmodel.set(i,{"id":id,"groupName":name, "networkType":network, "personNum":num});
//                        break;
//                    }
//                }
//            }
        }
    }
    Connections {
        target: getData
        onSignalSendGroupInfoToGroup: {
            conversationmodel.append({"id":id, "groupName":name, "networkType":network, "personNum":num})
        }
    }
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
//getData.getGroups(false);
        groupmain.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
//            toolBar.direction = true
            groupmain.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
//            toolBar.direction = false
            groupmain.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }

    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
//                toolBar.direction = true
                groupmain.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
//                toolBar.direction = false
                groupmain.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
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
            leftItemEnabled: false//bIsConversation ? true : false
            leftItemBackgroundEnabled: true
            leftItemSize: Qt.size(74,74)
            leftItemComponent: leftTitle
            rightItemEnabled: false//bIsConversation ? true : false
            rightItemBackgroundEnabled: true
            rightItemSize: Qt.size(74,74)
            rightItemComponent: rightTitle
            titleItemEnabled: true
            titleAreaCentered: true
            titlePixelSize: 30
            titleTextColor: "white"
            titleItemComponent: title
            editView: listview
            editLeftItemLeftMargin: 2
            editLeftComponent: CButton {
                iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                backgroundEnabled: false
                onClicked: {
                    listview.deselect()
                    conversationmodel.clear()
                    groupSendModel.getGroups(false)
                    listview.editing = false
                    editToolBar.visible = false
//                    toolBar.visible = true
                }
            }
            editRightComponent: CButton {
                id: rightCheck
                property bool checked: false
                anchors.fill: parent
                pixelSize: 30
                textColor: "white"
                backgroundEnabled: false
                text: (listview.selectedCount == listview.count) ? "取消" : "全选"
                iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                spacing: -68
                onClicked: {
                    if (rightCheck.text == "全选"){
                        listview.selectAll()
                    } else {
                        listview.deselect()
                    }
                }
            }
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
//            Component.onCompleted: {
//                if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
//                    titleBar.rightItemEnabled = true
//                else
//                    titleBar.rightItemEnabled = false
//            }
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
            id: subordinate
            Item {
                width: listview.width
                height: 126
                Image {
                    id: backGround
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    anchors.left: parent.left
                    width: parent.width
                    height: 118
                    source: "qrc:/main/images/list_line.png"
                }
                Text {
                    id: postname
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    clip: true
                    width: 60
                    height: 40
                    text: postinfo
                    color: "white"
                    font.pixelSize: 36
                }
                Text {
                    id: personname
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: postname.right
                    anchors.leftMargin: 24
                    clip: true
                    width: 300
                    height: 40
                    text: nameinfo
                    color: "white"
                    font.pixelSize: 36
                    elide: Text.ElideRight
                }
                Image {
                    id: numicon
                    anchors.top: postname.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    width: 30
                    height: 30
                    source: "qrc:/group/images/PersonNum.png"
                }
                Text {
                    id: postnum
                    anchors.top: postname.bottom
                    anchors.topMargin: 16
                    anchors.left: numicon.right
                    anchors.leftMargin: 16
                    width: 100
                    height: 30
                    text: numinfo
                    color: "#b6b6b6"
                }
                MouseArea {
                    anchors.right: more.left
                    anchors.rightMargin: 50
                    anchors.left: parent.left
                    height: parent.height
                    onPressed: {
                        clickeffects.opacity = 0.5
                    }
                    onReleased: {
                        clickeffects.opacity = 0
                    }
                    onPositionChanged: {
                        clickeffects.opacity = 0
                    }
                }
                CButton{
                    id: more
                    visible: false
                    anchors.top: parent.top
                    anchors.topMargin: 48
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 100
                    height: 72
                    anchors.verticalCenter: parent.verticalCenter
                    iconSource: "qrc:/main/images/more.png"
                    backgroundEnabled: false
                    onClicked: {
                        pageStack.push("qrc:/qml/GroupInfo.qml",{"groupid":subordinatemodel.get(index).id,
                                           "groupname":subordinatemodel.get(index).postinfo,
                                           "groupnum":subordinatemodel.get(index).numinfo,
                                           "groupnet":subordinatemodel.get(index).netinfo,
                                           "qmlcontains":groupmain})
                    }
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
        Component {
            id: listText
            CEditListViewDelegate {
                id: checkIcon
                height: 172
                width: listview.width
                editLabelRightMargin: bIsChange ? 1198 : 638
                editLabelSize: Qt.size(50,50)
                editLabelComponent: Image {
                    id:checkImage
                    width: 50
                    height: 50
                    source: (checkIcon.checked) ? "qrc:/main/images/checkTrue.png" : "qrc:/main/images/checkFalse.png"
                }
                Item {
                    height: 172
                    width: listview.editing ? listview.width - 74 : listview.width
                    anchors.left: parent.left
                    anchors.leftMargin: listview.editing ? 74 : 0
                    Image {
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        height: 164
                        width: parent.width
                        source: "qrc:/main/images/list_line.png"
                    }
                    Text {
                        id: thename
                        anchors.top: parent.top
                        anchors.topMargin: 16
                        anchors.left: parent.left
                        anchors.leftMargin: 24
                        width: 300
                        height: 40
                        clip: true
                        horizontalAlignment: Qt.AlignLeft
                        text: groupName
                        color: "white"
                        font.pixelSize: 36
                        elide: Text.ElideRight
                    }
                    Text {
                        id: network
                        anchors.top: thename.bottom
                        anchors.topMargin: 16
                        anchors.left: parent.left
                        anchors.leftMargin: 24
                        width: 200
                        height: 30
                        clip: true
                        horizontalAlignment: Qt.AlignLeft
                        text: networkType
                        color: "white"
                        font.pixelSize: 30
                    }
                    Image {
                        id: personIcon
                        anchors.top: network.bottom
                        anchors.topMargin: 16
                        anchors.left: parent.left
                        anchors.leftMargin: 24
                        source: "qrc:/group/images/PersonNum.png"
                    }
                    Text {
                        id: num
                        anchors.top: network.bottom
                        anchors.topMargin: 16
                        anchors.left: personIcon.right
                        anchors.leftMargin: 16
                        width: 50
                        height: 30
                        clip: true
                        horizontalAlignment: Qt.AlignLeft
                        text: personNum
                        color: "white"
                        font.pixelSize: 30
                    }
                    MouseArea {
                        anchors.left: thename.left
                        anchors.right: more.left
                        anchors.rightMargin: 30
                        height: parent.height
                        visible: listview.editing ? false : true
                        onPressed: {
                            clickeffects.opacity = 0.5
                        }
                        onReleased: {
                            clickeffects.opacity = 0
                            getData.SetGroupId(conversationmodel.get(index).id)
                            //filesSendModel.setRemoteIp(listModel.get(index).value)
                            pageStack.push("qrc:/qml/VideoViewPage.qml",{type:1,sendtype:1});
                        }
                        onPositionChanged: {
                            clickeffects.opacity = 0
                        }
                    }
                    CButton{
                        id: more
                        anchors.top: parent.top
                        anchors.topMargin: 48
                        anchors.right: parent.right
                        anchors.rightMargin: 24
                        width: 100
                        height: 72
                        visible: false//listview.editing ? false : true
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "qrc:/main/images/more.png"
                        backgroundEnabled: false
                        onClicked: {
                            pageStack.push("qrc:/qml/GroupInfo.qml",{"groupid":conversationmodel.get(index).id,
                                               "groupname":conversationmodel.get(index).groupName,
                                               "groupnum":conversationmodel.get(index).personNum,
                                               "groupnet":conversationmodel.get(index).networkType,
                                               "qmlcontains":groupmain})
                        }
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
        }
        ListModel {
            id: conversationmodel
        }
        ListModel {
            id: subordinatemodel
        }
        CEditListView {
            id:listview
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.bottom: parent.bottom
            width: parent.width - 32
            model: bIsConversation ? conversationmodel : subordinatemodel
            editable: bIsConversation
            clip: true
            delegate: bIsConversation ? listText : subordinate
            onEditingChanged: {
                if (!listview.editing)
                    editToolBar.visible = false
            }

            Component.onCompleted: {
                //getData.getGroups(false)
                //conversationmodel.append({"id":id, "groupName":name, "networkType":network, "personNum":num})
                                //conversationmodel.append({"id":"2002","groupName":"1营","networkType":"自组网","personNum":"20"})
                //                subordinatemodel.append({"id":"2003","postinfo":"1连","nameinfo":"张国荣","numinfo":"4","netinfo":"自组网"})
                //                subordinatemodel.append({"id":"1004","postinfo":"2连","nameinfo":"段正淳","numinfo":"6","netinfo":"模块数字对讲"})
                //                subordinatemodel.append({"id":"1005","postinfo":"3连","nameinfo":"乔峰","numinfo":"3","netinfo":"自组网"})
                //                subordinatemodel.append({"id":"1006","postinfo":"4连","nameinfo":"郝斌","numinfo":"2","netinfo":"LTE"})
                //                subordinatemodel.append({"id":"1007","postinfo":"5连","nameinfo":"高乐","numinfo":"5","netinfo":"模块数字对讲"})
            }
        }
//        Menu {
//            id: toolBar
//            anchors.bottom: parent.bottom
//            height: 120
//            width: parent.width
//            group.iconSource: "qrc:/group/images/GroupSelect.png"
//            home.iconSource: "qrc:/group/images/PersonUnSelect.png"
//            history.iconSource: "qrc:/main/images/history.png"
//            bIsGroup: true
//            bIsHistory: false
//        }
        CToolBar {
            id: editToolBar
            visible: listview.editing ? true : false
            height: 120
            width: parent.width
            anchors.bottom: parent.bottom
            spacing: 200
            backgroundEnabled: true
            backgroundComponent: Component {
                Image {
                    anchors.fill: parent
                    source: "qrc:/main/images/bottom_line.png"
                }
            }
            CButton {
                height: 80
                width: 200
                backgroundEnabled: true
                backgroundComponent: Component {
                    Image {
                        anchors.fill: parent
                        source: "qrc:/main/images/delete.png"
                    }
                }
                onClicked: {
                    deleteback.visible = true
                    dialog.visible = true
                    deleteback.opacity = 0.7
                }
            }
        }
        Component {
            id: rightTitle
            CButton {
                anchors.fill: parent
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/main/images/add_click.png" : "qrc:/main/images/add.png"
                onClicked: {
                    pageStack.push("qrc:/qml/GroupAdd.qml")
                }
            }
        }
        Component {
            id: leftTitle
            CButton {
                anchors.fill: parent
                backgroundEnabled: false
                iconSource: "qrc:/set/images/set_delete.png"
                onClicked: {
                    conversationmodel.clear()
                    getData.getGroups(true)
                    if (conversationmodel.count) {
                        listview.editing = true
                        toolBar.visible = false
                        editToolBar.visible = true
                    } else {
                        getData.getGroups(false)
                    }

                }
            }
        }
        Component {
            id: title
            Item {
                width: 244
                height: 50
                CButton{
                    id: leftGroup
                    width: 122
                    height: 50
                    iconSource: "qrc:/group/images/subordinateclick.png"
                    backgroundEnabled: false
                    onClicked: {
                        if (rightGroup.iconSource == "qrc:/group/images/conversationclick.png") {
                            leftGroup.iconSource = "qrc:/group/images/subordinateclick.png"
                            rightGroup.iconSource = "qrc:/group/images/conversation.png"
                        }
                        bIsConversation = false
//                        if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
//                            titleBar.rightItemEnabled = true
//                        else
//                            titleBar.rightItemEnabled = false
                    }
                }
                CButton{
                    id: rightGroup
                    anchors.left: leftGroup.right
                    width: 122
                    height: 50
                    backgroundEnabled: false
                    iconSource: "qrc:/group/images/conversation.png"
                    onClicked: {
                        if (leftGroup.iconSource == "qrc:/group/images/subordinateclick.png") {
                            leftGroup.iconSource = "qrc:/group/images/subordinate.png"
                            rightGroup.iconSource = "qrc:/group/images/conversationclick.png"
                        }
                        bIsConversation = true

//                        if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
//                            titleBar.rightItemEnabled = true
//                        else
//                            titleBar.rightItemEnabled = false

                    }
                }
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
        width: 492
        height: 386
        z: 4
        Image {
            anchors.fill: parent
            source: "qrc:/history/images/dialog.png"
        }
        Text {
            id: message
            anchors.top: parent.top
            anchors.topMargin: 116
            height: 40
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            color: "white"
            font.pixelSize: 40
            text: qsTr("要删除选中的群组?")
        }
        CButton {
            id: deletecancel
            anchors.top: message.bottom
            anchors.topMargin: 65
            anchors.left: parent.left
            anchors.leftMargin: 54
            backgroundEnabled: false
            width: 174
            height: 100
            iconSource: pressed ? "qrc:/history/images/cancelclick.png" :
                                  "qrc:/history/images/canceldelete.png"
            onClicked: {
                dialog.visible = false
                deleteback.visible = false
                deleteback.opacity = 0
            }
        }
        CButton {
            id: deletesure
            anchors.top: message.bottom
            anchors.topMargin: 65
            anchors.left: deletecancel.right
            anchors.leftMargin: 48
            width: 174
            height: 100
            backgroundEnabled: false
            iconSource: pressed ? "qrc:/history/images/sureclick.png" : "qrc:/history/images/suredelete.png"
            onClicked: {
                var indexs = listview.selectedIndexes
                listview.deselect()
                if (indexs.length) {
//                    for (var i = indexs.length - 1; i >= 0; --i){
//                        getData.removeGroup(conversationmodel.get(indexs[i]).id)
//                    }
                    conversationmodel.clear()
                    getData.getGroups(false)
                    listview.editing = false
                    editToolBar.visible = false
//                    toolBar.visible = true
                }
                dialog.visible = false
                deleteback.opacity = 0
                deleteback.visible = false

            }
        }
    }
}
