import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.filemanager.filepicker 1.0
import QtMultimedia 5.1

CPage {
    id: chatRoom
    property string groupid: "0"
    property string groupname: "红色尖兵"
    property string groupnum: "15"
    property string netinfo: ""
    property var prepage: null
    property int contentYOnFlickStarted: 0
    property int syncCount: 0
    property bool isFresh: false
    property bool isbottom: false
    property bool bIsPtt: false
    property bool isFromTalk: false
    property var myindex: 0
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            chatRoom.orientationPolicy = CPageOrientation.LockLandscape
        syncCount = getData.getSyncCount(groupid)
        if (syncCount)
            sync.visible = true
        else
            sync.visible = false
        console.log("getSyncCount",syncCount)
        //getData.getImageMessageList()
        chatRoom.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            chatRoom.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            voice.iconSource = voice.pressed ? "qrc:/group/images/group-voice-h_click.png" : "qrc:/group/images/group-voice-h.png"
            voice.width = 1040
            textBorder.width = 1097
        } else {
            chatRoom.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            voice.iconSource = voice.pressed ? "qrc:/group/images/group-voice_click.png" : "qrc:/group/images/group-voice.png"
            voice.width = 480
            textBorder.width = 537
        }
    }
    Connections {
        target: gInputContext
        onSoftwareInputPanelVisibleChanged: {
            console.log("softwareInputPanelVisible==============",gInputContext.softwareInputPanelVisible)
            if(gInputContext.softwareInputPanelVisible) //弹起：softwareInputPanelVisible为true；否则为false
                bottomBar.height = 120
            timer.start()
        }
    }

    Connections {
        target: getData
        onSignalGroupChanged: {
            if (groupid == id)
            {
                groupname = name
                groupnum = num
                titleBar.titleItem.myName = (groupnum == "") ? groupname : groupname + "(" + groupnum + ")"
            }
        }
    }

    Timer {
        id: timer
        interval: 250; repeat: false
        onTriggered: {
            listView.anchors.bottomMargin = 50
            listView.positionViewAtEnd()
        }
    }

    function getgroupname(name)
    {
        groupname = name
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged: {
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                chatRoom.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                voice.iconSource = voice.pressed ? "qrc:/group/images/group-voice-h_click.png" : "qrc:/group/images/group-voice-h.png"
                voice.width = 1040
                textBorder.width = 1097
            } else {
                chatRoom.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                voice.iconSource = voice.pressed ? "qrc:/group/images/group-voice_click.png" : "qrc:/group/images/group-voice.png"
                voice.width = 480
                textBorder.width = 537
            }
        }
    }

    Connections {
        target: getData
        onSignalFreshMessage: {
            //getData.getMessageList(groupid)
            listView.positionViewAtEnd()
        }
    }
    Connections {
        target: m_messageModel
        onSignalFreshEnd: {
            listView.positionViewAtEnd()
        }
    }

    Connections {
        target: getData
        onSignalMessageChanged: {
            //flag 0:time  1:text 2:picture 3:files 4:audio 5:video
            console.log("messagestate = ",messageState,messageHandle,msg)
            var direction = ""
            var photoPath = "";
            if (getData.getLocalId() == userId)
            {
                direction = "right"
                photoPath = getData.getLocalPhoto();
            }
            else
            {
                direction = "left"
                photoPath = getData.getContacterPhoto(userId)
            }
            if (photoPath == "null")
                photoPath == "qrc:/main/images/init_photo.png"

            if (messageHandle == 0)
                listModel.insert(0, {"userId":userId,"flag":messageType,"messageState":messageState,"fileName":fileName,"text":msg,"direction":direction,"messId":id,"photoPath":photoPath})
            else if (messageHandle == 1)
                listModel.append({"userId":userId,"flag":messageType,"messageState":messageState,"fileName":fileName,"text":msg,"direction":direction,"messId":id,"photoPath":photoPath})
            else if (messageHandle == 2)
            {
                for (var i = 0;i < listModel.count; i++)
                {
                    if (listModel.get(i).messId == id)
                    {
                        listModel.remove(i, 1)
                        break
                    }
                }
            }
            else if (messageHandle == 3)
            {
                for (var i = 0;i < listModel.count; i++)
                {
                    if (listModel.get(i).messId == id)
                    {
                        listModel.remove(i,1)
                        listModel.insert(i,{"flag":messageType,"messageState":messageState,"fileName":fileName,"text":msg,"direction":direction,"messId":id,"photoPath":photoPath})
                        //listModel.set(i,{"flag":messageType,"messageState":messageState,"fileName":fileName,"text":msg,"direction":direction,"messId":id,"photoPath":photoPath})
                        break
                    }
                }
            }
            //listView.currentIndex = listView.count - 1;
            listView.positionViewAtEnd()
        }
    }
    contentAreaItem: Item {
        CButton {
            id: sync
            width: 180
            height: 72
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 300
            z: 10
            backgroundEnabled: false
            iconSource: pressed ? "qrc:/group/images/unread-click.png" : "qrc:/group/images/unread.png"
            onClicked: {
                getData.clearSyncCount(groupid)
                listView.positionViewAtIndex(getData.getMessageCount() - syncCount,listView.Center)
            }
        }

        CTitleBar {
            id:titleBar
            anchors.top: parent.top
            height: 96
            width: parent.width
            leftItemEnabled: true
            rightItemEnabled: true
            leftItemSize: Qt.size(93,74)
            rightItemSize: Qt.size(74,74)
            rightItemBackgroundEnabled: false
            leftItemComponent: leftTitleBar
            rightItemComponent: rightTitleBar
            titleItemComponent: Component {
                id: titile
                Item {
                    Row {
                        anchors.centerIn: parent
                        visible: true
                        spacing: 0
                        height: 50
                        width: 244
                        CButton {
                            id: button1
                            width: 122
                            height: 50
                            backgroundEnabled: false
                            iconSource: "qrc:/digital/images/digital-button1-up.png"
                            onClicked: {
                                if (isFromTalk)
                                    pageStack.pop()
                                else
                                    pageStack.push("qrc:/qml/DigitalTalk.qml",{"bIsGroup":true,"isFromText":true,"groupid":groupid,"netinfo":netinfo})
                            }
                        }
                        CButton {
                            id: button2
                            width: 122
                            height: 50
                            backgroundEnabled: false
                            iconSource: "qrc:/digital/images/digital-button2-up.png"
                            onClicked: {
                            }
                        }
                    }
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
            Component.onCompleted: {

            }
        }

        Item {
            id: mid
            width: parent.width
            anchors.top: parent.top
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
        ListModel {
            id: listModel
        }

        ListView {
            id: listView
            spacing: 24
            width: parent.width
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            //            anchors.bottom: parent.bottom
            //            anchors.bottomMargin: 120
            anchors.bottom: bottomBar.top
            anchors.bottomMargin: 50
            delegate: lineDelegate
            model: m_messageModel
            header: Component{
                Item{
                    visible: false
                    id: pv
                    width: listView.width
                    height: 100
                    clip: true
                    Text {
                        text: qsTr("松开刷新")
                        anchors.centerIn: parent
                        color: "#ffffff"
                    }
                    //Rectangle{width:pv.width; height:pv.height; color: 'green'}
                }
            }
            Component.onCompleted: {
                //getData.onGetMessageList(groupid)
                //positionViewAtEnd()
                //index = listModel.count
                positionViewAtEnd()

            }
            onMovementStarted: {
                if(gInputContext.softwareInputPanelVisible)
                    mainroot.closeSoftKeyboard()
            }

            //            onFlickStarted: {
            //                contentYOnFlickStarted = contentY

            //            }
            //            onFlickEnded: {
            //                headerItem.visible = false
            //                if (isFresh){
            //                    console.log("end",contentY)
            //                    isFresh = false
            //                    getData.onGetMessageList(groupid)
            //                }
            //            }
            //            onContentYChanged: {
            //                // 下拉刷新判断逻辑：已经到头了，还下拉一定距离
            //                if (contentY < originY){
            //                    var dy = contentYOnFlickStarted - originY;
            //                    if (dy < -200){
            //                        isFresh = true;
            //                        headerItem.visible = true
            //                    }
            //                }
            //            }
        }


        Item {
            id: bottomBar
            width: parent.width
            height: 120
            anchors.bottom: parent.bottom
            Image {
                anchors.fill: parent
                source: "qrc:/main/images/top_line.png"
            }
            Item {
                height: 120
                width: parent.width
                anchors.top: parent.top
                CButton {
                    visible: false
                    id: button1
                    height: 72
                    width: 72
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    //anchors.verticalCenter: parent.verticalCenter
                    backgroundEnabled: false
                    iconSource: bIsPtt ? (pressed ? "qrc:/group/images/chatroom-leftbottom-key_click.png": "qrc:/group/images/chatroom-leftbottom-key.png")
                                       : (pressed ? "qrc:/chatroom/images/chatroom_leftbottom.png_click" : "qrc:/chatroom/images/chatroom_leftbottom.png")
                    onClicked: {
                        if (voice.visible == false){
                            if (bIsPtt) {
                                textBorder.visible = false
                                button3.visible = false
                            }
                            textBorder.visible = false
                            voice.visible = true
                            button2.anchors.left = voice.right
                            if (button1.pressed) {
                                button1.iconSource = "qrc:/group/images/chatroom-leftbottom-key_click.png"
                            } else {
                                button1.iconSource = "qrc:/group/images/chatroom-leftbottom-key.png"
                            }
                            mainroot.closeSoftKeyboard()
                        }
                        else {
                            voice.visible = false
                            textBorder.visible = true
                            if (bIsPtt) {
                                textBorder.visible = true
                                button3.visible = true
                            }
                            button2.anchors.left = textBorder.right
                            if (button1.pressed) {
                                button1.iconSource = "qrc:/chatroom/images/chatroom_leftbottom_click.png"
                            } else {
                                button1.iconSource = "qrc:/chatroom/images/chatroom_leftbottom.png"
                            }
                        }
                    }
                }

                BorderImage {
                    id: textBorder
                    source: "qrc:/chatroom/images/chatroom_lineback.png"
                    border.left: 5; border.top: 5
                    border.right: 5; border.bottom: 5
                    width: 537
                    height: 72
                    anchors.left:parent.left
                    anchors.leftMargin: 24
                    visible: true
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    //anchors.verticalCenter: parent.verticalCenter
                    CTextArea {
                        id: textArea
                        width: parent.width
                        height: 72
                        anchors.left:parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        verticalAlignment: Qt.AlignVCenter
                        textColor: "#ffffff"
                        cursorColor: "#ffffff"
                        font.pixelSize: 30
                        wrapMode: TextEdit.Wrap
                        clip: true
                        maximumLength: 60
                        onContentHeightChanged: {
                            //height = contentHeight
                        }
                    }
                }
                CButton {
                    id: voice
                    width: 480
                    height: 72
                    z:4
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/group/images/group-voice_click.png" : "qrc:/group/images/group-voice.png"
                    visible: false
                    anchors.left:button1.right
                    anchors.leftMargin: 24
                    //                    anchors.right: button2.left
                    //                    anchors.rightMargin: 24
                    //anchors.verticalCenter: parent.verticalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    onPressed:  {
                        console.log("getData.getBtPttAllow() getData.setBtPttState")
                        if (!getData.getBtPttAllow())
                            return
                        getData.setBtPttState(groupid,2)
//                        if (bIsPtt && !timecount.running) {
//                            timecount.running = true
//                            digtaltip.visible = true
//                            digAnimation.start()
//                        }
                    }
                    onReleased: {
                        getData.setBtPttState(groupid,2)
                    }
                }

                CButton {
                    visible: false
                    id: button2
                    height: 72
                    width: 72
                    anchors.left: bIsPtt ? voice.right : textBorder.right
                    anchors.leftMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    //anchors.verticalCenter: parent.verticalCenter
                    backgroundEnabled: false
                    iconSource: "qrc:/chatroom/images/chatroom_add.png"
                    onClicked: {
                        if(gInputContext.softwareInputPanelVisible)
                            mainroot.closeSoftKeyboard()
                        if (bottomBar.height == 120){
                            bottomBar.height = 240
                        }
                        else{
                            bottomBar.height = 120
                        }

                        //pageStack.push(Qt.resolvedUrl("TakePhoto.qml"));
                        //打开文件管理插件页面，其中category的取值为0: all, 1: Picture, 2: Video; 3: Music
                        //pageStack.push(filesPickerCom, {"category": 0});
                    }
                }
                CButton {
                    id: button3
                    height: 72
                    width: 111
                    visible: true
                    //                    anchors.left: button2.right
                    //                    anchors.leftMargin: 24
                    anchors.left: textBorder.right
                    anchors.leftMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    //anchors.verticalCenter: parent.verticalCenter
                    backgroundEnabled: false
                    iconSource: pressed ? "qrc:/chatroom/images/chatroom_send_click.png" : "qrc:/chatroom/images/chatroom_send.png"
                    onClicked: {
                        if (textArea.text != ""){
                            getData.sendTextMessage(textArea.text,getData.getLocalId(),groupid)
                            textArea.text = ""
                        }
                    }
                }
            }
            Item{
                width: parent.width
                height: 121
                anchors.top: parent.top
                anchors.topMargin: 124
                z: 4
                Row {
                    spacing: 24
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    CButton {
                        id: file
                        height: 100
                        width: 150
                        backgroundEnabled: false
                        iconSource: file.pressed ? "qrc:/group/images/group-file-click.png" : "qrc:/group/images/group-file.png"
                        onClicked: {
                            pageStack.push(filesPickerCom, {"category": 0});
                        }
                    }
                    CButton {
                        id: photo
                        height: 100
                        width: 150
                        backgroundEnabled: false
                        iconSource: photo.pressed ? "qrc:/group/images/group-photo-click.png" : "qrc:/group/images/group-photo.png"
                        onClicked: {
                            goTocamera(false)
                            //pageStack.push(Qt.resolvedUrl("TakePhoto.qml"));
                        }
                    }
                    CButton {
                        id: video
                        height: 100
                        width: 150
                        backgroundEnabled: false
                        iconSource: video.pressed ? "qrc:/group/images/group-video-click.png" : "qrc:/group/images/group-video.png"
                        onClicked: {
                            console.log("goToCameraRecorder")
                            goToCameraRecorder()
                        }
                    }
                    CButton {
                        id: location
                        height: 100
                        width: 150
                        backgroundEnabled: false
                        iconSource: location.pressed ? "qrc:/group/images/group-location-click.png" : "qrc:/group/images/group-location.png"
                        onClicked: {
                        }
                    }
                }
            }
        }

        Component {
            id: videoPlayer
            Item {
                id: item
                //anchors.fill: parent
                property alias videoSource: mediaPlayer.source
                Rectangle {
                    id: rect1
                    color: "black"
                    anchors.fill: parent
                    MediaPlayer {
                        id: mediaPlayer
                        //source: "file:///data/data/com.sinux.phonebookadhoc/messagefiles/video.mp4"     //"file:///home/user/DCIM/Camera/VID_20160323_172608.mp4"
                        autoPlay: false
                    }
                    VideoOutput {
                        id: video
                        anchors.fill: parent
                        source: mediaPlayer
                    }
                    Connections {
                        target: chatRoom
                        onReplay: {
                            mediaPlayer.play()
                        }
                    }
                    Connections {
                        target: chatRoom
                        onStopplay: {
                            mediaPlayer.stop()
                        }
                    }
                }
            }
        }

        Component {
            id: fullVideoPlayer
            Item {
                width:chatRoom.width
                height: chatRoom.height
                //anchors.fill: parent
                property alias videoSource: mediaPlayer1.source
                Rectangle {
                    color: "black"
                    anchors.fill: parent
                    MediaPlayer {
                        id: mediaPlayer1
                        //source: "file:///data/data/com.sinux.phonebookadhoc/messagefiles/video.mp4"     //"file:///home/user/DCIM/Camera/VID_20160323_172608.mp4"
                        autoPlay: true
                    }
                    VideoOutput {
                        id: video
                        anchors.fill: parent
                        source: mediaPlayer1
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            mediaPlayer1.stop()
                            pageStack.pop()
                        }
                    }
                }
                Component.onDestruction: {
                    mediaPlayer1.stop()
                }
            }

        }

        Component {
            id: filesPickerCom
            SyberosFilesPicker {
                id: filesPicker
                onOk: { //选择完文件后，点击右上角的确定按钮则触发该信号
                    gScreenInfo.setStatusBar(true)
                    gScreenInfo.setStatusBarStyle("black")
                    var size = 0;
                    var selectedSize = filesPicker.getFileSize() //获取选择文件的大小（返回值为数组，为每一个选中文件的大小列表）
                    for(var i = 0; i < selectedSize.length; i++) {
                        size += parseInt(selectedSize[i]) //获取所选中文件的总大小
                    }
                    for (var j = 0; j < filesPicker.filesPath.length; j++) { //filesPicker.filesPath为选中文件的完整路径（其值为数组，为每一个选中文件的路径的列表）
                        console.log("index----" + j + ", filePath= " + filesPicker.filesPath[j]);
                        getData.sendMessage(3,filesPicker.filesPath[j],getData.getLocalId(),groupid)
                    }
                }
            }
        }

        Component {
            id: lineDelegate
            Item {
                id: lineItem
                /*
                property alias linetext: myText.text
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var userId: ""
                Image {
                    id: photo
                    width: 90
                    height: 90
//                    anchors.left: parent.left
//                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubble
                    width: 150; height: 92
                    border.left: 20; border.top: 60
                    border.right: 20; border.bottom: 20
//                    anchors.left: photo.right
//                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chartroom_bubble.png"
                    Image {
                        visible: false
                        id: myImage
                        height: 400; width: 400
                        anchors.centerIn: parent
                        onSourceChanged: {
                            bubble.height = myImage.height + 48
                        }
                    }
                    Text {
                        visible: false
                        id: myText
                        text: ""
                        color: "#ffffff"
                        font.pixelSize: 40
                        wrapMode: Text.Wrap
                        anchors.margins: 24
                        anchors.fill: parent
                        //horizontalAlignment: Qt.AlignLeft
                        onContentSizeChanged: {
                            if (contentWidth < 500){
                                bubble.width = contentWidth + 48
                            }else {
                                bubble.width = 548
                                myText.width = 500
                            }
                            bubble.height = contentHeight + 48
                        }
                    }
                }

                Component.onCompleted: {
                    console.log("flag == ",flag,text,index,direction,messId)
                    height = bubble.height
                    if (direction == "left")
                    {
                        photo.anchors.left = lineDelegate.left
                        photo.anchors.leftMargin = 24
                        bubble.anchors.left = photo.right
                        bubble.anchors.leftMargin = 16
                    }
                    else {
                        photo.anchors.right = lineDelegate.right
                        photo.anchors.rightMargin24
                        bubble.anchors.right = photo.left
                        bubble.anchors.rightMargin = 16
                    }
                    //flag 0:time  1:text 2:picture 3:files 4:audio 5:video
                    switch (flag)
                    {
                    case 0:
                        break;
                    case 1:
                        myText.visible = true
                        myText.text = text
                        break;
                    case 2:
                        myImage.visible = true
                        //myImage.source = text
                        break;
                    case 3:
                        break;
                    case 4:
                        myImage.visible = true
                        break;
                    case 5:
                        myImage.visible = true
                        break;
                    default:
                        break;
                    }
                }
                */

                Loader {
                    id: lineLoader
                    onLoaded: {
                    }
                }

                Component.onCompleted: {
                    //flag 0:time  1:text 2:picture 3:files 4:audio 5:video
                    if (flag == 1)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = textComponentLeft
                        else
                            lineLoader.sourceComponent = textComponentRight
                        lineLoader.item.text = text
                        lineLoader.item.userId = userId
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/main/images/init_photo.png"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 0){
                        lineLoader.sourceComponent = timeComponent
                        lineItem.height = lineLoader.item.myHeight
                    }
                    else if (flag == 2)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = imageComponentLeft
                        else
                            lineLoader.sourceComponent = imageComponentRight
                        if (getData.isFileExists(text))
                            lineLoader.item.source = text
                        else
                            lineLoader.item.source = "qrc:/group/images/picture-broken.png"

                        lineLoader.item.myindex = index
                        lineLoader.item.userId = userId
                        if (photoPath == "null" || !getData.isFileExists(photoPath))
                            lineLoader.item.photoIcon = "qrc:/main/images/init_photo.png"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 3)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = fileComponentLeft
                        else
                            lineLoader.sourceComponent = fileComponentRight
                        lineLoader.item.fileName = fileName
                        lineLoader.item.messageId = messId
                        lineLoader.item.state = messageState
                        lineLoader.item.stateText = "  "
                        lineLoader.item.userId = userId
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/main/images/init_photo.png"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 4)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = audioComponentLeft
                        else
                            lineLoader.sourceComponent = audioComponentRight
                        lineLoader.item.messageId = messId
                        lineLoader.item.state = messageState
                        lineLoader.item.userId = userId
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/main/images/init_photo.png"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 5)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = videoComponentLeft
                        else
                            lineLoader.sourceComponent = videoComponentRight
                        //if (messageState == 8 || messageState == 1)
                        lineLoader.item.sourceTemp = text
                        lineLoader.item.state = messageState
                        lineLoader.item.messageId = messId
                        lineLoader.item.userId = userId
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/main/images/init_photo.png"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    lineItem.height = lineLoader.item.myHeight
                }

            }
        }

        Timer {
            id: timecount
            interval: 1000
            repeat: true
            onTriggered: {

            }
        }
        Text {
            id: digtaltip
            anchors.bottom: bottomBar.top
            anchors.bottomMargin: 48
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            height: 48
            font.pixelSize: 40
            color: "red"
            opacity: 1
            visible: false
            text: qsTr("抢麦失败")
        }
        NumberAnimation {
            id: digAnimation
            target: digtaltip
            property: "opacity"
            to: 0
            duration: 2000
            onStopped: {
                timecount.running = false
                digtaltip.visible = false
                digtaltip.opacity = 1
            }
        }

        Component {
            id: timeComponent
            Item {
                id: item
                property alias text: timeText.text
                property alias myHeight: timeText.height
                width: listView.width
                Text {
                    height: 30
                    anchors.centerIn: parent
                    id: timeText
                    font.pixelSize: 30
                    color: "#b6b6b6"
                }
            }
        }

        Component {
            id: locationComponentLeft
            Item {
                id: item
                property var messageId: ""
                property var state: -1
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property alias location: locationname.text
                property var userId: ""
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubble
                    width: 448; height: 198
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photo.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chartroom_bubble.png"
                    Image {
                        id: groupfile
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        source: "qrc:/group/images/group-location-ready.png"
                    }
                    Text {
                        id: locationname
                        anchors.left: groupfile.right
                        anchors.leftMargin: 24
                        font.pixelSize: 40
                        color: "#ffffff"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                        }
                    }
                }
                Component.onCompleted: {
                    height = 198
                }
            }
        }

        Component {
            id: locationComponentRight
            Item {
                id: item
                width: listView.width
                property var messageId: ""
                property var state: -1
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property alias location: locationname.text
                property var userId: ""
                Image {
                    id: photo
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 90
                    height: 90
                    source: "qrc:/main/images/init_photo.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubble
                    width: 448; height: 198
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photo.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/group/images/group-location-ready.png"
                    Image {
                        id: groupfile
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        source: "qrc:/group/images/group-file-name.png"
                    }
                    Text {
                        id: locationname
                        anchors.left: groupfile.right
                        anchors.leftMargin: 24
                        font.pixelSize: 40
                        color: "#ffffff"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                        }
                    }
                }
                Component.onCompleted: {
                    height = 198
                }
            }
        }

        Component {
            id: fileComponentLeft
            Item {
                id: item
                property alias fileName: groupfilename.text
                property var messageId: ""
                property var state: -1
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property alias stateText: statetext.text
                property var userId: ""
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubble
                    width: 448; height: 198
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photo.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chartroom_bubble.png"
                    Image {
                        id: groupfile
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        source: "qrc:/group/images/group-file-name.png"
                    }
                    Image {
                        id: groupfileup
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        visible: item.state == 6 ? true : false
                        source: "qrc:/group/images/group-file-ready.png"
                    }
                    Text {
                        id: groupfilename
                        anchors.left: groupfile.right
                        anchors.leftMargin: 24
                        font.pixelSize: 40
                        color: "#ffffff"
                        width: 150
                        elide: Text.ElideRight
                        onTextChanged: {
                            var list = groupfilename.text.split(".")
                            if (list.length >= 1){
                                if (list[1] == "txt")
                                    groupfile.source = "qrc:/group/images/message-txt.png"
                                else if (list[1] == "mp3")
                                    groupfile.source = "qrc:/group/images/message-audio.png"
                                else if (list[1] == "mp4")
                                    groupfile.source = "qrc:/group/images/message-video.png"
                                else if (list[1] == "png" || list[1] == "jpg")
                                    groupfile.source = "qrc:/group/images/message-image.png"
                            }
                        }
                    }
                    Text {
                        id: statetext
                        anchors.right: parent.right
                        anchors.rightMargin: 24
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 24
                        font.pixelSize: 20
                        color: "#ffffff"
                        onTextChanged: {
                            if (item.state == 3)
                                statetext.text = "已发送"
                            else if (item.state == 1)
                                statetext.text = "发送中"
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (item.state == 6)
                                getData.getMessageDownload(item.messageId, groupid);
                            else if (item.state == 8)
                            {
                                console.log("打开文件")
                            }
                        }
                    }
                }
                Component.onCompleted: {
                    height = 198
                }
            }
        }

        Component {
            id: fileComponentRight
            Item {
                id: item
                width: listView.width
                property alias fileName: groupfilename.text
                property var messageId: ""
                property var state: -1
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property alias stateText: statetext.text
                property var userId: ""
                Image {
                    id: photo
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 90
                    height: 90
                    source: "qrc:/main/images/init_photo.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubble
                    width: 448; height: 198
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photo.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chatroom_bubbleright.png"
                    Image {
                        id: groupfile
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        source: "qrc:/group/images/group-file-name.png"
                    }
                    Image {
                        id: groupfileup
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        visible: item.state == 6 ? true : false
                        source: "qrc:/group/images/group-file-ready.png"
                    }
                    Text {
                        id: groupfilename
                        anchors.left: groupfile.right
                        anchors.leftMargin: 24
                        font.pixelSize: 40
                        elide: Text.ElideRight
                        anchors.top: parent.top
                        anchors.topMargin: 24
                        width: 150
                        color: "#ffffff"
                        onTextChanged: {
                            var list = groupfilename.text.split(".")
                            if (list.length >= 1){
                                if (list[1] == "txt")
                                    groupfile.source = "qrc:/group/images/message-txt.png"
                                else if (list[1] == "mp3")
                                    groupfile.source = "qrc:/group/images/message-audio.png"
                                else if (list[1] == "mp4")
                                    groupfile.source = "qrc:/group/images/message-video.png"
                                else if (list[1] == "png" || list[1] == "jpg")
                                    groupfile.source = "qrc:/group/images/message-image.png"
                            }
                        }
                    }
                    Text {
                        id: statetext
                        anchors.right: parent.right
                        anchors.rightMargin: 24
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 24
                        font.pixelSize: 20
                        color: "#ffffff"
                        onTextChanged: {
                            if (item.state == 3)
                                statetext.text = "已发送"
                            else if (item.state == 1)
                                statetext.text = "发送中"
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (item.state == 6)
                                getData.getMessageDownload(item.messageId, groupid);
                            else if (item.state == 8)
                            {
                                console.log("打开文件")
                            }
                        }
                    }
                }
                Component.onCompleted: {
                    height = 198
                }
            }
        }


        Component {
            id: videoComponentLeft
            Item {
                id: item
                property var state: -1
                property alias sourceTemp: temps.text
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var messageId: ""
                property var index: 0
                property var userId: ""
                Text {
                    id: temps
                    text: qsTr("text")
                    visible: false
                    onTextChanged: {
                        console.log("onTextChanged",temps.text)
                        //videoLoader.item.videoSource = temps.text
                    }
                }
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubble
                    width: 358; height: 358
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photo.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chartroom_bubble.png"
                    property var userId: ""
                    //                    Loader {
                    //                        id: videoLoader
                    //                        sourceComponent: videoPlayer
                    //                        height: 310; width: 310
                    //                        anchors.centerIn: parent
                    //                    }
                    Image {
                        id: groupfileup
                        z: 5
                        width: 150
                        height: 150
                        anchors.centerIn: parent
                        //visible: item.state == 6 ? true : false
                        source: "qrc:/group/images/group-video-ready.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            qApp.openUrl("video://play?fileuri=" + temps.text)
                        }

                        //                        onClicked: {
                        //                            myindex = index
                        //                            if (item.state == 8){
                        //                                replay()
                        //                            }
                        //                            else
                        //                                getData.getMessageDownload(messageId,groupid)
                        //                        }
                        //                        onDoubleClicked: {
                        //                            stopplay()
                        //                            qApp.openUrl("video://play?fileuri=" + temps.text)
                        //                        }
                    }
                }
                Component.onCompleted: {
                    myHeight = 358
                }
            }
        }

        Component {
            id: videoComponentRight
            Item {
                id: item
                width: listView.width
                property alias sourceTemp: temps.text
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var state: -1
                property var messageId: ""
                property var userId: ""
                Text {
                    id: temps
                    text: qsTr("text")
                    visible: false
                    onTextChanged: {
                        //videoLoader.item.videoSource = temps.text
                    }
                }
                Image {
                    id: photo
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 90
                    height: 90
                    source: "qrc:/main/images/init_photo.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubble
                    width: 358; height: 358
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photo.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chatroom_bubbleright.png"
                    //                    Loader {
                    //                        id: videoLoader
                    //                        sourceComponent: videoPlayer
                    //                        height: 310; width: 310
                    //                        anchors.centerIn: parent
                    //                    }
                    Image {
                        id: groupfileup
                        width: 150
                        height: 150
                        anchors.centerIn: parent
                        //visible: item.state == 6 ? true : false
                        source: "qrc:/group/images/group-video-ready.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log(temps.text)
                            qApp.openUrl("video://play?fileuri=" + temps.text)
                        }

                        //                        onClicked: {
                        //                            if (item.state == 8){
                        //                                replay()
                        //                            }
                        //                            else
                        //                                getData.getMessageDownload(messageId,groupid)
                        //                        }
                        //                        onDoubleClicked: {
                        //                            stopplay()
                        //                            qApp.openUrl("video://play?fileurl=" + temps.text)
                        //                        }
                    }
                }

                Component.onCompleted: {
                    myHeight = 358
                }

            }
        }

        Component {
            id: imageComponentLeft
            Item {
                id: item
                property alias source: myImage.source
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var userId: ""
                property var myindex: 0
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubble
                    //width: 150; height: 92
                    width: 448; height: 448
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photo.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chartroom_bubble.png"
                    Image {
                        id: myImage
                        height: 400; width: 400
                        anchors.centerIn: parent
                        rotation: 90
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (myImage.source == "qrc:/group/images/picture-broken.png")
                                return
                            var str = myImage.source
                            str = str.toString().substring(7)
                            pageStack.push("qrc:/qml/PhotoViewer.qml",{"myindex":myImage.source,myparent:chatRoom},true);
                            //pageStack.push(pictureView,{"myindex":myImage.source});
                            //var item = pageStack.push(fullImage,{"source":myImage.source})
                        }
                    }
                }
                Component.onCompleted: {
                    height = myImage.height
                }
            }
        }

        Component {
            id: imageComponentRight
            Item {
                id: item
                width: listView.width
                property alias source: myImage.source
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var userId: ""
                property var myindex: 0
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    source: "qrc:/main/images/init_photo.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubble
                    width: 448; height: 448
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photo.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chatroom_bubbleright.png"
                    Image {
                        id: myImage
                        height: 400;
                        width: 400
                        anchors.fill: parent
                        rotation: 90
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (myImage.source == "qrc:/group/images/picture-broken.png")
                                return
                            var str = myImage.source
                            str = str.toString().substring(7)
                            pageStack.push("qrc:/qml/PhotoViewer.qml",{"myindex":myImage.source,myparent:chatRoom},true);
                            //var item = pageStack.push(fullImage,{"source":myImage.source})
                        }
                    }
                }

                Component.onCompleted: {
                    height = myImage.height
                }
            }
        }

        Component {
            id: fullImage
            CPage {
                id: fullc
                property alias source: full.source
                Image {
                    id: full
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        full.scale = 1.5
                    }
                }
            }
        }

        Component {
            id: textComponentLeft
            Item {
                id: item
                property alias text: myText.text
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var userId: ""
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: photoIcon//"qrc:/main/images/init_photo.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubble
                    width: 150; height: 92
                    border.left: 20; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.left: photo.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chartroom_bubble.png"

                    Text {
                        id: myText
                        text: qsTr("")
                        color: "#ffffff"
                        font.pixelSize: 40
                        wrapMode: Text.Wrap
                        anchors.margins: 24
                        anchors.fill: parent
                        //horizontalAlignment: Qt.AlignLeft
                        onContentSizeChanged: {
                            if (contentWidth < 410){
                                bubble.width = contentWidth + 48
                            }else {
                                bubble.width = 458
                                myText.width = 410
                            }

                            bubble.height = contentHeight + 48
                        }
                    }
                }

                Component.onCompleted: {
                    height = myText.height
                }
            }
        }

        Component {
            id: textComponentRight
            Item {
                id: item
                width: listView.width
                property alias text: myText.text
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var userId: ""
                Image {
                    id: photo
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 90
                    height: 90
                    source: photoIcon//"qrc:/main/images/init_photo.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"local"})
                        }
                    }
                }

                BorderImage {
                    id: bubble
                    width: 150; height: 92
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photo.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chatroom_bubbleright.png"

                    Text {
                        id: myText
                        text: qsTr("")
                        color: "#ffffff"
                        font.pixelSize: 40
                        wrapMode: Text.Wrap
                        anchors.margins: 24
                        anchors.fill: parent
                        //horizontalAlignment: Qt.AlignRight
                        onContentSizeChanged: {
                            if (contentWidth < 410){
                                bubble.width = contentWidth + 48
                            }else {
                                bubble.width = 458
                                myText.width = 410
                            }

                            bubble.height = contentHeight + 48
                        }
                    }
                }

                Component.onCompleted: {
                    height = myText.height
                }
                MouseArea {
                    anchors.fill: parent
                    onPressAndHold: {
                        console.log("onPressAndHold")
                    }
                }
            }
        }

        Component {
            id: audioComponentLeft
            Item {
                id: item
                property alias source: myImage.source
                property alias audioSource: audio.source
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var userId: ""
                property var messageId: ""
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubble
                    //width: 150; height: 92
                    width: 256; height: 93
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photo.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chartroom_bubble.png"
                    Image {
                        id: myImage
                        height: 40
                        width: 40
                        anchors.left: parent.left
                        anchors.leftMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-stop-left.png"
                    }
                    AnimatedImage {
                        id: playing
                        height: 40
                        width: 40
                        anchors.left: parent.left
                        anchors.leftMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-play-left.gif"
                    }
                    Item {
                        id: setRec
                        visible: false
                        height: 96
                        width: 186
                        anchors.bottom: bubble.top
                        anchors.right: bubble.right
                        anchors.rightMargin: 35
                        Rectangle {
                            height: 72
                            width: 186
                            anchors.bottom: setgif.top
                            color: "black"
                            CButton {
                                height: 72
                                width: 93
                                backgroundEnabled: false
                                text: "已读"
                                textColor: "white"
                                anchors.left: parent.left
                                onClicked: {
                                    setRec.visible = false
                                }
                            }
                            CButton {
                                height: 72
                                width: 93
                                backgroundEnabled: false
                                text: "未读"
                                textColor: "white"
                                anchors.right: parent.right
                                onClicked: {
                                    setRec.visible = false
                                }
                            }
                        }
                        AnimatedImage {
                            id: setgif
                            height: 14
                            width: 30
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "qrc:/chatroom/images/audio-set.gif"
                        }
                    }

                    Audio {
                        id: audio
                        autoPlay: false
                        onStatusChanged: {
                            if (audio.status == 3 || audio.status == 7){
                                playing.visible = false;
                                myImage.visible = true
                            }
                            else if (audio.status == 6){
                                playing.visible = true;
                                myImage.visible = false
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (audio.status == 3 || audio.status == 7)
                                audio.play()
                            else if (audio.status == 6)
                                audio.stop()
                        }
                        onPressAndHold: {
                            setRec.visible = true
                        }
                    }
                }
                Component.onCompleted: {
                    height = 256
                }
            }
        }

        Component {
            id: audioComponentRight
            Item {
                id: item
                width: listView.width
                property alias source: myImage.source
                property alias audioSource: audio.source
                property alias myHeight: bubble.height
                property alias photoIcon: photo.source
                property alias bubbleSource: bubble.source
                property var userId: ""
                property var messageId: ""
                Image {
                    id: photo
                    width: 90
                    height: 90
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    source: "qrc:/main/images/init_photo.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":item.userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubble
                    width: 256; height: 93
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photo.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/chatroom/images/chatroom_bubbleright.png"
                    Image {
                        id: myImage
                        height: 40
                        width: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-stop-right.png"
                    }
                    AnimatedImage {
                        height: 40
                        width: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-play-right.gif"
                    }
                    Audio {
                        id: audio
                        autoPlay: false
                        onStatusChanged: {
                            if (audio.status == 3 || audio.status == 7){
                                playing.visible = false;
                                myImage.visible = true
                            }
                            else if (audio.status == 6){
                                playing.visible = true;
                                myImage.visible = false
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (audio.status == 3 || audio.status == 7)
                                audio.play()
                            else if (audio.status == 6)
                                audio.stop()
                        }
                        onPressAndHold: {
                            console.log("onPressAndHold")
                        }
                    }
                }

                Component.onCompleted: {
                    height = 256
                }
            }
        }

        Component {
            id: leftTitleBar
            CButton {
                anchors.fill: parent
                iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                backgroundEnabled: false
                spacing: 0
                onClicked: {
                    pageStack.pop()
                }
            }
        }

        Component {
            id: rightTitleBar
            CButton {
                visible: groupid == "" ? false : true
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/chatroom/images/chatroom_righttitle_click.png" :
                                      "qrc:/chatroom/images/chatroom_righttitle.png"
                onClicked: {
                    pageStack.push("qrc:/qml/GroupInfo.qml",{"groupid":groupid,
                                       "groupname":groupname,"groupnum":groupnum,
                                       "groupnet":netinfo,"qmlcontains":prepage,
                                       "precontains":chatRoom})
                }
            }
        }
        ListModel {
            id: pictureModel
            Component.onCompleted: {
                pictureModel.append({"sources":"file:///home/user/IMG_20160512_150902.jpg"})
            }
        }

        Component{
            id: pictureView
            Item {
                property var myindex: ""
                property bool isdouble: false
                width: 720
                height: 1280
                id: picture
                ListView{
                    id: listPage
                    anchors.fill: parent
                    model: pictureModel
                    delegate: Flickable {
                        id: flick
                        width: parent.width
                        height: parent.height
                        contentHeight: name.height
                        contentWidth: name.width
                        Image {
                            anchors.fill: parent
                            width: parent.width
                            height: parent.height
                            id: name
                            source: sources
                            scale: 1
                            Connections{
                                target: gScreenInfo
                                onCurrentOrientationChanged: {
                                    if (gScreenInfo.currentOrientation == 2 ||
                                            gScreenInfo.currentOrientation == 8) {
                                        name.rotation = -90
                                        console.log("onCurrentOrientationChanged")
                                        name.rotation = 0
                                        picture.height = 1280
                                        picture.width = 1280
                                    } else {
                                        //                                        name.rotation = 0
                                        //                                        picture.height = 1280
                                        //                                        picture.width = 720
                                    }
                                }
                            }
                            PinchArea {
                                id: pinch
                                anchors.fill: parent
                                pinch.maximumScale: 3;
                                pinch.minimumScale: 1;
                                pinch.target: parent;
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        pageStack.pop()
                                        //console.log("click",name.souce)
                                    }
                                }
                            }
                            onScaleChanged: {
                                name.height = 1280 * scale
                                name.width = 720 * scale
                            }
                        }
                    }
                    orientation: ListView.Horizontal
                    snapMode: ListView.SnapOneItem
                    highlightRangeMode: ListView.StrictlyEnforceRange
                    cacheBuffer: 2
                    boundsBehavior: Flickable.StopAtBounds
                    Component.onCompleted: {
                        for (var i = 0; i< pictureModel.count;i++)
                        {
                            if (myindex == pictureModel.get(i).sources){
                                positionViewAtIndex(i,listView.Center)
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}



