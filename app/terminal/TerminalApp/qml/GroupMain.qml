import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id:groupmain
    property bool bIsConversation: false/*是否会话群*/
    Connections {
        target: getData
        onSignalGroupChanged: {
            if (type == 1) {/*add*/
                conversationmodel.append({"id":id,"groupName":name, "networkType":network, "personNum":num,"syncCount":syncCount})
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
            else if (type == 3) {/*update*/
                for (var i = 0;i < conversationmodel.count;i++){
                    if (conversationmodel.get(i).id == id)
                    {
                        conversationmodel.set(i,{"id":id,"groupName":name, "networkType":network, "personNum":num});
                        break;
                    }
                }
            }
        }
    }
    Connections {
        target: getData
        onSignalSendGroupInfoToGroup: {
            conversationmodel.append({"id":id, "groupName":name, "networkType":network, "personNum":num, "syncCount":syncCount})
        }
        onSignalUpdateMessageCount: { // groupId, const int &messageCount
            for (var i = 0; i < conversationmodel.count; i++)
            {
                if (conversationmodel.get(i).id == groupId)
                {
                    conversationmodel.set(i,{"syncCount":messageCount})
                }
            }
        }
    }
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        groupmain.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            toolBar.direction = true
            groupmain.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            toolBar.direction = false
            groupmain.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                toolBar.direction = true
                groupmain.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                toolBar.direction = false
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
            leftItemEnabled: bIsConversation ? true : false
            leftItemBackgroundEnabled: true
            leftItemSize: Qt.size(74,74)
            leftItemComponent: leftTitle
            rightItemEnabled: bIsConversation ? true : false
            rightItemBackgroundEnabled: true
            rightItemSize: Qt.size(74,74)
            rightItemComponent: rightTitle
            titleItemEnabled: true
            titleAreaCentered: true
            titlePixelSize: 30
            titleTextColor: "#29afe2"
            titleItemComponent: title
            editView: listview
            editLeftItemLeftMargin: 2
            editLeftComponent: CButton {
                iconSource: "qrc:/main/images/back.png"
                backgroundEnabled: false
                onClicked: {
                    listview.deselect()
                    conversationmodel.clear()
                    getData.getGroups(false)
                    listview.editing = false
                    editToolBar.visible = false
                    toolBar.visible = true
                }
            }
            editRightComponent: CButton {
                id: rightCheck
                property bool checked: false
                anchors.fill: parent
                pixelSize: 30
                textColor: "#29afe2"
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
//            backgroundComponent: Component{
//                Image {
//                    anchors.fill: parent
//                    source: "qrc:/main/images/top_line.png"
//                }
//            }
            Component.onCompleted: {
                if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
                    titleBar.rightItemEnabled = true
                else
                    titleBar.rightItemEnabled = false
            }
        }
//        Item{
//            id: mid
//            width: parent.width
//            anchors.top: titleBar.bottom
//            anchors.bottom: parent.bottom
//            Image {
//                fillMode: Image.Tile
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid_up.png"
//            }
//            Image {
//                anchors.fill: parent
//                source: "qrc:/main/images/back_mid.png"
//            }
//        }
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

                    Rectangle {
                        anchors.top: parent.top
                        anchors.topMargin: 8
                        anchors.left: parent.left
                        height: 164
                        width: parent.width
                        color: "#546e8f"
                        opacity:0.3
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
                        width: 37
                        height: 40
                        source: "qrc:/main/images/icon_people_count.png"
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
                            if ("模块数字对讲" == conversationmodel.get(index).networkType)
                            {
                                getData.onGetMessageList(conversationmodel.get(index).id)

                                pageStack.push("qrc:/qml/DigitalGroup.qml",{"groupid":conversationmodel.get(index).id,
                                                   "groupname":conversationmodel.get(index).groupName,"bIsPtt":false,
                                                   "groupnum":conversationmodel.get(index).personNum,
                                                   "netinfo":conversationmodel.get(index).networkType,
                                                   "prepage":groupmain})
                            }
                            else {
//                                getData.addCachePage(conversationmodel.get(index).groupName, conversationmodel.get(index).id,
//                                                     false, conversationmodel.get(index).personNum, conversationmodel.get(index).networkType)
//                                var myPage = pageStack.getCachedPage("qrc:/qml/PptChatRoom.qml",conversationmodel.get(index).id)
//                                //myPage.doSomething() //对page数据进行设置
//                                pageStack.push(myPage,{"groupid":conversationmodel.get(index).id,
//                                                   "groupname":conversationmodel.get(index).groupName,"bIsPtt":false,
//                                                   "groupnum":conversationmodel.get(index).personNum,
//                                                   "netinfo":conversationmodel.get(index).networkType,
//                                                   "prepage":groupmain})
                                pageStack.push("qrc:/qml/PptChatRoom.qml",{"groupid":conversationmodel.get(index).id,
                                                   "groupname":conversationmodel.get(index).groupName,"bIsPtt":false,
                                                   "groupnum":conversationmodel.get(index).personNum,
                                                   "netinfo":conversationmodel.get(index).networkType,
                                                   "prepage":groupmain})
                            }
                        }
                        onPositionChanged: {
                            clickeffects.opacity = 0
                        }
                    }
                    Rectangle {
                        id: redRect
                        width: 50
                        height: 50
                        radius: 25
                        color: "red"
                        visible: syncCount == 0 ? false : true
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: more.left
                        anchors.rightMargin: 24
                        Text {
                            id: groupSyncCount
                            text: syncCount
                            anchors.centerIn: parent
                            color: "#ffffff"
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
                        visible: listview.editing ? false : true
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "qrc:/main/images/group_more.png"
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
            anchors.bottom: toolBar.top
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
                getData.getGroups(false)
                //                subordinatemodel.append({"id":"2002","postinfo":"1营","nameinfo":"赵军","numinfo":"20","netinfo":"LTE"})
                //                subordinatemodel.append({"id":"2003","postinfo":"1连","nameinfo":"张国荣","numinfo":"4","netinfo":"自组网"})
                //                subordinatemodel.append({"id":"1004","postinfo":"2连","nameinfo":"段正淳","numinfo":"6","netinfo":"模块数字对讲"})
                //                subordinatemodel.append({"id":"1005","postinfo":"3连","nameinfo":"乔峰","numinfo":"3","netinfo":"自组网"})
                //                subordinatemodel.append({"id":"1006","postinfo":"4连","nameinfo":"郝斌","numinfo":"2","netinfo":"LTE"})
                //                subordinatemodel.append({"id":"1007","postinfo":"5连","nameinfo":"高乐","numinfo":"5","netinfo":"模块数字对讲"})
            }
        }
        Menu {
            id: toolBar
            anchors.bottom: parent.bottom
            height: 120
            width: parent.width
            group.iconSource: "qrc:/main/images/group_click.png"
            home.iconSource: "qrc:/main/images/home.png"
            history.iconSource: "qrc:/main/images/history.png"
            bIsGroup: true
            bIsHistory: false
        }
        CToolBar {
            id: editToolBar
            visible: listview.editing ? true : false
            height: 120
            width: parent.width
            anchors.bottom: parent.bottom
            spacing: 200
            backgroundEnabled: true
            backgroundComponent: Rectangle {
                anchors.fill: parent
                color: "#546e8f"
                opacity: 0.3
            }
            CButton {
                height: 80
                width: 200
                backgroundEnabled: false
                text: "删除"
                pixelSize: 32
                textColor: "#29afe2"
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
                text: "添加"
                textColor: "#29afe2"
                //iconSource: pressed ? "qrc:/main/images/add_click.png" : "qrc:/main/images/add.png"
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
                iconSource: "qrc:/main/images/main_delete.png"
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
            Rectangle {
                width: 244
                height: 50
                border.color: "#29afe2"
                border.width: 2
                radius: 10
                color: Qt.rgba(0,0,0,0)
                CButton{
                    id: leftGroup
                    width: 122
                    height: 50
                    text: "建制群"
                    textColor: "#ffffff"
                    backgroundEnabled: true
                    backgroundComponent: Component {
                        Rectangle {
                            id: titlerect
                            anchors.fill: parent
                            radius: 10
                            color: "#29afe2"
                        }
                    }
                    onClicked: {
                        if (leftGroup.backgroundEnabled == false)
                        {
                            leftGroup.backgroundEnabled = true
                            leftGroup.textColor = "#ffffff"
                            rightGroup.backgroundEnabled = false
                            rightGroup.textColor = "#29afe2"
                        }

                        bIsConversation = false
                        if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
                            titleBar.rightItemEnabled = true
                        else
                            titleBar.rightItemEnabled = false
                    }
                }
                CButton{
                    id: rightGroup
                    anchors.left: leftGroup.right
                    width: 122
                    height: 50
                    text: "会话群"
                    textColor: "#29afe2"
                    backgroundEnabled: false
                    backgroundComponent: Component {
                        Rectangle {
                            anchors.fill: parent
                            radius: 10
                            color: "#29afe2"
                        }
                    }
                    onClicked: {
                        if (rightGroup.backgroundEnabled == false)
                        {
                            leftGroup.backgroundEnabled = false
                            leftGroup.textColor = "#29afe2"
                            rightGroup.backgroundEnabled = true
                            rightGroup.textColor = "#ffffff"
                        }
                        bIsConversation = true

                        if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
                            titleBar.rightItemEnabled = true
                        else
                            titleBar.rightItemEnabled = false

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
        width: 504
        height: 296
        color: "#ffffff"
        radius: 10
        z: 4
        Text {
            id: message
            anchors.top: parent.top
            anchors.topMargin: 79
            height: 33
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            color: "black"
            font.pixelSize: 32
            text: qsTr("要删除选中的群组?")
        }
        CButton {
            id: deletecancel
            anchors.top: message.bottom
            anchors.topMargin: 69
            anchors.left: parent.left
            anchors.leftMargin: 62
            backgroundEnabled: true
            backgroundComponent: Rectangle {
                anchors.fill: parent
                radius: 10
                color: "#29afe2"
            }
            text: "取消"
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
            backgroundEnabled: true
            backgroundComponent: Rectangle {
                anchors.fill: parent
                radius: 10
                color: "#29afe2"
            }
            text: "确认"
            width: 150
            height: 67
            onClicked: {
                var indexs = listview.selectedIndexes
                listview.deselect()
                if (indexs.length) {
                    for (var i = indexs.length - 1; i >= 0; --i){
                        getData.removeGroup(conversationmodel.get(indexs[i]).id)
                    }
                    conversationmodel.clear()
                    getData.getGroups(false)
                    listview.editing = false
                    editToolBar.visible = false
                    toolBar.visible = true
                }
                dialog.visible = false
                deleteback.opacity = 0
                deleteback.visible = false

            }
        }
    }
}
