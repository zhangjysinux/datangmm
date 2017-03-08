import QtQuick 2.0
import com.syberos.basewidgets 2.0
/*getLocalInGroupIdentify 重写接口 180*/
CPage{
    id: groupInfo
    property string groupid: ""
    property string groupname: ""
    property string groupnum: ""
    property string groupnet: ""
    property var qmlcontains: null
    property bool bIsEdit: false/*是否编辑*/
    property bool bIsFirstEdit: true/*是否首次编辑*/
    property var precontains: null
    property var createrId: ""
    Connections {
        target: getData
        onSignalSendMemberInfoToGroup: {
            listmodel.append({"id":id, "iconvalue":"","infoname":"","name":name,"position":value,"type":type,"bIsLeader":bIsLeader})
            //if (getData.getLocalInGroupIdentify(groupid) == "leader" || getData.getLocalInGroupIdentify(groupid) == "creater") {
            if (bIsLeader == "3"){
                listModel.append({"id":id, "iconvalue":"","infoname":"","name":name,"position":value,"type":type,"bIsLeader":bIsLeader})
            }

            if (bIsLeader == "1" || bIsLeader == "2")
                memberModel.append({"id":id, "iconvalue":"","infoname":"","name":name,"position":value,"type":type,"bIsLeader":bIsLeader})
            if (bIsLeader == "1")
                createrId = id
        }
        onSignalGroupChanged : {
            if (id != groupid)
                return;
            memberModel.clear();
            for (var i = 0 ; i < listModel.count; i++) {
                if (listModel.get(i).bIsLeader == "1" ||
                        listModel.get(i).bIsLeader == "2" ||
                        listModel.get(i).bIsLeader == "3") {
                    listModel.remove(i,1)
                    i--
                }
            }
            while (listmodel.count > 3) {
                listmodel.remove(listmodel.count-1,1)
            }
            listModel.set(0,{"name":name})
            listModel.set(1,{"infoname":num})
            listModel.set(2,{"infoname":network})
            listmodel.set(0,{"name":name})
            listmodel.set(1,{"infoname":num})
            listmodel.set(2,{"infoname":network})
            getData.getGroup(groupid)
            if (getData.getLocalInGroupIdentify(groupid) == "creater" && !bIsEdit) {
                deleteGroup.visible = true
            }
        }
    }

    function theGroupAddPerson(id,personName,position)
    {
        pageStack.pop()
        console.log("theGroupAddPerson",id)
        listmodel.append({"id":id, "iconvalue":"","infoname":"","name":personName,"position":position,"type":"1","bIsLeader":"3"})
        listModel.insert(listModel.count - 1,{"id":id, "iconvalue":"","infoname":"","name":personName,"position":position,"type":"1","bIsLeader":"3"})
    }

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        groupInfo.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            groupInfo.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            groupInfo.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
        if (getData.getLocalInGroupIdentify(groupid) == "creater" && !bIsEdit && qmlcontains.bIsConversation) {
            deleteGroup.visible = true
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                groupInfo.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                groupInfo.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }
    contentAreaItem: Item {
        anchors.fill: parent
        CTitleBar {
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            leftItemEnabled: true
            leftItemBackgroundEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemComponent: Component {
                id: leftTitle
                CButton {
                    anchors.fill: parent
                    iconSource: "qrc:/main/images/back.png"
                    backgroundEnabled: false
                    text: "返回"
                    textColor: "#29afe2"
                    spacing: 0
                    onClicked: {
                        if (bIsEdit) {
                            closeSoftKeyboard()
                            deleteback.visible = true
                            deleteback.opacity = 0.7
                            dialog.visible = true
                            dialog.isClear = false
                            dialog.text = "是否退出当前编辑页面?"
                        } else {
                            pageStack.pop()
                        }
                    }
                }
            }
            rightItemEnabled: ((getData.getLocalInGroupIdentify(groupid) == "creater" || getData.getLocalInGroupIdentify(groupid) == "leader") &&
                               qmlcontains.bIsConversation ) ? true : false
            rightItemBackgroundEnabled: true
            rightItemSize: Qt.size(74,74)
            rightItemComponent: rightTitle
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
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
            id: rightTitle
            CButton {
                anchors.fill: parent
                pixelSize: 30
                textColor: "#29afe2"
                backgroundEnabled: false
                text: qsTr("编辑")
                iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                spacing: -68
                onClicked: {
                    closeSoftKeyboard()
                    if (text == "编辑") {
                        bIsEdit = true
                        deleteGroup.visible = false
                        if (bIsFirstEdit) {
                            bIsFirstEdit = false
                            listModel.remove(1,1)
                            listModel.remove(1,1)
                        }
                        if (getData.getLocalInGroupIdentify(groupid) == "creater"){
                            for (var i =0 ;i < memberModel.count;i++)
                            {
                                listModel.append(memberModel.get(i))
                            }
                        }
                        listModel.append({"id":"add", "iconvalue":"","infoname":"","name":"添加","position":"","type":"","bIsLeader":"3"})
                        listview.model = listModel
                        text = "完成"
                    } else {
                        bIsEdit = false
                        listModel.remove(listModel.count -1,1)
                        var i = listModel.count
                        listmodel.set(0,{"name":groupname})
                        listModel.set(0,{"name":groupname})
                        listmodel.set(1,{"infoname":i.toString()})
                        listview.model = listmodel
                        getData.clearGroupLeaders()
                        getData.clearGroupMembers()
                        for (var i = 0; i < listmodel.count; i++) {
                            if (listmodel.get(i).bIsLeader == "3") {
                                getData.setGroupMembers(listmodel.get(i).id)
                            }
                            if (listmodel.get(i).bIsLeader == "1" || listmodel.get(i).bIsLeader == "2") {
                                getData.setGroupLeaders(listmodel.get(i).id)
                            }
                        }
                        //getData.setGroupLeaders(getData.getLocalId())
                        if (getData.getLocalInGroupIdentify(groupid) == "creater"){
                            for (var i =0 ;i < memberModel.count;i++){
                                for (var j = 0; j < listModel.count; j++){
                                    if (memberModel.get(i).id == listModel.get(j).id)
                                        listModel.remove(j,1)
                                }
                            }
                        }
                        /*getLocalInGroupIdentify 重写接口 */

                        getData.updateGroup(groupid,groupname)
                        text = "编辑"
                    }
                }
            }
        }
        Component {
            id: grouplist
            Item {
                id: listtext
                height: 137
                width: listview.width
                Image {
                    id: icon
                    anchors.top: parent.top
                    width: 36
                    height: 36
                    source: iconvalue
                    visible: false
                }
                Text {
                    id: iconInfo
                    anchors.top: icon.top
                    anchors.left: icon.right
                    anchors.leftMargin: 16
                    width: 60
                    height: 36
                    font.pixelSize: 36
                    text: infoname
                    visible: false
                    color: "#ffffff"
                    onTextChanged: {
                        if ((listmodel.get(index).id == "") &&
                                (listmodel.get(index).name == "") &&
                                (listmodel.get(index).iconvalue != "")) {
                            listtext.height = 60
                            icon.visible = true
                            iconInfo.visible = true
                        }
                    }
                }
                Text {
                    id: personName1
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    width: 200
                    height: 42
                    text: name
                    clip: true
                    color: "#ffffff"
                    elide: Text.ElideRight
                    font.pixelSize: 40
                    visible: false
                    onTextChanged: {
                        if (!bIsEdit) {
                            if ((listmodel.get(index).id == "") &&
                                    (listmodel.get(index).name != "") &&
                                    (listmodel.get(index).iconvalue == "")) {
                                listtext.height = 176
                                positionText.anchors.topMargin = 48
                                positionText.visible = true
                                positionText.font.pixelSize = 40
                                personName1.visible = true
                            } else {
                                personName1.visible = true
                                positionText.visible = true
                                mousearea.visible = true
                                dial.visible = true
                                line.visible = true
                            }
                        }
                    }
                }
                Image {
                    id: groupflag
                    anchors.top: parent.top
                    anchors.topMargin: 28
                    anchors.left: personName1.right
                    anchors.leftMargin: 16
                    width: 98
                    height: 39
                    visible: (!bIsEdit && ( bIsLeader == "1" || bIsLeader == "2")) ? true : false
                    source: (bIsLeader == "1") ? "qrc:/main/images/groupman.png" :
                                                 (bIsLeader == "2") ? "qrc:/main/images/groupmanager.png" : ""
                    Text {
                        anchors.centerIn: parent
                        color: "#ffffff"
                        font.pixelSize: 26
                        text: (bIsLeader == "1") ? "创建者" : "管理员"
                    }
                }
                TextInput {
                    id: personName
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: bIsEdit ? 74 : 0
                    anchors.right: parent.right
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    height: 42
                    text: name
                    autoScroll: false
                    font.pixelSize: 40
                    readOnly: true
                    visible: bIsEdit
                    color: "#ffffff"
                    onTextChanged: {
                        if (bIsEdit) {
                            if ((listModel.get(index).id == "") &&
                                    (listModel.get(index).name != "") &&
                                    (listModel.get(index).iconvalue == "")) {
                                listtext.height = 176
                                positionText.anchors.topMargin = 48
                                positionText.visible = true
                                positionText.font.pixelSize = 40
                                personName.visible = true
                                personName.readOnly = false
                                if (bIsEdit) {
                                    line.anchors.top = personName.bottom
                                    line.visible = true
                                }
                                numLock.visible = true
                                groupname = personName.text
                            } else {
                                if (listModel.get(index).name == "添加") {
                                    listtext.height = 88
                                    editAdd.visible = true
                                    line.anchors.top = personName.bottom
                                } else {
                                    positionText.visible = true
                                    editDelete.visible = true
                                }
                                personName.visible = true
                                line.visible = true
                            }
                        }
                    }
                }
                Text {
                    id: positionText
                    anchors.top: personName.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: bIsEdit ? 74 : 0
                    width: 100
                    height: 30
                    text: position
                    font.pixelSize: 30
                    visible: false
                    color: "#b6b6b6"
                }
                CButton {
                    id: editDelete
                    anchors.right: line.left
                    anchors.rightMargin: 24
                    anchors.top: personName.top
                    anchors.topMargin: 20
                    width: 50
                    height: 50
                    backgroundEnabled: false
                    visible: false
                    iconSource: "qrc:/main/images/edit_delete.png"
                    onClicked: {
                        for (var i = 0; i< listmodel.count;i++) {
                            if (listmodel.get(i).id == listModel.get(index).id) {
                                listmodel.remove(i,1)
                                break;
                            }
                        }
                        listModel.remove(index,1)
                    }
                }
                CButton {
                    id: editAdd
                    anchors.right: line.left
                    anchors.rightMargin: 24
                    anchors.top: personName.top
                    anchors.topMargin: 2
                    width: 50
                    height: 50
                    iconSource: "qrc:/main/images/edit_add_s.png"
                    backgroundEnabled: false
                    visible: false
                    onClicked: {
                        getData.getAddGroupPersonInfo(getData.getGroupNetType(groupnet),"",groupid)
                        pageStack.push("qrc:/qml/GroupAdd.qml",{"parentqml":groupInfo,"bIsFromGroupInfo":true,
                                           "groupListnum":listModel.count - 2,"networkInfo":groupnet,"groupid":groupid})
                    }
                }
                Image {
                    id: numLock
                    anchors.top: positionText.top
                    anchors.right: line.right
                    width: 40
                    height: 50
                    source: "qrc:/main/images/num_lock.png"
                    visible: false
                }
                MouseArea{
                    id: mousearea
                    anchors.left: parent.left
                    width: parent.width - 110
                    height: parent.height
                    visible: false
                    onPressed: {
                        clickeffects.opacity = 0.5
                    }
                    onReleased: {
                        clickeffects.opacity = 0
                        pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"group","userId":listmodel.get(index).id,
                                           "contactertype":listmodel.get(index).type})
                    }
                    onPositionChanged: {
                        clickeffects.opacity = 0
                    }
                }
                CButton {
                    id: dial
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width - 104
                    anchors.top: parent.top
                    anchors.topMargin: 34
                    height: 72
                    backgroundEnabled: false
                    visible: false
                    iconSource: "qrc:/main/images/icon_call_green.png"
                    onClicked: {
                        var url = "sinuxtel:makeCall?net="+ getData.getFirstNetwork(listmodel.get(index).id) +"&value=" + getData.getFirstNum(listmodel.get(index).id)
                        qApp.openUrl(url)
                    }
                }
                CLine {
                    id: line
                    anchors.top: positionText.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: bIsEdit ? 74 : 0
                    width: bIsEdit ? parent.width - 74 : parent.width
                    visible: false
                    color: "#36465a"
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
        ListModel {
            id: listmodel
        }
        ListModel {
            id: listModel
        }
        ListModel {
            id: memberModel
        }

        ListView {
            id: listview
            anchors.top: titleBar.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: bIsEdit ? 24 : 48
            width: parent.width - 96
            anchors.bottom: bIsEdit ? parent.bottom : (qmlcontains.bIsConversation ? deleteGroup.top : parent.bottom)
            model: listmodel
            clip: true
            delegate: grouplist
            Component.onCompleted: {
                if (qmlcontains.bIsConversation) {
                    listmodel.append({"id":"", "iconvalue":"","infoname":"","name":groupname,"position":groupid,"type":"","bIsLeader":""})
                    listmodel.append({"id":"", "iconvalue":"qrc:/main/images/PersonNum.png","infoname":groupnum,"name":"","position":"","type":"","bIsLeader":""})
                    listmodel.append({"id":"", "iconvalue":"qrc:/main/images/icon_wifi.png","infoname":groupnet,"name":"","position":"","type":"","bIsLeader":""})
                    listModel.append({"id":"", "iconvalue":"","infoname":"","name":groupname,"position":groupid,"type":"","bIsLeader":""})
                    listModel.append({"id":"", "iconvalue":"qrc:/main/images/PersonNum.png","infoname":groupnum,"name":"","position":"","type":"","bIsLeader":""})
                    listModel.append({"id":"", "iconvalue":"qrc:/main/images/icon_wifi.png","infoname":groupnet,"name":"","position":"","type":"","bIsLeader":""})
                    getData.getGroup(groupid)
                } else {
                    listmodel.append({"id":"106","name":"李云龙","position":"营长"})
                    listmodel.append({"id":"107","name":"楚云飞","position":"营长"})
                    listmodel.append({"id":"108","name":"赵子龙","position":"排长"})
                    listmodel.append({"id":"109","name":"关羽","position":"排长"})
                    listmodel.append({"id":"105","name":"荀彧","position":"连长"})
                }
            }
            onContentYChanged: {
                closeSoftKeyboard()
            }
        }

        CButton {
            id: clearText
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.left: parent.left
            width: parent.width
            backgroundEnabled: false
            text: qsTr("清空聊天记录")
            textColor: "#b61f18"
            pixelSize: 40
            height: 50
            onClicked: {
                deleteback.visible = true
                deleteback.opacity = 0.7
                dialog.visible = true
                dialog.isClear = true
                dialog.text = "确认清空聊天记录？"
            }
        }

        Item {
            id: deleteGroup
            anchors.bottom: clearText.top
            anchors.bottomMargin: 20
            anchors.left: parent.left
            width: parent.width
            height: 50
            visible: false
            CButton {
                id: deleteText
                anchors.centerIn: parent
                backgroundEnabled: false
                text: qsTr("设置管理员")
                textColor: "#29afe2"
                pixelSize: 40
                onClicked: {
                    pageStack.push("qrc:/qml/Groupmanagerset.qml",{"groupid":groupid,
                                       "groupname":groupname})
                }
            }
            CLine {
                id: line
                anchors.top: parent.bottom
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 48
                width: parent.width - 96
                color: "#36465a"
            }
        }
    }
    Rectangle {
        id: deleteback
        anchors.fill: parent
        color: "black"
        visible: false
        opacity: 0.3
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
        property alias text: message.text
        property bool isClear: false
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
            height: 32
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            color: "black"
            font.pixelSize: 32
            text: qsTr("是否退出当前编辑页面?")
        }
        CButton {
            id: deletecancel
            anchors.top: message.bottom
            anchors.topMargin: 69
            anchors.left: parent.left
            anchors.leftMargin: 62
            backgroundEnabled: true
            width: 150
            height: 67
            text: "取消"
            textColor: "#ffffff"
            backgroundComponent: Rectangle {
                anchors.fill: parent
                color: "#29afe2"
                radius: 10
            }

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
                if (dialog.isClear)
                {
                    getData.clearGroupMessage(groupid)
                }
                else
                {
                    if (precontains != null)
                        precontains.getgroupname(groupname)
                    if (getData.getLocalInGroupIdentify(groupid) == "creater"){
                        for (var i =0 ;i < memberModel.count;i++){
                            for (var j = 0; j < listModel.count; j++){
                                if (memberModel.get(i).id == listModel.get(j).id)
                                    listModel.remove(j,1)
                            }
                        }
                    }
                }

                pageStack.pop()
                dialog.visible = false
                deleteback.opacity = 0
                deleteback.visible = false
            }
        }
    }
}
