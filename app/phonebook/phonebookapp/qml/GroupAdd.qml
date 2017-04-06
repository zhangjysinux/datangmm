import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: groupAdd
    property string groupid: ""
    property bool bIsSwitchOn: false
    property string networkInfo: "自组网"
    property int creategroupid: -1
    property string inputName: ""
    property int selectenum : 0
    property bool bIsChange: false /*是否横屏*/
    property bool bIsFromGroupInfo: false /*是否从群组详情界面迁移过来*/
    property bool bIsFinish: true /*搜索是否完成*/
    property var parentqml: null
    property bool bIsInput: false /*是否输入*/
    property string searchText: ""
    function addGroupPerson(bIsAdd)
    {
        listview.editable = bIsAdd
        listview.editing = bIsAdd
    }
    Keys.onBackPressed: {
        if (bIsFromGroupInfo && listview.editing)
            pageStack.pop()
    }
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            groupAdd.orientationPolicy = CPageOrientation.LockLandscape
        groupAdd.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            groupAdd.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            bIsChange = true
        } else {
            groupAdd.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            bIsChange = false
        }
        if (bIsFromGroupInfo) {
            addGroupPerson(true)
            //getData.getAddGroupPersonInfo(getData.getGroupNetType(networkInfo),"",groupid)
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                groupAdd.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                bIsChange = true
            } else {
                groupAdd.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                bIsChange = false
                keyboardground.visible = false
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
            leftItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemBackgroundEnabled: true
            leftItemComponent: leftTitle
            rightItemEnabled: true
            rightItemBackgroundEnabled: true
            rightItemSize: Qt.size(74,74)
            rightItemComponent: rightTitle
            editView: listview
            editLeftItemLeftMargin: 2
            editLeftComponent: CButton {
                iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                backgroundEnabled: false
                onClicked: {
                    closeSoftKeyboard()
                    listModel.clear()
                    listview.deselect()
                    getData.clearGroupMembers()
                    if (bIsFromGroupInfo) {
                        pageStack.pop()
                    } else {
                        addGroupPerson(false)
                        searchline.inputtext.text = ""
                    }
                }
            }
            editRightComponent: CButton {
                id: rightButton
                anchors.fill: parent
                pixelSize: 30
                textColor: "white"
                backgroundEnabled: false
                text: qsTr("确认")
                iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                spacing: -68
                property bool checked: false
                onClicked: {
                    closeSoftKeyboard()
                    for (var i = 0; i < listModel.count; i++) {
                        if (bIsFromGroupInfo) {
                            parentqml.theGroupAddPerson(listModel.get(i).id,
                                                        listModel.get(i).name,
                                                        listModel.get(i).position)
                        } else {
                            if (listModel.count >= 0)
                                getData.setGroupMembers(listModel.get(i).id)
                        }
                    }
                    if (!bIsFromGroupInfo && listModel.count >= 0){
                        console.log("groupname === ",inputName,networkInfo)
                        // by michael zheng 2017.3.7 for add input groupid
                        //getData.addGroup(inputName,networkInfo,null,1)
                        console.log("------------------群组编号:"+creategroupid+"----")
                        getData.addGroup(inputName,networkInfo,null,1,creategroupid)
                        // end by michael zheng
                        pageStack.pop()
                        listModel.clear()
                    }
                }
            }
            titlePixelSize: 30
            titleText: qsTr("选择网络")
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
        SearchLine {
            id:searchline
            anchors.top: titleBar.bottom
            width: parent.width
            height: 90
            visible: listview.editing ? true : false
            onSignalgetinputtext: {
                searchText = text
                bIsFinish = false
                var array = new Array
                getData.getAddGroupPersonInfo(getData.getGroupNetType(networkInfo),text,groupid,array)
                listview.positionViewAtBeginning()
                if (listModel.count) {
                    for (var j = 0; j < listModel.count; j++) {
                        var index = groupPersonModel.getCurrentIndex(listModel.get(j).id)
                        if ( index != -1)
                            listview.select(index)
                    }
                }
                bIsFinish = true
            }
        }
        //        Component {
        //            id: switchOnOff
        //            Item {
        //                width: 148
        //                height: 40
        //                BorderImage {
        //                    id: backgroup
        //                    source: "qrc:/group/images/switch.png"
        //                    width: parent.width
        //                    height: parent.height
        //                    border.left: 5
        //                    border.top: 5
        //                    border.right: 5
        //                    border.bottom: 5
        //                }
        //                Image {
        //                    id: switchOn
        //                    width: 74
        //                    height: parent.height
        //                    source: "qrc:/group/images/on.png"
        //                    opacity: 0
        //                }
        //                Image {
        //                    id: switchOff
        //                    anchors.left: switchOn.right
        //                    width: 74
        //                    height: parent.height
        //                    source: "qrc:/group/images/off.png"
        //                    opacity: 1
        //                }
        //                MouseArea {
        //                    anchors.fill: parent
        //                    onClicked: {
        //                        bIsSwitchOn = !bIsSwitchOn
        //                        if (bIsSwitchOn) {
        //                            switchOff.opacity = 1
        //                            switchOn.opacity = 0
        //                        } else {
        //                            switchOff.opacity = 0
        //                            switchOn.opacity = 1
        //                        }
        //                    }
        //                }
        //            }
        //        }
        Component {
            id: addGroupList
            Item {
                height: 71
                width: listview.width
                Text {
                    id: thename
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    width: 130
                    height: 30
                    clip: true
                    font.pixelSize: 30
                    text: thenameinfo
                    color: "white"
                    onTextChanged: {
                        //                        if (thename.text == "是否统一网络") {
                        //                            thename.width = 190
                        //                            switchButton.visible = true
                        //                            rec.visible = false
                        //                        } else
                        if (thename.text == "群组名称"||thename.text == "群组编号") {
                            input.visible = true
                            rec.visible = false
                            input.text = inputName
                        } else if (thename.text == "模块数字对讲") {
                            thename.width = 190
                        }
                    }
                }
                Rectangle {
                    id: rec
                    width: parent.width
                    height: parent.height
                    opacity: opacitynum
                    visible: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (thename.text == "自组网" || thename.text == "模块数字对讲" ||
                                thename.text == "LTE" || thename.text == "默认" || thename.text == "WIFI") {
                            networkadd.set(selectenum,{"opacitynum":"0"})
                            networkadd.set(index,{"opacitynum":"0.3"})
                            selectenum = index
                            networkInfo = thename.text
                            if (gInputContext.softwareInputPanelVisible) {
                                closekeyboard()
                            }
                        }
                    }
                }

                TextInput {
                    id: input
                    anchors.top: parent.top
                    anchors.topMargin: 5
//                    anchors.bottom: thename.bottom
                    anchors.left: thename.right
                    anchors.leftMargin: 30
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    height: 71
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: 30
                    visible: false
                    clip: true
                    color: "#ffffff"
//                    textColor: "#ffffff"
//                    cursorColor: "#ffffff"
                    wrapMode: TextEdit.Wrap
                    maximumLength: 60
                    onTextChanged: {
                        if (thename.text == "群组名称")
                        {
//                            input.validator = RegExpValidator.regExp = /^[0-9]{16}$/
                            console.log("onTextChanged " , input.text)
                            if (bIsInput) {
                                keyboardground.inputinfo = input.text
                            }
                            inputName = input.text
                        }
                        if (thename.text == "群组编号")
                        {
//                            input.validator = (RegExpValidator.regExp=/^[0-9]{18}$/)
//                            validator: RegExpValidator{regExp: /^[0-9]{18}$/}
                            console.log("onTextChanged " , input.text)
                            if (bIsInput) {
                                keyboardground.inputinfo = input.text
                            }
                            creategroupid = input.text
                        }
                    }

                    onFocusChanged: {
                        console.log("tttttttt ",bIsChange,bIsInput)
                        if (bIsChange) {
                            bIsInput = !bIsInput
                            console.log("bisinput xxxx === ",bIsInput)
                            if (bIsInput) {
                                keyboardground.visible = true
                                keyboardground.inputinfo = input.text
                            } else {
                                closeSoftKeyboard()
                                keyboardground.visible = false
                                keyboardground.inputinfo = ""
                            }
                        }
                    }
                }
                //                Loader {
                //                    id: switchButton
                //                    anchors.top: parent.top
                //                    anchors.topMargin: 24
                //                    anchors.right: line.right
                //                    visible: false
                //                    sourceComponent: switchOnOff
                //                }
                CLine {
                    id: line
                    anchors.top: thename.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    width: parent.width
                    color: "#9b9b9b"
                }
            }
        }
        ListModel {
            id: listModel
        }
        ListModel {
            id: networkadd
        }
        CEditListView {
            id: listview
            anchors.top: listview.editing ? searchline.bottom : titleBar.bottom
            anchors.topMargin: listview.editing ? 0 : (bIsChange ? 24 : 8)
            anchors.left: parent.left
            anchors.leftMargin: listview.editing ? 24 : (bIsChange ? 48 : 60)
            anchors.bottom: parent.bottom
            width: listview.editing ? parent.width - 24 : (bIsChange ? parent.width - 96 : parent.width - 108)
            model: listview.editing ? groupPersonModel : networkadd
            editable: false
            clip: true
            delegate: listview.editing ? listText : addGroupList
            section {
                property: "size";
                criteria: ViewSection.FirstCharacter
                delegate: (bIsChange || searchText != "") ? null : sectionfill
            }
            Component.onCompleted: {
                //networkadd.append({"thenameinfo":"默认","opacitynum":"0.3"})
                networkadd.append({"thenameinfo":"自组网","opacitynum":"0.3"})
                networkadd.append({"thenameinfo":"LTE","opacitynum":"0"})
                //networkadd.append({"thenameinfo":"模块数字对讲","opacitynum":"0"})
                networkadd.append({"thenameinfo":"WIFI","opacitynum":"0"})
                //networkadd.append({"thenameinfo":"是否统一网络"})
                networkadd.append({"thenameinfo":"群组名称"})

                // by michael zheng 2017.3.7 for add groupid
                networkadd.append({"thenameinfo":"群组编号"})
                // by michael zheng
            }
            //            onContentYChanged: {
            //                if(bIsFinish)
            //                    closeSoftKeyboard()
            //            }
        }
        MySectionScroller {
            id: sectionScroller
            listView: listview
            anchors.left: parent.left
            anchors.leftMargin: 8
            width: 30
            height:listView.height
            visible: (bIsChange || gInputContext.softwareInputPanelVisible ||
                      searchText != "") ? false : true
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
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.topMargin: 16
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignLeft
                        text: section
                        font.bold: true
                        font.pixelSize: 30
                        color: "#ffffff"
                    }
                }
            }
        }
        Component {
            id: listText
            CEditListViewDelegate {
                id: checkIcon
                height: 150
                width: listview.width
                editLabelRightMargin: bIsChange ? 1206 : 646
                editLabelSize: Qt.size(50,50)
                editLabelComponent: Image {
                    id:checkImage
                    width: 50
                    height: 50
                    source: (checkIcon.checked) ? "qrc:/main/images/checkTrue.png" : "qrc:/main/images/checkFalse.png"
                }
                onCheckedChanged: {
//                    if (sectionScroller.bIsKeyPress) {
//                        sectionScroller.bIsKeyPress = false
//                        return
//                    }
                    if (bIsFinish) {
                        if (listview.isSelected(index)) {
                            listModel.append({"id":id,"name":personName,"position":position})
                        } else {
                            for (var i = 0; i < listModel.count; i++) {
                                if (listModel.get(i).id == id) {
                                    listModel.remove(i,1)
                                }
                            }
                        }
                    }
                }
                Item {
                    height: 150
                    width: parent.width - 120
                    anchors.left: parent.left
                    anchors.leftMargin: 74
                    anchors.right: parent.right
                    anchors.rightMargin: sectionScroller.visible ? 46 : 16
                    Image {
                        anchors.left: parent.left
                        height: 142
                        width: parent.width
                        source: "qrc:/main/images/list_line.png"
                    }
                    Text {
                        id: text1
                        anchors.top: parent.top
                        anchors.topMargin: 24
                        anchors.left: parent.left
                        anchors.leftMargin: 24
                        width: 200
                        height: 40
                        font.pixelSize: 40
                        color: "white"
                        text: personName
                    }
                    Text {
                        id: text2
                        anchors.top: text1.bottom
                        anchors.topMargin: 24
                        anchors.left: parent.left
                        anchors.leftMargin: 24
                        width: 200
                        height: 30
                        font.pixelSize: 30
                        color: "white"
                        text: position
                    }
                }
            }
        }
        Component {
            id: leftTitle
            CButton {
                anchors.fill: parent
                iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                backgroundEnabled: false
                onClicked: {
                    closeSoftKeyboard()
                    pageStack.pop()
                }
            }
        }
        Component {
            id: rightTitle
            CButton {
                anchors.fill: parent
                pixelSize: 30
                textColor: "white"
                backgroundEnabled: false
                text: qsTr("确认")
                iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                spacing: -68
                onClicked: {
                    closeSoftKeyboard()
                    // by michael zheng 2017.3.7
//                    if (inputName != "" && networkInfo != "") {
                    if (inputName != "" && networkInfo != "" &&creategroupid != -1) {
                        // end by michael zheng
                        getData.clearGroupMembers()
                        var array = new Array
                        getData.getAddGroupPersonInfo(getData.getGroupNetType(networkInfo),"",groupid,array)
                        addGroupPerson(true)
                    } else {
                        sureback.visible = true
                        sureback.opacity = 0.7
                        suredialog.visible = true
                    }
                }
            }
        }
        Rectangle {
            id: keyboardground
            property alias inputinfo: inputt.text
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            anchors.left: parent.left
            anchors.leftMargin: 48
            width: parent.width - 96
            height: 100
            border.width: 5
            visible: false
            Image {
                anchors.fill: parent
                source: "qrc:/main/images/editinput.png"
            }
            Text {
                id: inputt
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 45
                horizontalAlignment: Qt.AlignLeft
                color: "white"
                text: ""
            }
        }
    }
    Rectangle {
        id: sureback
        anchors.fill: parent
        visible: false
        color: "black"
        opacity: 0
        z: 4
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
        z: 4
        Image {
            anchors.fill: parent
            source: "qrc:/bluetooth/images/bluetoothblack.png"
        }
        Text {
            id: message
            anchors.top: parent.top
            anchors.topMargin: 80
            height: 80
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            color: "white"
            font.pixelSize: 40
            text: qsTr("请输入完整的群组信息!")
        }
        CButton {
            id: surebutton
            anchors.top: message.bottom
            anchors.topMargin: 60
            anchors.left: parent.left
            anchors.leftMargin: 159
            width: 174
            height: 100
            backgroundEnabled: false
            iconSource: pressed ? "qrc:/history/images/sureclick.png" :
                                  "qrc:/history/images/suredelete.png"
            onClicked: {
                sureback.visible = false
                suredialog.visible = false
                sureback.opacity = 0
            }
        }
    }
}
