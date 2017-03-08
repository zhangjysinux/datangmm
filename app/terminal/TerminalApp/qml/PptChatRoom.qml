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
    property int timeL: 0
    property bool isFresh: false
    property bool isbottom: false
    property bool bIsPtt: false
    property var myindex: 0
    property int pptGroupId: 0
    signal replay()
    signal stopplay()
    orientationPolicy: CPageOrientation.Automatic

    onStatusChanged: {
        if (status == CPageStatus.WillShow)
        {
            titleBar.titleItemComponent = null
            titleBar.titleItemComponent = titile

            getData.onGetMessageList(groupid)
            listView.positionViewAtEnd()
        }
    }

    function findNet(myNetText)
    {
        var net;
        if (myNetText == "自组网")
            net = 1
        else if (myNetText == "LTE")
            net = 16
        else if (myNetText == "天通网")
            net = 256
        else if (myNetText == "WIFI")
            net = 1048576
        return net;
    }

    Component.onCompleted:	{
        getData.joinGroup(findNet(netinfo),groupid)
        getData.setPttState(findNet(netinfo),groupid,1)
        syncCount = getData.getSyncCount(groupid)
        if (syncCount)
            sync.visible = true
        else
            sync.visible = false
        console.log("getSyncCount",syncCount)
        //getData.getImageMessageList()
        chatRoom.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            chatRoom.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            //voice.iconSource = voice.pressed ? "qrc:/group/images/group-voice-h_click.png" : "qrc:/group/images/group-voice-h.png"
            voice.width = 1040
            textBorder.width = 905
        } else {
            chatRoom.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            //voice.iconSource = voice.pressed ? "qrc:/group/images/group-voice_click.png" : "qrc:/group/images/group-voice.png"
            voice.width = 480
            textBorder.width = 345
        }
    }

    Component.onDestruction: {
        getData.leaveGroup(findNet(netinfo),groupid)
    }

    Connections {
        target: gInputContext
        onSoftwareInputPanelVisibleChanged: {
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
                textBorder.width = 905
            } else {
                chatRoom.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                voice.iconSource = voice.pressed ? "qrc:/group/images/group-voice_click.png" : "qrc:/group/images/group-voice.png"
                voice.width = 480
                textBorder.width = 345
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
        target: m_messageModel
        onSignalFindImage: {

            //            var str = "file:///data/data/com.sinux.phonebook/"+sources;
            //            console.log("onSignalFindImage",str)
            pictureModel.append({"sources":sources})
        }
    }

    function goTocamera(enableCut)
    {
        var tackPhoto  = pageStack.push(Qt.resolvedUrl("TakePhoto.qml"), {"enableCut": enableCut});
        tackPhoto.imageConfirmed.connect(function(filePath) { //处理信号
            console.log(filePath)
            var imageUrl = "file://" + filePath
            console.log("TakePhoto",imageUrl)
            getData.sendMessage(2,imageUrl,getData.getLocalId(),groupid)
            pageStack.pop(chatRoom);
            //chatRoom.statusBarHoldItemColor("black")
            //gScreenInfo.setStatusBar(true)
            //gScreenInfo.setStatusBarStyle("black")
        })
    }

    function goToCameraRecorder()
    {
        var tackVideo  = pageStack.push(Qt.resolvedUrl("CameraRecorder.qml"));
        tackVideo.videoSaved.connect(function(filePath) { //处理信号
            console.log(filePath,"video")
            getData.sendMessage(5,filePath,getData.getLocalId(),groupid)
            pageStack.pop(chatRoom);
        })
    }

    Connections {
        target: getData
        onSignalMessageChanged: {
            //flag 0:time  1:text 2:picture 3:files 4:audio 5:video
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
                photoPath == "qrc:/set/images/set_photo.jpg"

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
            listView.positionViewAtEnd()
        }
    }
    contentAreaItem: Item {
        CButton {
            id: sync
            width: 180
            height: 72
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 300
            z: 10
            backgroundEnabled: true
            backgroundComponent: Rectangle {
                anchors.fill: parent
                color: "#2e3b4c"
            }

            //iconSource:
            onClicked: {
                getData.clearSyncCount(groupid)
                listView.positionViewAtIndex(getData.getMessageCount() - syncCount,listView.Center)
            }
        }

        Component {
            id: titile
            Item {
                height: 96
                width: 250
                property alias myName: tempName.text

                Text {
                    id: tempName
                    anchors.centerIn: parent
                    font.pixelSize: 30
                    clip: true
                    width: 250
                    horizontalAlignment: Qt.AlignCenter
                    color: "#29afe2"
                    elide: Text.ElideRight
                    text: (groupnum == "") ? groupname : groupname + "(" + groupnum + ")"
                    visible: true//getData.getCachePageCount() == 1 ? true : false
                }
//                GroupTitleList {
//                    id: title2
//                    height: 96
//                    width: 250
//                    visible: true//getData.getCachePageCount() > 1 ? true : false
//                }
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
            titleItemEnabled: true
            titleItemComponent: titile
            titleAreaCentered: true
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
            backgroundEnabled: true
            backgroundComponent: Rectangle {
                anchors.fill: parent
                color: "#293443"
            }
            Component.onCompleted: {
            }
        }

//        Item {
//            id: mid
//            width: parent.width
//            anchors.top: parent.top
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
        ListModel {
            id: listModel
        }

        ListView {
            id: listView
            spacing: 24
            width: parent.width
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
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


        Rectangle {
            id: bottomBar
            width: parent.width
            height: 120
            color: "#2e3b4c"
            anchors.bottom: parent.bottom
//            Image {
//                anchors.fill: parent
//                source: "qrc:/main/images/top_line.png"
//            }
            onYChanged: {
                if (gScreenInfo.currentOrientation == 2 ||
                        gScreenInfo.currentOrientation == 8) {
                    if(gInputContext.softwareInputPanelVisible && y == 150)
                        bottomBar.height =120
                }else {
                    if(gInputContext.softwareInputPanelVisible && y == 426)
                        bottomBar.height =120
                }
            }

            Rectangle {
                height: 120
                color: "#2e3b4c"
                width: parent.width
                radius: 10
                anchors.top: parent.top
                CButton {
                    id: button1
                    height: 72
                    width: 72
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    //anchors.verticalCenter: parent.verticalCenter
                    backgroundEnabled: false
                    iconSource:  "qrc:/main/images/icon_audio.png" //bIsptt ? "qrc:/main/images/icon_jianpan1.png" : "qrc:/main/images/icon_audio.png"
//                    iconSource: bIsPtt ? (pressed ? "qrc:/group/images/chatroom-leftbottom-key_click.png": "qrc:/group/images/chatroom-leftbottom-key.png")
//                                       : (pressed ? "qrc:/chatroom/images/chatroom_leftbottom.png_click" : "qrc:/chatroom/images/chatroom_leftbottom.png")

                    onClicked: {
                        if (voice.visible == false){
                            if (bIsPtt) {
                                textBorder.visible = false
                                button3.visible = false
                            }
                            textBorder.visible = false
                            voice.visible = true
                            button2.anchors.left = voice.right
                            button1.iconSource = "qrc:/main/images/icon_jianpan1.png"
//                            if (button1.pressed) {
//                                button1.iconSource = "qrc:/group/images/chatroom-leftbottom-key_click.png"
//                            } else {
//                                button1.iconSource = "qrc:/group/images/chatroom-leftbottom-key.png"
//                            }
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
                            button1.iconSource = "qrc:/main/images/icon_audio.png"
//                            if (button1.pressed) {
//                                button1.iconSource = "qrc:/chatroom/images/chatroom_leftbottom_click.png"
//                            } else {
//                                button1.iconSource = "qrc:/chatroom/images/chatroom_leftbottom.png"
//                            }
                        }
                    }
                }

                Rectangle {
                    id: textBorder
                    color: "#293443"
                    radius: 10
                    //source: "qrc:/chatroom/images/chatroom_lineback.png"
//                    border.left: 5; border.top: 5
//                    border.right: 5; border.bottom: 5
                    width: 345
                    height: 72
                    anchors.left:button1.right
                    anchors.leftMargin: 24
                    visible: !bIsPtt
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
                    backgroundEnabled: true
                    //iconSource: pressed ? "qrc:/group/images/group-voice_click.png" : "qrc:/group/images/group-voice.png"
                    visible: bIsPtt
                    anchors.left:button1.right
                    anchors.leftMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    text: "按住说话"
                    textColor: "#29afe2"
                    backgroundComponent: Component {
                        Rectangle {
                            anchors.fill: parent
                            border.color: "#29afe2"
                            radius: 10
                            border.width: 2
                            color: Qt.rgba(0,0,0,0)
                        }
                    }

                    onPressed:  {
                        if (bIsPtt && !timecount.running) {
                            digtaltip.visible = true
                            digAnimation.start()
                        }
                        timecount.running = true
                        getData.setPttState(findNet(netinfo),groupid, 2)
                    }
                    onReleased: {
                        timecount.running = false
                        getData.setPttState(findNet(netinfo),groupid, 1)
                    }
                }

                CButton {
                    id: button2
                    height: 72
                    width: 72
                    anchors.left: bIsPtt ? voice.right : textBorder.right
                    anchors.leftMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    //anchors.verticalCenter: parent.verticalCenter
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/icon-add.png"
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
                    visible: !bIsPtt
                    //                    anchors.left: button2.right
                    //                    anchors.leftMargin: 24
                    anchors.left: button2.right
                    anchors.leftMargin: 24
                    anchors.top: parent.top
                    anchors.topMargin: 27
                    //anchors.verticalCenter: parent.verticalCenter
                    backgroundEnabled: false
                    text: "发送"
                    textColor: "#29afe2"
                    //iconSource: pressed ? "qrc:/chatroom/images/chatroom_send_click.png" : "qrc:/chatroom/images/chatroom_send.png"
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
                        iconSource: "qrc:/main/images/group-file.png"
                        onClicked: {
                            pageStack.push(filesPickerCom, {"category": 0});
                        }
                    }
                    CButton {
                        id: photo
                        height: 100
                        width: 150
                        backgroundEnabled: false
                        iconSource: "qrc:/main/images/group-photo.png"
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
                        iconSource: "qrc:/main/images/group-video.png"
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
                        iconSource: "qrc:/main/images/group-location.png"
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
                        //source: "file:///data/data/com.sinux.phonebook/messagefiles/video.mp4"     //"file:///home/user/DCIM/Camera/VID_20160323_172608.mp4"
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
                        //source: "file:///data/data/com.sinux.phonebook/messagefiles/video.mp4"     //"file:///home/user/DCIM/Camera/VID_20160323_172608.mp4"
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
                Loader {
                    id: timeLoader
                    height: 30
                    visible: false
                }

                Loader {
                    id: lineLoader
                    //                    anchors.bottom: lineItem.bottom
                    onLoaded: {
                    }
                }
                CButton {
                    id: resendButton
                    property var msgId: value
                    visible: false
                    backgroundEnabled: false
                    anchors.right: lineLoader.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: lineLoader.verticalCenter
                    onClicked: {
                        resendButton.visible = false
                    }
                }

                Component.onCompleted: {
                    //flag 6:system  1:text 2:picture 3:files 4:audio 5:video
                    if (flag == 1)
                    {
                        if (direction == "left"){
                            lineLoader.sourceComponent = textComponentLeft
                        }
                        else {
                            lineLoader.sourceComponent = textComponentRight
                            if (messageState == 2){
                                resendButton.visible = true
                                resendButton.msgId = messId
                            }
                        }
                        lineLoader.item.text = text
                        lineLoader.item.userId = userId
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/main/images/set_photo.jpg"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 2)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = imageComponentLeft
                        else {
                            lineLoader.sourceComponent = imageComponentRight
                            if (messageState == 2){
                                resendButton.visible = true
                                resendButton.msgId = messId
                            }
                        }

                        var small
                        small = getData.getSmallUrl(text)
                        if (getData.isFileExists(small))
                            lineLoader.item.source = small
                        else
                            lineLoader.item.source = "qrc:/group/images/picture-broken.png"

                        lineLoader.item.bigsource = text
                        lineLoader.item.messageId = messId
                        lineLoader.item.myindex = index
                        lineLoader.item.userId = userId
                        if (photoPath == "null" || !getData.isFileExists(photoPath))
                            lineLoader.item.photoIcon = "qrc:/set/images/set_photo.jpg"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 3)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = fileComponentLeft
                        else {
                            lineLoader.sourceComponent = fileComponentRight
                            if (messageState == 2){
                                resendButton.visible = true
                                resendButton.msgId = messId
                            }
                        }
                        lineLoader.item.fileName = fileName
                        lineLoader.item.messageId = messId
                        lineLoader.item.state = messageState
                        lineLoader.item.stateText = "  "
                        lineLoader.item.userId = userId
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/set/images/set_photo.jpg"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 4)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = audioComponentLeft
                        else {
                            lineLoader.sourceComponent = audioComponentRight
                            if (messageState == 2){
                                resendButton.visible = true
                                resendButton.msgId = messId
                            }
                        }
                        lineLoader.item.messageId = messId
                        lineLoader.item.state = messageState
                        lineLoader.item.userId = userId
                        lineLoader.item.audioSource = text
                        lineLoader.item.isRead = isRead
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/set/images/set_photo.jpg"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 5)
                    {
                        if (direction == "left")
                            lineLoader.sourceComponent = videoComponentLeft
                        else {
                            lineLoader.sourceComponent = videoComponentRight
                            if (messageState == 2){
                                resendButton.visible = true
                                resendButton.msgId = messId
                            }
                        }
                        //if (messageState == 8 || messageState == 1)
                        lineLoader.item.sourceTemp = text
                        lineLoader.item.state = messageState
                        lineLoader.item.messageId = messId
                        lineLoader.item.userId = userId
                        if (photoPath == "null")
                            lineLoader.item.photoIcon = "qrc:/set/images/set_photo.jpg"
                        else
                            lineLoader.item.photoIcon = photoPath
                    }
                    else if (flag == 6){
                        lineLoader.sourceComponent = timeComponent
                        lineItem.height = lineLoader.item.myHeight
                        lineLoader.item.text = text
                    }
                    var time = getData.getMessageDate(m_messageModel.getMessageValue(index - 1 , "time"), dateTime)
                    //lineItem.height = lineLoader.item.myHeight

                    if (time == ""){
                        lineItem.height = lineLoader.item.myHeight
                    }
                    else {
                        lineItem.height = lineLoader.item.myHeight + 30
                        timeLoader.visible = true
                        timeLoader.sourceComponent = timeComponent
                        timeLoader.height = timeLoader.item.myHeight
                        timeLoader.item.text = time
                        lineLoader.anchors.top = timeLoader.bottom
                        lineLoader.anchors.topMargin = 10
                    }

                }

            }
        }

        Timer {
            id: timecount
            interval: 30000
            repeat: true
            onTriggered: {
                getData.setPttState(findNet(netinfo),groupid, 1)
                getData.setPttState(findNet(netinfo),groupid, 2)
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
                Component.onCompleted: {
                    height = 30
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
                    anchors.bottom: bubble.bottom
                    source: "qrc:/main/images/init_photo.jpg"

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
                    source: "qrc:/main/images/ppt_left.png"
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
                    source: "qrc:/set/images/set_photo.jpg"
                    anchors.bottom: bubble.bottom
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
                    source: "qrc:/main/images/ppt_right.png"
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
                id: itemfl
                property alias fileName: groupfilenamefl.text
                property var messageId: ""
                property var state: -1
                property alias myHeight: bubblefl.height
                property alias photoIcon: photofl.source
                property alias bubbleSource: bubblefl.source
                property alias stateText: statetextfl.text
                property var userId: ""
                Image {
                    id: photofl
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.jpg"
                    anchors.bottom: bubblefl.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":itemfl.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubblefl
                    width: 448; height: 198
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photofl.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_left.png"
                    Image {
                        id: groupfilefl
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        source: "qrc:/group/images/group-file-name.png"
                    }
                    Image {
                        id: groupfileupfl
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        visible: itemfl.state == 6 ? true : false
                        source: "qrc:/group/images/group-file-ready.png"
                    }
                    Text {
                        id: groupfilenamefl
                        anchors.left: groupfilefl.right
                        anchors.leftMargin: 24
                        font.pixelSize: 40
                        color: "#ffffff"
                        width: 150
                        elide: Text.ElideRight
                        onTextChanged: {
                            var list = groupfilenamefl.text.split(".")
                            if (list.length >= 1){
                                if (list[1] == "txt")
                                    groupfilefl.source = "qrc:/group/images/message-txt.png"
                                else if (list[1] == "mp3")
                                    groupfilefl.source = "qrc:/group/images/message-audio.png"
                                else if (list[1] == "mp4")
                                    groupfilefl.source = "qrc:/group/images/message-video.png"
                                else if (list[1] == "png" || list[1] == "jpg")
                                    groupfilefl.source = "qrc:/group/images/message-image.png"
                            }
                        }
                    }
                    Text {
                        id: statetextfl
                        anchors.right: parent.right
                        anchors.rightMargin: 24
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 24
                        font.pixelSize: 20
                        color: "#ffffff"
                        onTextChanged: {
                            if (itemfl.state == 3)
                                statetextfl.text = "已发送"
                            else if (itemfl.state == 1)
                                statetextfl.text = "发送中"
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (itemfl.state == 6)
                                getData.getMessageDownload(itemfl.messageId, groupid);
                            else if (itemfl.state == 8)
                            {
                                console.log("打开文件")
                            }
                        }
                    }
                }
            }
        }

        Component {
            id: fileComponentRight
            Item {
                id: itemfr
                width: listView.width
                property alias fileName: groupfilenamefr.text
                property var messageId: ""
                property var state: -1
                property alias myHeight: bubblefr.height
                property alias photoIcon: photofr.source
                property alias bubbleSource: bubblefr.source
                property alias stateText: statetextfr.text
                property var userId: ""
                Image {
                    id: photofr
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 90
                    height: 90
                    source: "qrc:/main/images/set_photo.jpg"
                    anchors.bottom: bubblefr.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":itemfr.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubblefr
                    width: 448; height: 198
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photofr.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_right.png"
                    Image {
                        id: groupfilefr
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        source: "qrc:/group/images/group-file-name.png"
                    }
                    Image {
                        id: groupfileupfr
                        width: 150
                        height: 150
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: 24
                        anchors.topMargin: 24
                        visible: itemfr.state == 6 ? true : false
                        source: "qrc:/group/images/group-file-ready.png"
                    }
                    Text {
                        id: groupfilenamefr
                        anchors.left: groupfilefr.right
                        anchors.leftMargin: 24
                        font.pixelSize: 40
                        elide: Text.ElideRight
                        anchors.top: parent.top
                        anchors.topMargin: 24
                        width: 150
                        color: "#ffffff"
                        onTextChanged: {
                            var list = groupfilenamefr.text.split(".")
                            if (list.length >= 1){
                                if (list[1] == "txt")
                                    groupfilefr.source = "qrc:/group/images/message-txt.png"
                                else if (list[1] == "mp3")
                                    groupfilefr.source = "qrc:/group/images/message-audio.png"
                                else if (list[1] == "mp4")
                                    groupfilefr.source = "qrc:/group/images/message-video.png"
                                else if (list[1] == "png" || list[1] == "jpg")
                                    groupfilefr.source = "qrc:/group/images/message-image.png"
                            }
                        }
                    }
                    Text {
                        id: statetextfr
                        anchors.right: parent.right
                        anchors.rightMargin: 24
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 24
                        font.pixelSize: 20
                        color: "#ffffff"
                        onTextChanged: {
                            if (itemfr.state == 3)
                                statetextr.text = "已发送"
                            else if (item.state == 1)
                                statetextfr.text = "发送中"
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (itemfr.state == 6)
                                getData.getMessageDownload(itemfr.messageId, groupid);
                            else if (item.state == 8)
                            {
                                console.log("打开文件")
                            }
                        }
                    }
                }
            }
        }


        Component {
            id: videoComponentLeft
            Item {
                id: itemvl
                property var state: -1
                property alias sourceTemp: tempsvl.text
                property alias myHeight: bubblevl.height
                property alias photoIcon: photovl.source
                property alias bubbleSource: bubblevl.source
                property var messageId: ""
                property var index: 0
                property var userId: ""
                Text {
                    id: tempsvl
                    text: qsTr("text")
                    visible: false
                    onTextChanged: {
                        console.log("onTextChanged",temps.text)
                        //videoLoader.item.videoSource = temps.text
                    }
                }
                Image {
                    id: photovl
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.jpg"
                    anchors.bottom: bubblevl.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":itemvl.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubblevl
                    width: 358; height: 358
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photo.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_left.png"
                    property var userId: ""
                    //                    Loader {
                    //                        id: videoLoader
                    //                        sourceComponent: videoPlayer
                    //                        height: 310; width: 310
                    //                        anchors.centerIn: parent
                    //                    }
                    Image {
                        id: groupfileupvl
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
                            qApp.openUrl("video://play?fileuri=" + tempsvl.text)
                        }
                        onPressAndHold: {
                            deleteback.visible = true
                            dialog.visible = true
                            deleteback.opacity = 0.3
                            var sour = itemir.bigsource.substring(7, itemir.bigsource.length)
                            dialog.source = sour
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
                id: itemvr
                width: listView.width
                property alias sourceTemp: tempsvr.text
                property alias myHeight: bubblevr.height
                property alias photoIcon: photovr.source
                property alias bubbleSource: bubblevr.source
                property var state: -1
                property var messageId: ""
                property var userId: ""
                Text {
                    id: tempsvr
                    text: qsTr("text")
                    visible: false
                    onTextChanged: {
                        //videoLoader.item.videoSource = temps.text
                    }
                }
                Image {
                    id: photovr
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 90
                    height: 90
                    source: "qrc:/main/images/set_photo.jpg"
                    anchors.bottom: bubblevr.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":itemvr.userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubblevr
                    width: 358; height: 358
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photo.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_right.png"
                    //                    Loader {
                    //                        id: videoLoader
                    //                        sourceComponent: videoPlayer
                    //                        height: 310; width: 310
                    //                        anchors.centerIn: parent
                    //                    }
                    Image {
                        id: groupfileupvr
                        width: 150
                        height: 150
                        anchors.centerIn: parent
                        //visible: item.state == 6 ? true : false
                        source: "qrc:/group/images/group-video-ready.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log(tempsvr.text)
                            qApp.openUrl("video://play?fileuri=" + tempsvr.text)
                        }
                        onPressAndHold: {
                            deleteback.visible = true
                            dialog.visible = true
                            deleteback.opacity = 0.3
                            var sour = itemir.bigsource.substring(7, itemir.bigsource.length)
                            dialog.source = sour
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
                id: itemil
                property var bigsource: ""
                property alias source: myImageil.source
                property alias myHeight: bubbleil.height
                property alias photoIcon: photoil.source
                property alias bubbleSource: bubbleil.source
                property var userId: ""
                property var myindex: 0
                property var messageId: ""
                Image {
                    id: photoil
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.jpg"
                    asynchronous: true
                    cache: true
                    anchors.bottom: bubbleil.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":itemil.userId,"contactertype":type})
                        }
                    }
                }
                BorderImage {
                    id: bubbleil
                    //width: 150; height: 92
                    width: 448; height: 448
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photoil.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_left.png"
                    Image {
                        id: myImageil
                        height: 400; width: 400
                        anchors.fill: parent
                        rotation: 90
                        asynchronous: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (myImageil.source == "qrc:/group/images/picture-broken.png")
                                return
                            var str = myImageil.source
                            str = str.toString().substring(7)
                            pageStack.push("qrc:/qml/PhotoViewer.qml",{"myindex":bigsource,myparent:chatRoom},true);
                            //var item = pageStack.push(fullImage,{"source":myImage.source})
                        }
                        onPressAndHold: {
                            deleteback.visible = true
                            dialog.visible = true
                            deleteback.opacity = 0.3
                            var sour = itemir.bigsource.substring(7, itemir.bigsource.length)
                            dialog.source = sour
                        }
                    }
                }
                Component.onCompleted: {
                    height = bubbleil.height
                }
            }
        }



        Component {
            id: imageComponentRight
            Item {
                id: itemir
                width: listView.width
                property var bigsource: ""
                property alias source: myImageir.source
                property alias myHeight: bubbleir.height
                property alias photoIcon: photoir.source
                property alias bubbleSource: bubbleir.source
                property var userId: ""
                property var myindex: 0
                property var messageId: ""
                Image {
                    id: photoir
                    width: 90
                    height: 90
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    source: "qrc:/main/images/set_photo.jpg"
                    asynchronous: true
                    cache: true
                    anchors.bottom: bubbleir.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":itemir.userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubbleir
                    width: 448; height: 448
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photoir.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_right.png"
                    Image {
                        id: myImageir
                        height: 400;
                        width: 400
                        anchors.fill: parent
                        rotation: 90
                        asynchronous: true
                    }
                    Rectangle {
                        id: sending
                        height: 448
                        width: 448
                        anchors.top: myImageir.top
                        anchors.right: myImageir.right
                        opacity: 0.3
                        visible: false
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (myImageir.source == "qrc:/group/images/picture-broken.png")
                                return
                            var str = myImageir.source
                            str = str.toString().substring(7)

                            pageStack.push("qrc:/qml/PhotoViewer.qml",{"myindex":bigsource,myparent:chatRoom},true);
                            //var item = pageStack.push(fullImage,{"source":myImage.source})
                        }
                        onPressAndHold: {
                            deleteback.visible = true
                            dialog.visible = true
                            deleteback.opacity = 0.3
                            var sour = itemir.bigsource.substring(7, itemir.bigsource.length)
                            dialog.source = sour
                        }
                    }
                    Connections {
                        target: getData
                        onSignalSendPercent: {
                            console.debug("onSignalSendPercent:" + recvNum + allNum)
                            if (sendGroupId == groupid)
                            {
                                if (recvNum == allNum){
                                    sending.visible = false
                                    return
                                }
                                if (sendMessageid == itemir.messageId){
                                    sending.visible = true
                                    sending.width = (1 - (recvNum / allNum)) * 448
                                }
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    height = myImageir.height
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
                id: itemtl
                property alias text: myTexttl.text
                property alias myHeight: bubbletl.height
                property alias photoIcon: phototl.source
                property alias bubbleSource: bubbletl.source
                property var userId: ""
                Image {
                    id: phototl
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: photoIcon//"qrc:/main/images/init_photo.jpg"
                    anchors.bottom: bubbletl.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var type = "3";
                            if (getData.bIsOrganization(userId))
                                type = "1";
                            else
                                type = "2";
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"network","userId":itemtl.userId,"contactertype":type})
                        }
                    }
                }

                BorderImage {
                    id: bubbletl
                    width: 150; height: 92
                    border.left: 20; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.left: phototl.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_left.png"

                    Text {
                        id: myTexttl
                        text: qsTr("")
                        color: "#ffffff"
                        font.pixelSize: 40
                        wrapMode: Text.Wrap
                        anchors.margins: 24
                        anchors.fill: parent
                        //horizontalAlignment: Qt.AlignLeft
                        onContentSizeChanged: {
                            if (contentWidth < 410){
                                bubbletl.width = contentWidth + 48
                            }else {
                                bubbletl.width = 458
                                myTexttl.width = 410
                            }

                            bubbletl.height = myTexttl.contentHeight + 48
                        }
                    }
                }
            }
        }

        Component {
            id: textComponentRight
            Item {
                id: itemtr
                width: listView.width
                property alias text: myTexttr.text
                property alias myHeight: bubbletr.height
                property alias photoIcon: phototr.source
                property alias bubbleSource: bubbletr.source
                property var userId: ""
                Image {
                    id: phototr
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    width: 90
                    height: 90
                    source: photoIcon//"qrc:/set/images/set_photo.jpg"
                    anchors.bottom: bubbletr.bottom
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            pageStack.push("qrc:/qml/PersonInfo.qml",{"fromText":"local"})
                        }
                    }
                }

                BorderImage {
                    id: bubbletr
                    width: 150; height: 92
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: phototr.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_right.png"

                    Text {
                        id: myTexttr
                        text: qsTr("")
                        color: "#ffffff"
                        font.pixelSize: 40
                        wrapMode: Text.Wrap
                        anchors.margins: 24
                        anchors.fill: parent
                        //horizontalAlignment: Qt.AlignRight
                        onContentSizeChanged: {
                            if (contentWidth < 410){
                                bubbletr.width = myTexttr.contentWidth + 48
                            }else {
                                bubbletr.width = 458
                                myTexttr.width = 410
                            }
                            bubbletr.height = myTexttr.contentHeight + 48
                        }
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onPressAndHold: {
                        console.log("onPressAndHold")
                    }
                }
                Component.onCompleted: {
                    itemtr.height = bubbletr.height
                    bubbletr.height = myTexttr.contentHeight + 48
                }
            }
        }

        Component {
            id: audioComponentLeft
            Item {
                id: itemal
                property  var isRead: -1
                property alias source: myImageal.source
                property alias audioSource: audioal.source
                property alias myHeight: bubbleal.height
                property alias photoIcon: photoal.source
                property alias bubbleSource: bubbleal.source
                property var userId: ""
                property var messageId: ""
                Image {
                    id: photoal
                    width: 90
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 24
                    source: "qrc:/main/images/init_photo.jpg"
                    anchors.bottom: bubbleal.bottom
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
                    id: bubbleal
                    //width: 150; height: 92
                    width: 256; height: 93
                    border.left: 20; border.top: 60
                    border.right: 10; border.bottom: 20
                    anchors.left: photoal.right
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_left.png"
                    Image {
                        id: myImageal
                        height: 40
                        width: 40
                        anchors.left: parent.left
                        anchors.leftMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-stop-left.png"
                    }
                    AnimatedImage {
                        id: playingal
                        height: 40
                        width: 40
                        anchors.left: parent.left
                        anchors.leftMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-play-left.gif"
                        visible: false
                    }
                    Audio {
                        id: audioal
                        autoPlay: false
                        onStatusChanged: {
                            if (audioal.status == 3){
                                timeL = audioal.duration / 1000
                                timeLengthleft.text = timeL + 1 + "'"
                            }
                            if (audioal.status == 3 || audioal.status == 7){
                                playingal.visible = false;
                                myImageal.visible = true
                            }
                            else if (audioal.status == 6){
                                playingal.visible = true;
                                myImageal.visible = false
                            }
                        }
                    }
                    Connections {
                        target: chatRoom
                        onStopplay:{
                            audioar.stop()
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            chatRoom.stopplay()
                        }
                        onReleased: {
                            if (audioal.status == 3 || audioal.status == 7)
                            {
                                getData.setMessageIsread(true, itemal.messageId, groupid)
                                audioal.play()
                                readTipleft.visible = false
                            }
                            if (audioal.status == 3 || audioal.status == 7)
                                audioal.play()
                            else if (audio.status == 6)
                                audioal.stop()
                        }
                    }
                }

                Text {
                    id: timeLengthleft
                    height: 93
                    anchors.left: bubbleal.right
                    anchors.leftMargin: 16
                    anchors.top: bubbleal.top
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 20
                    color: "#36465a"
                }
                Item {
                    id: readTipleft
                    visible: isRead == 1 ? false : true
                    height: 93
                    width: 20
                    anchors.left: timeLengthleft.right
                    anchors.leftMargin: 16
                    anchors.top: bubbleal.top
                    Rectangle {
                        height: 10
                        width: 10
                        anchors.centerIn: parent
                        radius: 50
                        color: "#ff5755"
                    }
                }
            }
        }

        Component {
            id: audioComponentRight
            Item {
                id: itemar
                width: listView.width
                property  var isRead: -1
                property alias source: myImagear.source
                property alias audioSource: audioar.source
                property alias myHeight: bubblear.height
                property alias photoIcon: photoar.source
                property alias bubbleSource: bubblear.source
                property var userId: ""
                property var messageId: ""
                Image {
                    id: photoar
                    width: 90
                    height: 90
                    anchors.right: parent.right
                    anchors.rightMargin: 24
                    source: "qrc:/main/images/set_photo.jpg"
                    anchors.bottom: bubblear.bottom
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
                    id: bubblear
                    width: 256; height: 93
                    border.left: 10; border.top: 60
                    border.right: 20; border.bottom: 20
                    anchors.right: photoar.left
                    anchors.rightMargin: 16
                    anchors.top: parent.top
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "qrc:/main/images/ppt_right.png"
                    Image {
                        id: myImagear
                        height: 40
                        width: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-stop-right.png"
                    }
                    AnimatedImage {
                        id: animatedar
                        height: 40
                        width: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 42
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/chatroom/images/audio-play-right.gif"
                        visible: false
                    }
                    Audio {
                        id: audioar
                        autoPlay: false
                        onStatusChanged: {
                            if (audioar.status == 3){
                                timeL = audioar.duration / 1000
                                timeLength.text = timeL + 1 + "'"
                            }

                            if (audioar.status == 3 || audioar.status == 7){
                                animatedar.visible = false;
                                myImagear.visible = true
                            }
                            else if (audioar.status == 6){
                                animatedar.visible = true;
                                myImagear.visible = false
                            }
                        }
                    }
                    Connections {
                        target: chatRoom
                        onStopplay:{
                            audioar.stop()
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            chatRoom.stopplay()
                        }
                        onReleased: {
                            if (audioar.status == 3 || audioar.status == 7)
                            {
                                getData.setMessageIsread(true, itemar.messageId, groupid)
                                audioar.play()
                                readTipar.visible = false
                            }
                            else if (audioar.status == 6)
                            {
                                audioar.stop()
                            }
                        }
                        onPressAndHold: {
                            console.log("onPressAndHold")
                        }
                    }
                }
                Text {
                    id: timeLength
                    height: 93
                    anchors.right: bubblear.left
                    anchors.rightMargin: 16
                    anchors.top: bubblear.top
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 20
                    color: "#36465a"
                }
                Item {
                    id: readTipar
                    visible: isRead == 1 ? false : true
                    height: 93
                    width: 20
                    anchors.right: timeLength.left
                    anchors.rightMargin: 16
                    anchors.top: bubblear.top
                    Rectangle {
                        height: 10
                        width: 10
                        anchors.centerIn: parent
                        radius: 50
                        color: "#ff5755"
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
            property var source: ""
            visible: false
            anchors.centerIn: parent
            width: 492
            height: 120
            z: 4
//            Image {
//                anchors.fill: parent
//                source: "qrc:/history/images/dialog.png"
//            }
            Column {
                width: 492
                height: 120
                CButton {
                    width: 492
                    height: 120
                    backgroundEnabled: false
                    text: "另存为"
                    textColor: "#ffffff"
                    pixelSize: 30
                    onClicked: {
                        getData.saveAsFile(dialog.source)
                        deleteback.visible = false
                        dialog.visible = false
                        deleteback.opacity = 0
                    }
                }
            }
        }


        Component {
            id: leftTitleBar
            CButton {
                anchors.fill: parent
                iconSource: "qrc:/main/images/back.png"
                backgroundEnabled: false
                text: "返回"
                textColor: "#29afe2"
                spacing: 0
                onClicked: {
                    pageStack.pop(prepage)

                }
            }
        }

        Component {
            id: rightTitleBar
            CButton {
                visible: groupid == "" ? false : true
                backgroundEnabled: false
//                text: "详情"
//                textColor: "#ffffff"
                iconSource: "qrc:/main/images/icon_people.png"
//                iconSource: pressed ? "qrc:/chatroom/images/chatroom_righttitle_click.png" :
//                                      "qrc:/chatroom/images/chatroom_righttitle.png"
                onClicked: {
//                    pageStack.deleteCachedPage(groupid)
//                    getData.removeCachePage(groupid)
//                    pageStack.pop(prepage)
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



