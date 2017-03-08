import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id:groupmain
    property bool bIsConversation: false/*是否会话群*/
    property var groupType: 0
    Connections {
        target: getData
        onSignalGroupChanged: {
            console.log("onSignalGroupChanged",syncCount)
            if (type == 1) {/*add*/
                conversationmodel.append({"id":id,"groupName":name, "networkType":network, "personNum":num,"syncCount":syncCount})
            }
            else if (type == 2) {/*delete*/
                for (var i = 0;i < conversationmodel.count;i++){
                    if (conversationmodel.get(i).id == id)
                    {
                        conversationmodel.remove(i,1);
                        pageStack.deleteCachedPage(id)
                        getData.removeCachePage(id)
                        break;
                    }
                }
            }
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
        //(QString id,QString name, QString network, QString num, int title, int syncCount
        onSignalSendOrganizationGroupInfoToGroup: {
            if (title == 5)
            {
                subordinatemodel.append({"id":id,"postinfo":name,"nameinfo":nameInfo,"numinfo":num,"netinfo":network,"level":0,"subNode":[]})
            }
            else if (title == 4)
            {
                for (var i = 0; i < subordinatemodel.count; i++)
                {
                    if (parentId == subordinatemodel.get(i).id)
                    {
                        subordinatemodel.get(i).subNode.append({"id":id,"postinfo":name,"nameinfo":nameInfo,"numinfo":num,"netinfo":network,"level":1,"subNode":[]})
                        break;
                    }
                }
            }
            else if (title == 3)
            {
                for (var j = 0; j < subordinatemodel.count; j++)
                {
                    for (var m = 0; m < subordinatemodel.get(j).subNode.count; m++)
                    {
                        if (parentId == subordinatemodel.get(j).subNode.get(m).id)
                        {
                            subordinatemodel.get(j).subNode.get(m).subNode.append({"id":id,"postinfo":name,"nameinfo":nameInfo,"numinfo":num,"netinfo":network,"level":2,"subNode":[]})
                            break;
                        }
                    }
                }
            }
        }

        onSignalGroupExchange: {
            subordinatemodel.clear()
            if (type == 2){
                console.log("2xx")
                groupmain.groupType = 2
                if (groupmain.bIsConversation)
                    titleBar.titleItem.leftSource = "qrc:/group/images/subordinate.png"
                else
                    titleBar.titleItem.leftSource = "qrc:/group/images/subordinateclick.png"

            }
            else if (type == 3){
                groupmain.groupType = 3
                console.log("3xx")
                if (groupmain.bIsConversation)
                    titleBar.titleItem.leftSource = "qrc:/group/images/battle.png"
                else
                    titleBar.titleItem.leftSource = "qrc:/group/images/battle-click.png"
            }
        }

//        subordinatemodel.append({"id":"1003","postinfo":"3营","nameinfo":"赵军","numinfo":"20","netinfo":"LTE","level":0,"subNode":[]})
//        subordinatemodel.get(1).subNode.append({"id":"2001","postinfo":"1连","nameinfo":"张国荣","numinfo":"4","netinfo":"自组网","level":1,"subNode":[]})

    }
    Connections {
        target: getData
        onSignalSendGroupInfoToGroup: {
            console.log("onSignalSendGroupInfoToGroup",syncCount)
            conversationmodel.append({"id":id, "groupName":name, "networkType":network, "personNum":num, "syncCount":syncCount})
        }
        onSignalUpdateMessageCount: { // groupId, const int &messageCount
            console.log("onSignalUpdateMessageCount",messageCount)
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
        if (gScreenInfo.displayHeight == 800)
            groupmain.orientationPolicy = CPageOrientation.LockLandscape
        groupmain.color = "black"
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
            rightItemEnabled: true//bIsConversation ? true : false
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
            Component.onCompleted: {
                if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
                    titleBar.rightItemEnabled = true
                else
                    titleBar.rightItemEnabled = true
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
            id: subordinate
            Column {
                id: subordinateColumn
                MouseArea {
                    width: objRow.implicitWidth
                    height: objRow.implicitHeight
                    onClicked: {
//                        getData.addCachePage(subordinatemodel.get(index).postinfo, subordinatemodel.get(index).id,
//                                             false, subordinatemodel.get(index).numinfo, subordinatemodel.get(index).netinfo)
//                        var myPage = pageStack.getCachedPage("qrc:/qml/PptChatRoom.qml",subordinatemodel.get(index).id)
//                        //myPage.doSomething() //对page数据进行设置
//                        pageStack.push(myPage,{"groupid":subordinatemodel.get(index).id,
//                                           "groupname":subordinatemodel.get(index).postinfo,"bIsPtt":false,
//                                           "groupnum":subordinatemodel.get(index).numinfo,
//                                           "netinfo":subordinatemodel.get(index).netinfo,
//                                           "prepage":groupmain})
                    }

//                    onDoubleClicked: {
//                        for(var i = 1; i < parent.children.length - 1; ++i) {
//                            parent.children[i].visible = !parent.children[i].visible
//                        }
//                    }
                    Row {
                        id: objRow
                        //"qrc:/main/images/add.png"

                        Item {
                            height: 126
                            width: (model.level + 1) * 50
                            z: 10
                            Text {
                                anchors.fill: parent

                                font.pixelSize: 60
                                text: (subordinateColumn.children.length > 2 ?
                                           subordinateColumn.children[1].visible ?
                                               qsTr("-  ") : qsTr("+ ") : qsTr("   ")) + qsTr("")
                                color: "white" //subordinateColumn.children.length > 2 ? "blue" : "green"
                                anchors.left: parent.left
                                verticalAlignment: Qt.AlignVCenter
                            }
                            MouseArea {
                                z: 11
                                anchors.fill: parent
                                onClicked: {
                                    for(var i = 1; i < subordinateColumn.children.length - 1; ++i) {
                                        subordinateColumn.children[i].visible = !subordinateColumn.children[i].visible
                                    }
                                }
                            }
                        }



                        Item {
                            width: listview.width - (model.level + 1) * 50
                            height: 126
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    getData.addCachePage(model.postinfo, model.id,
                                                         false, model.numinfo, model.netinfo)
                                    var myPage = pageStack.getCachedPage("qrc:/qml/PptChatRoom.qml",model.id)
                                    //myPage.doSomething() //对page数据进行设置
                                    pageStack.push(myPage,{"groupid":model.id,
                                                       "groupname":model.postinfo,"bIsPtt":false,
                                                       "groupnum":model.numinfo,
                                                       "netinfo":model.netinfo,
                                                       "prepage":groupmain})
                                }
                            }


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
                                width: 270
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
                                width: 150
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
                            //                    MouseArea {
                            //                        anchors.right: more.left
                            //                        anchors.rightMargin: 50
                            //                        anchors.left: parent.left
                            //                        height: parent.height
                            //                        onPressed: {
                            //                            clickeffects.opacity = 0.5
                            //                        }
                            //                        onReleased: {
                            //                            clickeffects.opacity = 0
                            //                        }
                            //                        onPositionChanged: {
                            //                            clickeffects.opacity = 0
                            //                        }
                            //                    }
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
                                    pageStack.push("qrc:/qml/GroupInfo.qml",{"groupid":model.id,
                                                       "groupname":model.postinfo,
                                                       "groupnum":model.numinfo,
                                                       "groupnet":model.netinfo,
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
                Repeater {
                   model: subNode
                   delegate: subordinate
                }
            }
        }


        /*
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
        */
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
                            if ("模块数字对讲" == conversationmodel.get(index).networkType)
                            {
                                getData.onGetMessageList(conversationmodel.get(index).id)

                                pageStack.push("qrc:/qml/DigitalGroup.qml",{"groupid":conversationmodel.get(index).id,
                                                   "groupname":conversationmodel.get(index).groupName,"bIsPtt":false,
                                                   "groupnum":conversationmodel.get(index).personNum,
                                                   "netinfo":conversationmodel.get(index).networkType,
                                                   "prepage":groupmain})
                                getData.joinBtGroup(conversationmodel.get(index).id)
                            }
                            else {
                                getData.addCachePage(conversationmodel.get(index).groupName, conversationmodel.get(index).id,
                                                     false, conversationmodel.get(index).personNum, conversationmodel.get(index).networkType)
                                var myPage = pageStack.getCachedPage("qrc:/qml/PptChatRoom.qml",conversationmodel.get(index).id)
                                //myPage.doSomething() //对page数据进行设置
                                pageStack.push(myPage,{"groupid":conversationmodel.get(index).id,
                                                   "groupname":conversationmodel.get(index).groupName,"bIsPtt":false,
                                                   "groupnum":conversationmodel.get(index).personNum,
                                                   "netinfo":conversationmodel.get(index).networkType,
                                                   "prepage":groupmain})
//                                pageStack.push("qrc:/qml/PptChatRoom.qml",{"groupid":conversationmodel.get(index).id,
//                                                   "groupname":conversationmodel.get(index).groupName,"bIsPtt":false,
//                                                   "groupnum":conversationmodel.get(index).personNum,
//                                                   "netinfo":conversationmodel.get(index).networkType,
//                                                   "prepage":groupmain})
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
            anchors.bottom: toolBar.top
            width: parent.width - 32
            model: bIsConversation ? conversationmodel : subordinatemodel
            editable: bIsConversation
            clip: true
            delegate: bIsConversation ? listText : subordinate
            onEditingChanged: {
                if (!listview.editing){
                    editToolBar.visible = false
                    toolBar.visible = true
                }
            }

            Component.onCompleted: {
                getData.getGroups(false)
                getData.onGetGroups(3)
//                subordinatemodel.append({"id":"2002","postinfo":"1营","nameinfo":"赵军","numinfo":"20","netinfo":"LTE"})
//                subordinatemodel.append({"id":"2003","postinfo":"1连","nameinfo":"张国荣","numinfo":"4","netinfo":"自组网"})
//                subordinatemodel.append({"id":"1004","postinfo":"2连","nameinfo":"段正淳","numinfo":"6","netinfo":"模块数字对讲"})
//                subordinatemodel.append({"id":"1005","postinfo":"3连","nameinfo":"乔峰","numinfo":"3","netinfo":"自组网"})
//                subordinatemodel.append({"id":"1006","postinfo":"4连","nameinfo":"郝斌","numinfo":"2","netinfo":"LTE"})
//                subordinatemodel.append({"id":"1007","postinfo":"5连","nameinfo":"高乐","numinfo":"5","netinfo":"模块数字对讲"})

//                subordinatemodel.append({"name":"Zero","level":0,"subNode":[]})
//                subordinatemodel.append({"name":"One","level":0,"subNode":[]})
//                subordinatemodel.append({"name":"Two","level":0,"subNode":[]})
//                subordinatemodel.get(1).subNode.append({"name":"Three","level":1,"subNode":[]})
//                subordinatemodel.get(1).subNode.append({"name":"Four","level":1,"subNode":[]})
//                subordinatemodel.get(1).subNode.get(0).subNode.append({"name":"Five","level":2,"subNode":[]})

//                subordinatemodel.append({"id":"1001","postinfo":"1营","nameinfo":"赵军","numinfo":"20","netinfo":"LTE","level":0,"subNode":[]})
//                subordinatemodel.append({"id":"1002","postinfo":"2营","nameinfo":"赵军","numinfo":"20","netinfo":"LTE","level":0,"subNode":[]})
//                subordinatemodel.append({"id":"1003","postinfo":"3营","nameinfo":"赵军","numinfo":"20","netinfo":"LTE","level":0,"subNode":[]})

//                subordinatemodel.get(1).subNode.append({"id":"2001","postinfo":"1连","nameinfo":"张国荣","numinfo":"4","netinfo":"自组网","level":1,"subNode":[]})
//                subordinatemodel.get(1).subNode.append({"id":"2002","postinfo":"2连","nameinfo":"段正淳","numinfo":"6","netinfo":"模块数字对讲","level":1,"subNode":[]})
//                subordinatemodel.get(1).subNode.append({"id":"2003","postinfo":"3连","nameinfo":"乔峰","numinfo":"3","netinfo":"自组网","level":1,"subNode":[]})
//                subordinatemodel.get(1).subNode.append({"id":"2004","postinfo":"4连","nameinfo":"郝斌","numinfo":"2","netinfo":"LTE","level":1,"subNode":[]})
//                subordinatemodel.get(1).subNode.append({"id":"2005","postinfo":"5连","nameinfo":"高乐","numinfo":"5","netinfo":"模块数字对讲","level":1,"subNode":[]})
//                subordinatemodel.get(2).subNode.append({"id":"2006","postinfo":"6连","nameinfo":"高乐","numinfo":"5","netinfo":"模块数字对讲","level":1,"subNode":[]})

            }
        }
        Menu {
            id: toolBar
            anchors.bottom: parent.bottom
            height: 120
            width: parent.width
            group.iconSource: "qrc:/group/images/GroupSelect.png"
            home.iconSource: "qrc:/group/images/PersonUnSelect.png"
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
                property alias leftSource: leftGroup.iconSource
                property alias rightSource: rightGroup.iconSource
                CButton{
                    id: leftGroup
                    width: 122
                    height: 50
                    iconSource: "qrc:/group/images/subordinateclick.png"
                    backgroundEnabled: false
                    onClicked: {
                        if (rightGroup.iconSource == "qrc:/group/images/conversationclick.png") {
                            if (groupmain.groupType == 2)
                                leftGroup.iconSource = "qrc:/group/images/subordinateclick.png"
                            else if (groupmain.groupType == 3)
                                leftGroup.iconSource = "qrc:/group/images/battle-click.png"
                            rightGroup.iconSource = "qrc:/group/images/conversation.png"
                        }
                        bIsConversation = false
                        if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
                            titleBar.rightItemEnabled = true
                        else
                            titleBar.rightItemEnabled = true
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
                        if (leftGroup.iconSource == "qrc:/group/images/subordinateclick.png" || leftGroup.iconSource == "qrc:/group/images/battle-click.png") {
                            if (groupmain.groupType == 2)
                                leftGroup.iconSource = "qrc:/group/images/subordinate.png"
                            else if (groupmain.groupType == 3)
                                leftGroup.iconSource = "qrc:/group/images/battle.png"
                            rightGroup.iconSource = "qrc:/group/images/conversationclick.png"
                        }
                        bIsConversation = true

                        if ((getData.getLocalTitle() == 5 || getData.getLocalTitle() == 4) && bIsConversation == true )
                            titleBar.rightItemEnabled = true
                        else
                            titleBar.rightItemEnabled = true

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
                    for (var i = indexs.length - 1; i >= 0; --i){
                        getData.removeCachePage(conversationmodel.get(indexs[i]).id)
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
