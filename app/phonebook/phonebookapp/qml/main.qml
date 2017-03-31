import QtQuick 2.3
import com.syberos.basewidgets 2.0
import QtMultimedia 5.0

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
    //applicationQuitGestureEnabled: false

    property bool alreadyOpenCamera: false
    property bool alreadyInitGroup: false

    property bool alreadyInitGroupAndPtt: false
    property bool bIsimageCaptureAndSendP2P: false

    signal pttStateChanged(bool state)
    signal popCamera()
    function setPopCamera(state)
    {
        alreadyOpenCamera = state
    }

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

            listView.deselect()
            listmodel.clear()
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
            if (bIsChange){
                if (gScreenInfo.displayHeight == 800)
                    main.height = gScreenInfo.displayHeight
                else
                    main.height = 720
            }
            else
                main.height = 1280
            if (gInputContext.softwareInputPanelVisible || listView.editing) {
                return
            } else {
                respondOnBackKeyReleased = true
            }
        }
    }

    function imageCapture()
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        imageCaptureTimer.start()
    }

    Timer {
        id: imageCaptureTimer
        interval: 3000; repeat: false
        onTriggered: {
            cameraLoader.sourceComponent = cameraComponent
        }
    }

    function imageCaptureAndSendP2P()
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        imageCaptureAndSendP2PTimer.start()
    }

    Timer {
        id: imageCaptureAndSendP2PTimer
        interval: 3000; repeat: false
        onTriggered: {
            bIsimageCaptureAndSendP2P = true
            cameraLoader.sourceComponent = cameraComponent
        }
    }

    function openCamera()
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
            return
        }
        console.log("openCamera   in main.qml")
        var tackPhoto  = pageStack.push(Qt.resolvedUrl("TakePhoto.qml"), {"enableCut": false});
        tackPhoto.imageConfirmed.connect(function(filePath) { //处理信号
            console.log(filePath)
            var imageUrl = "file://" + filePath
            console.log("TakePhoto",imageUrl)
            pageStack.pop(mainroot);
            //chatRoom.statusBarHoldItemColor("black")
            //gScreenInfo.setStatusBar(true)
            //gScreenInfo.setStatusBarStyle("black")
        })
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
                main.height = gScreenInfo.displayHeight
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
            listView.select(10,true)
            listView.deselect()
            listmodel.clear()
            bIsFinish = true
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

    function quitGroupRoom()
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        pageStack.pop()
    }

    function initGroupRoom(myGroupId)
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        var array = new Array
        array = getData.getGroupValue(myGroupId)
        if (array.length < 5)
            return
        //name id false num net
        if (array[4] != "模块数字对讲")
        {
            pageStack.push("qrc:/qml/PptChatRoom.qml",{"groupid":array[1],
                               "groupname":array[0],"bIsPtt":false,"groupnum":array[3],
                               "netinfo":array[4],"isFromEx":true})
        }
        else
        {
            return
        }
    }

    function initP2pGroupRoom()
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        //name id false num net
        var userId;
        var localId;
        localId = getData.getLocalId();
        if (localId == 208)
            userId = 210
        else if (localId == 210)
            userId = 208
        else if (localId == 209)
            userId = 205
        else if (localId == 205)
            userId = 209


        console.log("uuuuuuuuuuuuu",userId)
        pageStack.push("qrc:/qml/PptChatRoom.qml",{"groupid":"p2pgroup_" + userId + "_networktype_"+"1",
                           "groupname":"","bIsPtt":false,
                           "groupnum":"",
                           "netinfo":"自组网",
                           "prepage":mainroot,"bIsP2P":true})
    }

    function stopPtt()
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        alreadyInitGroupAndPtt = false
    }

    function initGroupRoomAndStartPtt(myGroupId)
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        if (alreadyInitGroupAndPtt)
        {
            pttStateChanged(true)
            return;
        }
        var array = new Array
        array = getData.getGroupValue(myGroupId)
        if (array.length < 5)
            return
        //name id false num net
        if (array[4] != "模块数字对讲")
        {
            alreadyInitGroupAndPtt = true;
            pageStack.push("qrc:/qml/PptChatRoom.qml",{"groupid":array[1],
                               "groupname":array[0],"bIsPtt":false,"groupnum":array[3],
                               "netinfo":array[4],"isFromEx":true,"bIsAutoPtt":true})
        }
        else
        {
            return
        }
    }

    function initGroupRoomAndStopPtt(myGroupId)
    {
        if (alreadyOpenCamera) {
            pageStack.pop(mainroot,true)
        }
        pttStateChanged(false)

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
            main.height = gScreenInfo.displayHeight
        else
            main.height = 1280
        bIsPushPersonInfo = true
        pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"local"})
    }

//    camera.imageCapture.capture();

//        var newObject = Qt.createQmlObject('import QtQuick 2.0; Rectangle {color: "red"; width: 20; height: 20}',
//            parentItem, "dynamicSnippet1");

    Loader {
        id: cameraLoader
    }

    Component {
        id: cameraComponent 
        Item {
            Camera {
                id: camera
                focus {
                    focusMode: Camera.FocusAuto
                }
                imageCapture {
                    onImageSaved: {
                        console.log("xxxxxxxxxxxxxxxxxxxxx",path)
                        var imagePath = getData.copyFileToPath(path,"/home/user/DCIM/Camera")

                        if (bIsimageCaptureAndSendP2P)
                        {
                            var userId
                            if (getData.getLocalId() == 210)
                                userId = 208
                            else if (getData.getLocalId() == 208)
                                userId = 210
                            else if (getData.getLocalId() == 209)
                                userId = 205
                            else if (getData.getLocalId() == 205)
                                userId = 209

                            var groupid = "p2pgroup_" + userId + "_networktype_"+"1"
                            getData.onCallP2P("自组网",groupid)
                            getData.sendMessage(2,imagePath,getData.getLocalId(),groupid)

                            bIsimageCaptureAndSendP2P = false
                        }

                        cameraLoader.sourceComponent = null
                    }
                }
                onCameraStateChanged: {
                    cameraTimer.start()
                    //                camera.imageCapture.capture();
                }
            }
            Timer {
                id: cameraTimer
                interval: 1000; repeat: false
                onTriggered: {
                    camera.imageCapture.capture();
                }
            }
        }
    }


//            Camera {
//                id: camera
//                imageCapture {
//                    onImageSaved: {
//                        // Show the preview in an Image
//                        console.log("xxxxxxxxxxxxxxxxxxxxx",path)
//                        getData.copyFileToPath(path,"/home/user/DCIM/Camera")
////                        cameraLoader.sourceComponent = null
//                    }
//                }
////                onCameraStatusChanged: {
////                    if(cameraStatus == 8)
////                    {
////                        camera.imageCapture.capture();
////                    }
////                }
//            }

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
            if (type == 1 || type == 3 || type == 4) {
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
            if (gScreenInfo.displayHeight == 800)
                main.orientationPolicy = CPageOrientation.LockLandscape
            main.color = "black"
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
                        if (gScreenInfo.displayHeight == 800)
                            main.height = gScreenInfo.displayHeight
                        else
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
                titleText: qsTr("联系人")
                titlePixelSize: 30
                titleTextColor: "#f9f9f9"
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
                    textColor: "white"
                    backgroundEnabled: false
                    text: (listView.selectedCount == listView.count) ? "取消" : "全选"
                    iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                    spacing: -68
                    onClicked: {
                        bIsFinish = false
                        if (rightCheck.text == "全选"){
                            listmodel.clear()
                            listView.selectAll()
                            for (var i = 0; i < listView.count; i++) {
                                listmodel.append({"id":contactermodel.getContacterValue(i,"id")})
                            }
                            console.log("qqqqqqqqqqqq",listView.count,listmodel.count)

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
                                            break
                                        }
                                    }
                                }
                            }
                        }
                        bIsFinish = true
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
                width: parent.width
                height: 90
                anchors.top: titleBar.bottom
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
                    var array = new Array
                    if (listmodel.count && listView.editing) {
                        for (var j = 0; j < listmodel.count; j++) {
                            var index = contactermodel.getCurrentIndex(listmodel.get(j).id)
                            if ( index != -1){
                                array.push(index)
                                //listView.select(index,true)
                            }
                        }
                        if (array.length)
                            listView.select(array,true)
                    }
                    bIsFinish = true
                }
            }
            Item {
                id: localNum
                height: 62
                width: parent.width
                anchors.leftMargin: 16
                anchors.top: searchLine.bottom
                visible: listView.editing ? false : true
                Text {
                    id: local
                    font.bold: true
                    font.pixelSize: 30
                    color: "#ffffff"
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
            CLine {
                id: line
                anchors.top: localNum.bottom
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
                width: sectionScroller.visible ? parent.width - 62 : parent.width - 36
                visible: listView.editing ? false : true
                color: "#9b9b9b"
            }
            ListModel {
                id: listmodel
                onCountChanged: {
                    //console.log(count)
                }
            }
            CEditListView {
                id: listView
                anchors.left: parent.left
                anchors.leftMargin: 16
                width: parent.width - 16
                model: contactermodel
                anchors.top: listView.editing ? searchLine.bottom : line.bottom
                anchors.bottom: listView.editing ? editToolBar.top : toolBar.top
                clip: true
                delegate: CEditListViewDelegate {
                    id: listText
                    height: 150
                    width: listView.width
                    editLabelRightMargin: bIsChange ? 1206 : 646
                    editLabelSize: Qt.size(50,50)
                    editLabelComponent: Image {
                        id:checkImage
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        source: listText.checked ? "qrc:/main/images/checkTrue.png" : "qrc:/main/images/checkFalse.png"
                        onSourceChanged: {
                            if (bIsFinish) {
                                if (checkImage.source == "qrc:/main/images/checkTrue.png") {
                                    var contain = 0;
                                    for (var j = 0; j < listmodel.count; j++)
                                    {
                                        if (listmodel.get(j).id == id)
                                        {
                                            contain = 1
                                            break
                                        }
                                    }
                                    if (!contain)
                                        listmodel.append({"id":id})
                                }
                                else if (checkImage.source == "qrc:/main/images/checkFalse.png"){
                                    for (var i = 0; i < listmodel.count; i++) {
                                        if (listmodel.get(i).id == id) {
                                            listmodel.remove(i,1)
                                            break
                                        }
                                    }
                                }
                            }
                        }
                    }
                    onCheckedChanged: {
                        if (sectionScroller.bIsKeyPress) {
                            sectionScroller.bIsKeyPress = false
                            return
                        }
                        if (bIsDelete)
                            return
//                        if (bIsFinish) {
//                            console.log("listView.isSelected",listView.isSelected(index))
//                            if (listView.isSelected(index)) {
//                                listmodel.append({"id":id})
//                            } else {
//                                for (var i = 0; i < listmodel.count; i++) {
//                                    if (listmodel.get(i).id == id) {
//                                        listmodel.remove(i,1)
//                                    }
//                                }
//                            }
//                        }
                    }
                    Item {
                        id: item
                        width: listView.editing ? parent.width - 144 : parent.width - 46
                        height:  150
                        anchors.left: parent.left
                        anchors.leftMargin: listView.editing ? 98 : 0
                        anchors.right: parent.right
                        anchors.rightMargin: sectionScroller.visible ? 46 : 16
                        Image {
                            anchors.top: parent.top
                            width: parent.width
                            height: 142
                            source: "qrc:/main/images/list_line.png"
                        }

                        Text {
                            id: leftText
                            color: "white"
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            anchors.top: parent.top
                            anchors.topMargin: 24
                            font.pixelSize:	40
                            width: 300
                            text: name
                            elide: Text.ElideRight
                            height: 40
                        }
                        Text {
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            anchors.top: leftText.bottom
                            anchors.topMargin: 24
                            font.pixelSize:	30;
                            color: "#b6b6b6"
                            text: position
                            height: 30
                        }
                        MouseArea {
                            id: viewmouse
                            anchors.fill: item
                            visible: listView.editing ? false : true
                            onPressed: {
                                clickeffects.opacity = 0.5
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
                        CButton {
                            id: dial
                            width: 72
                            height: 72
                            backgroundEnabled: false
                            anchors.right: parent.right
                            anchors.rightMargin: 30
                            anchors.verticalCenter: parent.verticalCenter
                            iconSource: pressed ? "qrc:/main/images/dial_click.png" : "qrc:/main/images/dial.png"
                            visible: listView.editing ? false : true
                            onClicked: {
                                if (timecount.running == true )
                                    return
                                timecount.running = true
                                if (getData.getAdhocNetwork(id)) { //sortPhoneNumbers
                                    var url = "sinuxtel:makeCall?net=" + getData.sortPhoneNumbers(id, "net") + "&value=" + getData.sortPhoneNumbers(id, "value")

                                    //var url = "sinuxtel:makeCall?net=自组网&value=" + getData.getAdhocNetwork(id)
                                    qApp.openUrl(url)
                                }
                            }
                        }
                        Rectangle {
                            id: clickeffects
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: parent.height
                            visible: viewmouse.visible
                            opacity: 0
                            color: "black"
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
                home.iconSource: "qrc:/main/images/home.png"
                history.iconSource: "qrc:/main/images/history.png"
                bIsGroup: false
                bIsHistory: false
            }
            MouseArea {
                id: leftslide
                anchors.left: loaderSetting.right
                anchors.top: titleBar.bottom
                anchors.bottom: toolBar.bottom
                width:  bIsChange ? (parent.width - 490) : (parent.width - 360)
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
                            if (gScreenInfo.displayHeight == 800)
                                main.height = 800
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

    Rectangle {
        id: dialog
        visible: false
        anchors.centerIn: parent
        width: 492
        height: 386
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
            text: qsTr("要删除选中的联系人?")
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
            iconSource: pressed ? "qrc:/main/images/add_click.png" : "qrc:/main/images/add.png"
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
            iconSource: pressed ? "qrc:/main/images/setting_click.png" : "qrc:/main/images/setting.png"
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
                    {
                        if (gScreenInfo.displayHeight == 800)
                            main.height = 800
                    }
                    else
                        main.height = 1280
                }
            }
        }
    }
    ParallelAnimation {
        id : set
        NumberAnimation {target: loaderSetting; properties: "x"; from: bIsChange ? -490 : -360; to: 0; duration: 200 }
        NumberAnimation {target: main; properties: "x"; to: bIsChange ? 490 : 360; duration: 200 }
        NumberAnimation {target: main; properties: "y"; to: bIsChange ? 40 : 38; duration: 200 }
        NumberAnimation {target: main; properties: "height"; to: bIsChange ? 640 : 1200; duration: 200 }
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
