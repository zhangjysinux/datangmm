import QtQuick 2.3
import com.syberos.basewidgets 2.0

//text: os.i18n.ctr(qsTr("第二个界面"))
CPageStackWindow {
    id: mainroot
    property alias sendip: ip.text
    property alias newFont: localFont.name
    property bool bIsChange: false/*是否横屏*/
    property string searchText: ""
    property int icurrentindex: 0
    property bool bIsContinue: false/*是否连续按键*/
    property bool bIsFinish: false/*是否搜索完成*/
    property bool bIsDelete: false/*是否删除*/
    property bool bIsPushPersonInfo: false/*是否横屏*/
    respondOnBackKeyReleased: true
    applicationQuitGestureEnabled: false
    onBackKeyReleased: {
        if (gInputContext.softwareInputPanelVisible) {
            closekeyboard()
            if (listView.editing || loaderSetting.status == Loader.Ready) {
                return
            } else {
                respondOnBackKeyReleased = true
            }
        }
        if (listView.editing) {
            listView.editing = false
            editToolBar.visible = false
            toolBar.visible = true
            listmodel.clear()
            if (gInputContext.softwareInputPanelVisible ||
                    loaderSetting.status == Loader.Read) {
                return
            } else {
                respondOnBackKeyReleased = true
            }
        }
        if (loaderSetting.status == Loader.Ready || bIsContinue) {
            if (timecount.running)
                return
            bIsContinue = false
            loaderSetting.source = ""
            main.x = 0
            main.y = 0
            if (bIsChange)
                main.height = 720
            else
                main.height = 1280
            if (gInputContext.softwareInputPanelVisible || listView.editing) {
                return
            } else {
                respondOnBackKeyReleased = true
            }
        }
    }

    function pushCachePage(groupid, groupname, groupnum, netinfo)
    {
        var myPage = pageStack.getCachedPage("qrc:/qml/PptChatRoom.qml",groupid)
        pageStack.push(myPage,{"groupid":groupid,"groupname":groupname,"bIsPtt":false,
                           "groupnum":groupnum, "netinfo":netinfo})
    }

    function setting(flag)
    {
        if (timecount.running)
            return
        leftslide.visible = false
        if (bIsContinue)
            bIsContinue = false
        if (loaderSetting.status == Loader.Ready)
        {
            loaderSetting.source = ""
            main.x = 0
            main.y = 0
            if (bIsChange)
                main.height = 720
            else
                main.height = 1280
        }
        if (flag == 2)
        {
            loadUpdata.sourceComponent = updata
        }
        else if (flag == 4){
            pageStack.push("qrc:/qml/PersonSettingMemory.qml")
        }
        else if (flag == 1){
            toolBar.visible = false
            getData.getSearchContactors(0,2,"")
            listView.positionViewAtBeginning()
            listView.editing = true
        }
    }
    function closekeyboard()
    {
        if (gInputContext.softwareInputPanelVisible)
            closeSoftKeyboard()
    }
    function history()
    {
        pageStack.push("qrc:/qml/CallRecord.qml")
    }
    function initHistory()
    {
        getData.getCallHistoryList()
        pageStack.push("qrc:/qml/CallRecord.qml")
    }

    function initGroup()
    {
        pageStack.push("qrc:/qml/GroupMain.qml")
    }

    function addContacter(net,number)
    {
        pageStack.push("qrc:/qml/AddPerson.qml",{"thenetinfo":net,"thenuminfo":number},true)
    }
    function viewContacter(userId)
    {
        getData.getContacter(userId)
        bIsPushPersonInfo = true
        if (getData.bIsOrganization(userId) == true) {
            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":userId,"contactertype":"1"},true)
        } else {
            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":userId,"contactertype":"2"},true)
        }
    }
    //    function insertMainList(name,position,size,id,type)
    //    {
    //        listModel.append({"name":name,"position":position,"size":size,"id":id,"type":type})
    //    }

    function clearSearchText()
    {
        searchLine.inputtext.text = ""
    }

    function dial()
    {
        qApp.openUrl("sinuxtel:")
    }
    //    function addPersonBackToMain(bIsEdit)
    //    {
    //        bIsAddperson = true
    //        pageStack.pop()
    //        if (searchText != "") {
    //            listModel.clear()
    //            getData.getSearchContactors(0,3,searchText)
    //        } else {
    //            if (bIsEdit) {
    //                listModel.clear()
    //                getData.getSearchContactors(0,3,"")
    //            }
    //        }
    //        sectionScroller.bIsUpdate = true
    //    }
    function pushPersonInfo()
    {
        if (timecount.running)
            return
        leftslide.visible = false
        if (bIsContinue)
            bIsContinue = false
        respondOnBackKeyReleased = true
        loaderSetting.source = ""
        main.x = 0
        main.y= 0
        if (bIsChange)
            main.height = 720
        else
            main.height = 1280
        bIsPushPersonInfo = true
        pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"local"})
    }
    FontLoader {
        id: localFont
        source: "qrc:/font/images/localFont.TTF"
    }

    Text {
        id: ip
        text: ""
        visible: false
        font.family: newFont
    }
    Connections {
        target: getData
        onSignalContacterChanged:{
            if (type == 1 || type == 3) {
                console.log("add add")
                getData.getSearchContactors(0,3,searchText)
                listView.positionViewAtIndex(icurrentindex,ListView.Center)
            } else {
                if (bIsPushPersonInfo) {
                    bIsPushPersonInfo = false
                    contactermodel.removeContacter(id,contactermodel.count())
                }
            }
        }
    }

    Loader { id: loaderSetting;}
    initialPage: CPage{
        id: main
        orientationPolicy: CPageOrientation.Automatic
        Component.onCompleted:	{
            main.color = "#293443"
            gScreenInfo.setStatusBar(true);
            gScreenInfo.setStatusBarStyle("black");
        }
        Connections{
            target: gScreenInfo
            onCurrentOrientationChanged:{
                if (gScreenInfo.currentOrientation == 2 ||
                        gScreenInfo.currentOrientation == 8) {
                    closekeyboard()
                    bIsChange = true
                    toolBar.direction = true
                    main.statusBarHoldEnabled = false
                    gScreenInfo.setStatusBar(false);
                    if (loaderSetting.status == Loader.Ready) {
                        loaderSetting.setSource("qrc:/qml/Setting.qml",{"bIsLandscape":bIsChange})
                        set.restart()
                    } else {
                        main.height = 720
                    }
                } else {
                    closekeyboard()
                    bIsChange = false
                    toolBar.direction = false
                    main.statusBarHoldEnabled = true
                    gScreenInfo.setStatusBar(true);
                    if (loaderSetting.status == Loader.Ready) {
                        loaderSetting.setSource("qrc:/qml/Setting.qml",{"bIsLandscape":bIsChange})
                        set.restart()
                    } else {
                        main.height = 1280
                    }
                }
            }
        }
        contentAreaItem: Item {
            anchors.fill: parent
            CEditTitleBar {
                id:titleBar
                anchors.top: parent.top
                height: 96
                width: parent.width
                leftItemEnabled: true
                rightItemEnabled: true
                leftItemSize: Qt.size(93,74)
                rightItemSize: Qt.size(74,74)
                rightItemBackgroundEnabled: false
                rightItemComponent: rightTitleBar
                leftItemComponent: leftTitleBar
                titleText: qsTr("所有联系人")
                titlePixelSize: 34
                titleTextColor: "#29afe2"
                titleAreaCentered: true
                editView: listView
                editLeftItemLeftMargin: 2
                editLeftComponent: CButton {
                    iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                    backgroundEnabled: false
                    onClicked: {
                        listView.deselect()
                        closeSoftKeyboard()
                        listmodel.clear()
                        listView.editing = false
                        editToolBar.visible = false
                        toolBar.visible = true
                        if (searchLine.inputtext.text == "") {
                            getData.getSearchContactors(0,3,"")
                            listView.positionViewAtBeginning()
                        } else {
                            searchLine.inputtext.text = ""
                        }
                    }
                }
                editRightComponent: CButton {
                    id: rightCheck
                    property bool checked: false
                    anchors.fill: parent
                    pixelSize: 30
                    textColor: "#29afe2"
                    backgroundEnabled: false
                    text: (listView.selectedCount == listView.count) ? "取消" : "全选"
                    iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                    spacing: -68
                    onClicked: {
                        if (rightCheck.text == "全选"){
                            listmodel.clear()
                            listView.selectAll()
                            for (var i = 0; i < listView.count; i++) {
                                listmodel.append({"id":contactermodel.getContacterValue(i,"id")})
                            }
                        }else {
                                listView.deselect()
                            if (searchText == ""){
                                listmodel.clear()
                            }
                            else {
                                for (var m = 0 ; m < contactermodel.count();m++)
                                {
                                    for (var j = 0;j < listmodel.count;j++)
                                    {
                                        if (contactermodel.getContacterValue(m,"id") == listmodel.get(j).id){
                                            listmodel.remove(j)
                                            j = j - 1
                                        }
                                    }
                                }
                            }


                        }
                        //bIsDelete = true
                    }
                }
                spacingBetweenLeftBorderAndLeftItem: 16
                spacingBetweenRightBorderAndRightItem: 16
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
                width: parent.width
                anchors.top: titleBar.bottom
                anchors.bottom: toolBar.top
                Image {
                    sourceSize.height: 4
                    sourceSize.width: 4
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
                id: searchLine
                width: parent.width - 100
                height: 60
                anchors.top: titleBar.bottom
                anchors.topMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                onSignalgetinputtext: {
                    searchText = text
                    if (bIsDelete)
                        bIsDelete = false
                    bIsFinish = false
                    if (listView.editing) {
                        getData.getSearchContactors(0,2,text)
                    } else {
                        getData.getSearchContactors(0,3,text)
                    }
                    listView.positionViewAtBeginning()
                    if (listmodel.count && listView.editing) {
                        for (var j = 0; j < listmodel.count; j++) {
                            var index = contactermodel.getCurrentIndex(listmodel.get(j).id)
                            if ( index != -1)
                                listView.select(index)
                        }
                    }
                    bIsFinish = true
                }
            }
            Item {
                id: localNum
                height: 30
                width: parent.width
                anchors.top: searchLine.bottom
                anchors.topMargin: 37
                visible: listView.editing ? false : true
                Text {
                    id: local
                    font.bold: true
                    font.pixelSize: 30
                    color: "#ffffff"
//                    anchors.left: parent.left
//                    anchors.leftMargin: 26
//                    anchors.verticalCenter: parent.verticalCenter
                    anchors.centerIn: parent
                    text: qsTr("本机号码")
                    font.family: newFont
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        closekeyboard()
                        pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"local"})

                    }
                }
            }
//            CLine {
//                id: line
//                anchors.top: localNum.bottom
//                anchors.topMargin: 16
//                anchors.left: parent.left
//                anchors.leftMargin: 16
//                width: sectionScroller.visible ? parent.width - 62 : parent.width - 36
//                visible: listView.editing ? false : true
//                color: "#36465a"
//            }
            ListModel {
                id: listmodel
                onCountChanged: {
                    console.log(count)
                }
            }
            CEditListView {
                id: listView
                anchors.left: parent.left
                anchors.leftMargin: 16
                width: parent.width - 16
                model: contactermodel
                anchors.top: listView.editing ? searchLine.bottom : localNum.bottom
                anchors.bottom: listView.editing ? editToolBar.top : toolBar.top
                anchors.topMargin: 27
                clip: true
                delegate: CEditListViewDelegate {
                    id: listText
                    height: 112
                    width: listView.width
                    editLabelRightMargin: bIsChange ? 1206 : 646
                    editLabelSize: Qt.size(50,50)
                    editLabelComponent: Image {
                        id:checkImage
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        source: listText.checked ? "qrc:/main/images/checkTrue.png" : "qrc:/main/images/checkFalse.png"
                    }
                    onCheckedChanged: {
                        if (sectionScroller.bIsKeyPress) {
                            sectionScroller.bIsKeyPress = false
                            return
                        }
                        if (bIsDelete)
                            return
                        if (bIsFinish) {
                            if (listView.isSelected(index)) {
                                listmodel.append({"id":id})
                            } else {
                                for (var i = 0; i < listmodel.count; i++) {
                                    if (listmodel.get(i).id == id) {
                                        listmodel.remove(i,1)
                                    }
                                }
                            }
                        }
                    }
                    Item {
                        id: item
                        width: listView.editing ? parent.width - 144 : parent.width - 46
                        height:  112
                        anchors.left: parent.left
                        anchors.leftMargin: listView.editing ? 98 : 0
                        anchors.right: parent.right
                        anchors.rightMargin: sectionScroller.visible ? 46 : 16
//                        Image {
//                            anchors.top: parent.top
//                            width: parent.width
//                            height: 142
//                            source: "qrc:/main/images/list_line.png"
//                        }

                        Text {
                            id: leftText
                            color: "white"
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            anchors.top: parent.top
                            anchors.topMargin: 21
                            font.pixelSize:	32
                            width: 300
                            text: name
                            elide: Text.ElideRight
                            height: 32
                        }
                        Text {
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            anchors.top: leftText.bottom
                            anchors.topMargin: 20
                            font.pixelSize:	22;
                            color: "#ffffff"
                            text: position
                            height: 22
                        }
                        MouseArea {
                            id: viewmouse
                            anchors.fill: item
                            visible: listView.editing ? false : true
                            onPressed: {
                                clickeffects.opacity = 0.3
                            }
                            onReleased: {
                                clickeffects.opacity = 0
                                bIsPushPersonInfo = true
                                closekeyboard()
                                getData.getContacter(id)
                                icurrentindex = contactermodel.getCurrentIndex(id)
                                pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":id,"contactertype":contactermodel.getContacterValue(index,"type")})
                            }
                            onPositionChanged: {
                                clickeffects.opacity = 0
                            }
                        }
//                        CButton {
//                            id: dial
//                            width: 72
//                            height: 72
//                            backgroundEnabled: false
//                            anchors.right: parent.right
//                            anchors.rightMargin: 30
//                            anchors.verticalCenter: parent.verticalCenter
//                            iconSource: pressed ? "qrc:/main/images/dial_click.png" : "qrc:/main/images/dial.png"
//                            visible: listView.editing ? false : true
//                            onClicked: {
//                                if (timecount.running == true )
//                                    return
//                                timecount.running = true
//                                if (getData.getAdhocNetwork(id)) {
//                                    var url = "sinuxtel:makeCall?net=自组网&value=" + getData.getAdhocNetwork(id)
//                                    qApp.openUrl(url)
//                                }
//                            }
//                        }
                        Rectangle {
                            id: clickeffects
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: parent.height
                            visible: viewmouse.visible
                            opacity: 0
                            color: "#546e8f"
                        }
                        CLine {
                            id: delegateline
                            anchors.top: parent.bottom
//                            anchors.topMargin: -3
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            width: sectionScroller.visible ? parent.width - 62 : parent.width - 36
                            color: "#36465a"
                        }
                    }
                }
                section {
                    property: "size"; //criteria: ViewSection.FirstCharacter
                    criteria: ViewSection.FirstCharacter
                    delegate: (bIsChange || searchText != "") ? null : sectionfill
                }
                onMovementStarted: {
                    closeSoftKeyboard()
                }

                onEditingChanged: {
                    if (listView.editing) {
                        editToolBar.visible = true
                    } else {
                        editToolBar.visible = false
                        toolBar.visible = true
                        if (mainroot.respondOnBackKeyReleased == false)
                            mainroot.respondOnBackKeyReleased = true
                    }
                }
            }
            MySectionScroller {
                id: sectionScroller
                listView: listView
                anchors.left: parent.left
                anchors.leftMargin: 8
                width: 30
                height: listView.height
                visible: (bIsChange || searchText != ""
                          || gInputContext.softwareInputPanelVisible) ? false : true
            }
            CToolBar {
                id: editToolBar
                visible: listView.editing ? true : false
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
                    textColor: "#29afe2"
                    pixelSize: 32
                    onClicked: {
                        if (listView.selectedCount == 0){
//                            searchLine.inputtext.text = ""
//                            listView.deselect()
//                            listView.editing = false
//                            getData.getSearchContactors(0,3,searchLine.inputtext.text)
//                            listView.positionViewAtBeginning()
//                            toolBar.visible = true
//                            listmodel.clear()
//                            dialog.visible = false
//                            deleteback.opacity = 0
//                            deleteback.visible = false
                            return
                        }

                        deleteback.visible = true
                        dialog.visible = true
                        deleteback.opacity = 0.7


                    }
                }
            }
            Menu {
                id: toolBar
                anchors.bottom: parent.bottom
                height: 120
                width: parent.width
                group.iconSource: "qrc:/main/images/group.png"
                home.iconSource: "qrc:/main/images/person_click.png"
                history.iconSource: "qrc:/main/images/history.png"
                bIsGroup: false
                bIsHistory: false
            }
            MouseArea {
                id: leftslide
                anchors.left: loaderSetting.right
                anchors.top: titleBar.bottom
                anchors.bottom: toolBar.bottom
                width:  parent.width//bIsChange ? (parent.width - 490) : (parent.width - 360)
                visible: false
                drag.target: main
                drag.axis: Drag.XAxis
                drag.minimumX: 0
                drag.maximumX: bIsChange ? 490 : 360
                drag.minimumY: 0
                drag.maximumY: 0
                onPositionChanged: {
                    //                    if (bIsContinue)
                    //                        bIsContinue = false
                    loaderSetting.x = main.x - loaderSetting.width
                    if (bIsChange) {
                        main.y = 40*main.x/490
                        main.height = (40 - main.y)*2+640
                    } else {
                        main.y = 38*main.x/360
                        main.height = (40 - main.y)*2+ 1200
                    }
                }
                onReleased: {
                    if (bIsChange) {
                        if (main.x > 245) {
                            main.x = 490
                            main.y = 40
                            main.height = 640
                        } else {
                            main.x = 0
                            main.y = 0
                            main.height = 720
                            if (bIsContinue)
                                bIsContinue = false
                        }
                    } else {
                        if (main.x > 180) {
                            main.x = 360
                            main.y = 38
                            main.height = 1200
                        } else {
                            main.x = 0
                            main.y = 0
                            main.height = 1280
                            if (bIsContinue)
                                bIsContinue = false
                        }
                    }
                    loaderSetting.x = main.x - loaderSetting.width
                    if (main.x == 0) {
                        loaderSetting.source = ""
                        leftslide.visible = false
                        respondOnBackKeyReleased = true
                    }
                }
            }
        }
        Loader {id: loadUpdata; z: 3; anchors.fill: parent}
    }
    Rectangle {
        id: deleteback
        anchors.fill: parent
        color: "black"
        visible: false
        opacity: 0
        MouseArea {
            anchors.fill: parent
            onClicked: {
                deleteback.visible = false
                dialog.visible = false
                deleteback.opacity = 0
            }
        }
    }
    Component {
        id: sectionfill
        Item {
            width: listView.width - 48
            height: 56
            Rectangle {
                width: parent.width
                height: 56
                color: "#546e8f"
                opacity: 0.3
                Text {
                    anchors { left: parent.left; leftMargin: 26; verticalCenter: parent.verticalCenter }
                    text: section; font.bold: true
                    font.pixelSize: 30
                    color: "#ffffff"
                }
            }
        }
    }

    Rectangle {
        id: dialog
        visible: false
        anchors.centerIn: parent
        width: 504
        height: 296

        Text {
            id: message
            anchors.top: parent.top
            anchors.topMargin: 79
            height: 33
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            color: "black"
            font.pixelSize: 32
            text: qsTr("要删除选中的联系人?")
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
                for (var i = 0; i < listmodel.count; i++){
                    getData.removeContacter(listmodel.get(i).id)
                }
                searchLine.inputtext.text = ""
                listView.deselect()
                listView.editing = false
                getData.getSearchContactors(0,3,searchLine.inputtext.text)
                listView.positionViewAtBeginning()
                toolBar.visible = true
                listmodel.clear()
                dialog.visible = false
                deleteback.opacity = 0
                deleteback.visible = false

            }
        }
    }

    Component {
        id: rightTitleBar
        CButton {
            anchors.fill: parent
            backgroundEnabled: false
            text: "添加"
            textColor: "#29afe2"
            //iconSource: pressed ? "qrc:/main/images/add_click.png" : "qrc:/main/images/add.png"
            onClicked: {
                closekeyboard()
                pageStack.push("qrc:/qml/AddPerson.qml",{"containerqml":mainroot})
            }
        }
    }
    Component {
        id: leftTitleBar
        CButton {
            anchors.fill: parent
            backgroundEnabled: false
            iconSource: "qrc:/main/images/icon_set.png"
            onClicked: {
                if (timecount.running)
                    return
                bIsContinue = !bIsContinue
                console.log(timecount.running,bIsContinue)
                timecount.start()
                if (bIsContinue) {
                    closekeyboard()
                    mainroot.respondOnBackKeyReleased = false
                    searchLine.inputtext.text = ""
                    loaderSetting.setSource("qrc:/qml/Setting.qml",{"bIsLandscape":bIsChange})
                    set.start()
                    leftslide.visible = true
                } else {
                    leftslide.visible = false
                    loaderSetting.source = ""
                    main.x = 0
                    main.y = 0
                    if (bIsChange)
                        main.height = 720
                    else
                        main.height = 1280
                }
            }
        }
    }
    ParallelAnimation {
        id : set
        NumberAnimation {target: loaderSetting; properties: "x"; from: bIsChange ? -490 : -360; to: 0; duration: 200 }
//        NumberAnimation {target: main; properties: "x"; to: bIsChange ? 490 : 360; duration: 200 }
//        NumberAnimation {target: main; properties: "y"; to: bIsChange ? 40 : 38; duration: 200 }
//        NumberAnimation {target: main; properties: "height"; to: bIsChange ? 640 : 1200; duration: 200 }
    }
    Component {
        id: updata
        Rectangle {
            anchors.fill: parent
            color: Qt.rgba(0,0,0,0.5)
            AnimatedImage {
                anchors.centerIn: parent
                source: "qrc:/main/images/loading.gif"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    loadUpdata.sourceComponent = null
                }
            }
        }
    }
    Timer {
        id: timecount
        interval: 500
        repeat: false
        running: true
        onTriggered: {
        }
    }
}
