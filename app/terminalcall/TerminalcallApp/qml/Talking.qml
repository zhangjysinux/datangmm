import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: talking
    anchors.fill: parent
    property int netFlag: 0
    property string handId: ""
    property int qmlState: 0
    property alias numtemp: name1.text
    property alias bt1: button1.enabled
    property bool bIsChange: false

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        console.log("make new talking ")
        talking.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            bIsChange = true
            talking.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            bottomback.height = 350
            row1.anchors.topMargin = 0;
            button4.anchors.topMargin = 0;
            photo.width = 245
            photo.height = 245
            if (qmlState == 2)
                time.visible = false
            else if (qmlState == 1)
                time.visible = true
            photo.anchors.topMargin = 5
            photo.anchors.horizontalCenterOffset = -(parent.width / 2 - 200)
            photo.width = 245
            photo.height = 245

            time.anchors.top = topNet.top
            time.anchors.topMargin = 0
            time.anchors.horizontalCenterOffset = topNet.width + 10
        } else {
            bIsChange = false
            talking.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            bottomback.height = 480
            row1.anchors.topMargin = 48;
            button4.anchors.topMargin = 48;
            if (qmlState == 2)
                time.visible = false
            else if (qmlState == 1)
                time.visible = true

            photo.anchors.topMargin = 66
            photo.anchors.horizontalCenterOffset = 0
            photo.width = 320
            photo.height = 320
            time.anchors.top =  topNet.bottom
            time.anchors.topMargin = 23
            time.anchors.horizontalCenterOffset = 0

        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                bIsChange = true
                talking.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                bottomback.height = 350
                row1.anchors.topMargin = 0;
                button4.anchors.topMargin = 0;

                photo.anchors.topMargin = 5
                photo.anchors.horizontalCenterOffset = -(parent.width / 2 - 200)
                photo.width = 245
                photo.height = 245

                time.anchors.top = topNet.top
                time.anchors.topMargin = 0
                time.anchors.horizontalCenterOffset = topNet.width + 10
                nametitle.anchors.top = photo.top
                nametitle.anchors.horizontalCenterOffset = -90
            } else {
                bIsChange = false
                talking.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                bottomback.height = 480
                row1.anchors.topMargin = 48;
                button4.anchors.topMargin = 48;

                photo.anchors.topMargin = 66
                photo.anchors.horizontalCenterOffset = 0
                photo.width = 320
                photo.height = 320
                time.anchors.top =  topNet.bottom
                time.anchors.topMargin = 23
                time.anchors.horizontalCenterOffset = 0
                nametitle.anchors.top = photo.bottom
                nametitle.anchors.horizontalCenterOffset = 0
            }
        }
    }
    //gInputContext.softwareInputPanelVisible

    Connections {
        target: csDbus
        onSignalCloseAnother: {
            if (colseHandlerId == dialog.dialogHandId)
                dialog.visible = false
        }
    }
    Connections {
        target: csDbus
        onSignalAudioModeChanged: {
            if (isHandsFree){
                button3.iconSource = "qrc:/main/images/3-1.png"
                loader.anchors.right = row1.right
                loader.anchors.top = row1.top
                loader.visible = true
            }
            else
            {
                loader.visible = false
                button3.iconSource = "qrc:/main/images/3.png"
            }
        }
        onSignalMicrophoneMuteChanged: {
            if (on){
                button2.iconSource = "qrc:/main/images/2-1.png"
                loader2.anchors.right = row1.right
                loader2.anchors.rightMargin = 220
                loader2.anchors.top = row1.top
                loader2.visible = true
            }
            else{
                loader2.visible = false
                button2.iconSource = "qrc:/main/images/2.png"
            }
        }
    }

    ListModel {
        id: myHandlerList
    }
    ListModel {
        id: myMettingList
    }
    Connections {
        target: csDbus
        onSignalAnotherIncoming: {
            console.log("onSignalAnotherIncomingg",handlerId)
            for (var i = 0;i<myHandlerList.count;i++){
                if (myHandlerList.get(i).handlerId == handlerId){
                    dialog.visible = true
                    if (myHandlerList.get(i).photopath != "")
                        anotherphoto.iconSource = myHandlerList.get(i).photopath
                    dialognum.text = myHandlerList.get(i).dialNumber
                    dialogname.text = myHandlerList.get(i).name
                    dialog.dialogHandId = myHandlerList.get(i).handlerId
                    break;
                }
            }
        }
    }

    Connections {
        target: csDbus
        onSignalUpdateTimer: {
            for (var i = 0;i<myHandlerList.count;i++){
                if (myHandlerList.get(i).handlerId == handlerId){
                    myHandlerList.set(i,{"mytime":timeString})
                    time.text = myHandlerList.get(0).mytime
                    break;
                }
            }
        }
    }

    Connections {
        target: csDbus
        onSignalUpdateContacterInfo: {
            for (var i = 0;i<myHandlerList.count;i++){
                if (myHandlerList.get(i).dialNumber == dialnum){
                    myHandlerList.set(i,{"userid":id,"name":name,"title":title,"photopath":photoPath})
                    break;
                }
            }
        }
    }

    Connections {
        target: csDbus
        onSignalHandlerListChanged: {
            qmlState = csDbus.getQmlState()
            if (qmlState == 2){
                nametitle.visible = false
                photo.visible = false
                list.visible = true
                time.visible = false
                button4.iconSource = "qrc:/main/images/change.png"
                list.interactive = false
            }else if (qmlState == 1){
                button4.iconSource = "qrc:/main/images/4.png"
                nametitle.visible = true
                photo.visible = true
                list.visible = false
                time.visible = true
            }else if (qmlState == 3){
                button4.iconSource = "qrc:/main/images/4.png"
                nametitle.visible = false
                photo.visible = false
                list.visible = true
                time.visible = true
                list.interactive = true
            }
            else if (qmlState == 4)
            {
                button4.iconSource = "qrc:/main/images/change.png"
                nametitle.visible = false
                photo.visible = false
                list.visible = true
                time.visible = true
                list.interactive = false
            }

            myHandlerList.clear()
            setHandlerList()
        }
    }

    function setHandlerList()
    {
        qmlState = csDbus.getQmlState()
        console.log("setHandlerList in Talking.qml",csDbus.getHandlerCount(),qmlState)
        if (qmlState == 1 || qmlState ==2 ){
            for (var i = 0; i < csDbus.getHandlerCount(); i++){
                myHandlerList.append({"handlerId":csDbus.getHandlerValue(i,1),"dialNumber":csDbus.getHandlerValue(i,2),
                                         "network":csDbus.getHandlerValue(i,3),"state":csDbus.getHandlerValue(i,4),"isIncoming":csDbus.getHandlerValue(i,5),
                                         "isMultiparty":csDbus.getHandlerValue(i,6),"isMuted":csDbus.getHandlerValue(i,7),"isHandsfree":csDbus.getHandlerValue(i,8),
                                         "isRecord":csDbus.getHandlerValue(i,9),"name":"","title":"","userid":"","photopath":"testpath","mytime":"保持","isOnline":true});
                csDbus.getNamebyDialnum(csDbus.getHandlerValue(i,2))
            }
            if (csDbus.getHandlerCount() > 0){
                if (qmlState == 1 && csDbus.getHandlerValue(0,4) == 1)
                    button1.enabled = true
            }
            if (qmlState == 2 && csDbus.getHandlerValue(0,3) == csDbus.getHandlerValue(1,3))
                merge.visible = true
            if (qmlState == 2)
                button1.enabled = false
        }
        else if (qmlState == 3)
        {
            for (var i = 0; i < csDbus.getMettingCount(); i++){
                myHandlerList.append({"handlerId":csDbus.getMettingValue(i,1),"dialNumber":csDbus.getMettingValue(i,2),
                                         "network":csDbus.getMettingValue(i,3),"state":csDbus.getMettingValue(i,4),"isIncoming":csDbus.getMettingValue(i,5),
                                         "isMultiparty":csDbus.getMettingValue(i,6),"isMuted":csDbus.getMettingValue(i,7),"isHandsfree":csDbus.getMettingValue(i,8),
                                         "isRecord":csDbus.getMettingValue(i,9),"isOnline":csDbus.getMettingValue(i,10), "name":"","title":"","userid":"","photopath":"testpath","mytime":"保持","isOnline":true});
                csDbus.getNamebyDialnum(csDbus.getMettingValue(i,2))
            }
            merge.visible = false
        }
        else if (qmlState == 4)
        {
            myHandlerList.append({"handlerId":csDbus.getMettingValue(0,1),"dialNumber":csDbus.getMettingValue(0,2),
                                     "network":csDbus.getMettingValue(0,3),"state":csDbus.getMettingValue(0,4),"isIncoming":csDbus.getMettingValue(0,5),
                                     "isMultiparty":csDbus.getMettingValue(0,6),"isMuted":csDbus.getMettingValue(0,7),"isHandsfree":csDbus.getMettingValue(0,8),
                                     "isRecord":csDbus.getMettingValue(0,9),"name":"电话会议","title":"","userid":"","photopath":"testpath","mytime":"保持","isOnline":true});

            var count = csDbus.getHandlerCount() - 1;
            myHandlerList.append({"handlerId":csDbus.getHandlerValue(count,1),"dialNumber":csDbus.getHandlerValue(count,2),
                                     "network":csDbus.getHandlerValue(count,3),"state":csDbus.getHandlerValue(count,4),"isIncoming":csDbus.getHandlerValue(count,5),
                                     "isMultiparty":csDbus.getHandlerValue(count,6),"isMuted":csDbus.getHandlerValue(count,7),"isHandsfree":csDbus.getHandlerValue(count,8),
                                     "isRecord":csDbus.getHandlerValue(count,9),"name":"","title":"","userid":"","photopath":"testpath","mytime":"保持","isOnline":true});
            csDbus.getNamebyDialnum(csDbus.getHandlerValue(count,2))

            if (csDbus.getMettingValue(0,3) == csDbus.getHandlerValue(count,3))
                merge.visible = true
            else
                merge.visible = false
        }
        if (qmlState == 1)
        {
            netText1.visible = true
            netIcon1.visible = true
        }
        else {
            netIcon1.visible = false
            netText1.visible = false
        }
        setStatus();
        console.log("setHandlerList in Talking.qml end")
    }
    function setStatus()
    {
        if (myHandlerList.count == 0)
            return;
        else if (myHandlerList.count > 1 )
        {

            for (var i = 0; i < myHandlerList.count; i++){
                if (myHandlerList.get(i).state == 4  || myHandlerList.get(i).state == 3 )//|| myHandlerList.get(i).state == 4)
                {
                    handId = myHandlerList.get(i).handlerId
                    netFlag = myHandlerList.get(i).network
                    if (i != 0)
                        myHandlerList.move(i,0,1)
                    break
                }
            }
            for (var i = 0; i < myHandlerList.count; i++){
                if (myHandlerList.get(i).state == 1 )// || myHandlerList.get(i).state == 3 || myHandlerList.get(i).state == 4)
                {
                    handId = myHandlerList.get(i).handlerId
                    netFlag = myHandlerList.get(i).network
                    if (i != 0)
                        myHandlerList.move(i,0,1)
                    break
                }
            }
        }
        findNetText()
        if (myHandlerList.get(0).name != "")
            name1.text = myHandlerList.get(0).name
        else
            name1.text = myHandlerList.get(0).dialNumber
        if (myHandlerList.get(0).photopath != ""){
            photo.iconSource = myHandlerList.get(0).photopath
        }
        else
            photo.iconSource = "qrc:/main/images/init_photo.jpg"
        handId = myHandlerList.get(0).handlerId
        netFlag = myHandlerList.get(0).network

//        if (myHandlerList.get(0).isHandsfree == true){
//            button3.iconSource = "qrc:/main/images/3-2.png"
//            loader.anchors.right = row1.right
//            loader.anchors.top = row1.top
//            loader.visible = true
//        }else{
//            loader.visible = false
//            button3.iconSource = button3.pressed ? "qrc:/main/images/3-1.png" : "qrc:/main/images/3.png"
//        }

//        if (myHandlerList.get(0).isMuted == true){
//            button2.iconSource = "qrc:/main/images/2-2.png"
//            loader2.anchors.right = row1.right
//            loader2.anchors.rightMargin = 220
//            loader2.anchors.top = row1.top
//            loader2.visible = true
//        }else{
//            loader2.visible = false
//            button2.iconSource = button2.pressed ? "qrc:/main/images/2-1.png" : "qrc:/main/images/2.png"
//        }

        if (myHandlerList.get(0).isRecord == true){
            button6.iconSource = "qrc:/main/images/6-2.png"
            loader6.anchors.right = row1.right
            loader6.anchors.rightMargin = 0
            loader6.anchors.top = button6.top
            //loader6.anchors.topMargin = 148
            loader6.visible = true
        }else{
            loader6.visible = false
            button6.iconSource = button6.pressed ? "qrc:/main/images/6-1.png" : "qrc:/main/images/6.png"
        }
    }

    function setMettingList()
    {

    }

    function findNetText()
    {
        var myNetText = "cc";
        if (myHandlerList.get(0).network == 1)
            myNetText = "自组网"
        else if (myHandlerList.get(0).network == 16)
            myNetText = "LTE"
        else if (myHandlerList.get(0).network == 256)
            myNetText = "天通网"
        else if (myHandlerList.get(0).network == 1048576)
            myNetText = "WIFI"
        netText.text = myNetText
    }

    contentAreaItem: Item {
        anchors.fill: parent
//        Item {
//            id: mid
//            anchors.fill: parent
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

        CButton {
            id: video
            width: 100
            height: 100
            anchors.left: parent.left
            anchors.bottom: bottomback.top
            backgroundEnabled: false
            iconSource: pressed ? "qrc:/video/images/changvideo-click.png" : "qrc:/video/images/changevideo.png"
            visible: csDbus.isVideoExit() ? true : false
            onClicked: {
                pageStack.replace("qrc:/qml/VideoCall.qml",{"callId":csDbus.getVideoValue("handlerId"),"phoneNum":getData.getVideoValue("phoneNum"),"network":getData.getVideoNetWork()},true)
            }
        }

        ListView {
            id: list
            visible: true
            anchors.top: parent.top
            anchors.bottom: bottomback.top
            width: parent.width
            model: myHandlerList
            delegate: Item {
                width: parent.width
                height: 62
//                Image {
//                    id: back
//                    width: parent.width
//                    height: 62
//                    source: "qrc:/main/images/list-line.png"
//                }
                Text {
                    height: 62
                    color: "#ffffff"
                    font.pixelSize: 32
                    text: name + " " + title
                    anchors.left: parent.left
                    anchors.leftMargin: 36
                    verticalAlignment: Qt.AlignVCenter
                }
                Text {
                    height: 62
                    color: "#ffffff"
                    font.pixelSize: 32
                    text: mytime
                    anchors.right: parent.right
                    anchors.rightMargin: 48
                    verticalAlignment: Qt.AlignVCenter
                    visible: false//isOnline
                }
                Image {
                    id: miss
                    source: "qrc:/main/images/missline.png"
                    verticalAlignment: Qt.AlignVCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 48
                    visible: !isOnline
                }
                CLine {
                    anchors.bottom: parent.bottom
                    color: "#36465a"
                }
            }
        }
        CButton {
            id: merge
            visible: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 160
            backgroundEnabled: false
            iconSource: "qrc:/main/images/merge.png"
            onClicked: {
                csDbus.createConfCall(csDbus.getHandlerValue(0,3))
                //csDbus.hold(myHandlerList.get(1).handlerId , false)
            }
        }

        CMaskImage {
            id: photo
            width: 320
            height: 320
            iconSource: "qrc:/main/images/init_photo.jpg"
            anchors.top: time.bottom
            anchors.topMargin: 66
            anchors.horizontalCenter: parent.horizontalCenter
            maskComponent: Rectangle{
                id: msk
                parent: photo
                radius: photo.width  / 2
                anchors.fill: parent
                smooth: true
                visible: false
            }
        }


        Item {
            id: nametitle
            anchors.top: photo.bottom
            anchors.topMargin: 43
            width: 400 //net.width + netIcon.width + netText.width
            anchors.horizontalCenter: parent.horizontalCenter
            visible: true
            height: name.height + 24 + net.height
            property alias name: name1.text
            //property alias net: netText.text
            Text {
                id: name1
                height: 40
                text: "13888888888"
                font.pixelSize: 40;color: "#29afe2"
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Qt.AlignHCenter
                width: 400
                elide: Text.ElideRight
            }
            Text {
                id: personPhoneNum
                height: 40
                text: ""
                font.pixelSize: 40;color: "#29afe2"
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Qt.AlignHCenter
                width: 400
                elide: Text.ElideRight
            }
            Text {
                id: net
                visible: false
                height: 40
                text: qsTr("对方")
                anchors.top: name1.bottom
                anchors.topMargin: 16
                font.pixelSize: 40;color: "#c9c9c9"
                //anchors.horizontalCenter: parent.horizontalCenter
            }
            Image {
                id: netIcon
                visible: false
                source: "qrc:/main/images/net-on.png"
                anchors.top: net.top
                anchors.left: net.right
                anchors.leftMargin: 8
            }
            Text {
                id: netText
                visible: false
                height: 40
                text: ""
                anchors.top: net.top
                anchors.left: netIcon.right
                anchors.leftMargin: 8
                font.pixelSize: 40;color: "#c9c9c9"
                //anchors.horizontalCenter: parent.horizontalCenter
                onTextChanged: {
                    netText1.text = netText.text
                }
            }
        }
        Item {
            id: topNet
            anchors.top: parent.top
            anchors.topMargin: 16
            width: netIcon1.width + netText1.width
            anchors.horizontalCenter: parent.horizontalCenter
            visible: true
            height: 40
            Text {
                id: netText1
                height: 40
                text: ""
                anchors.top: parent.top
                anchors.left: parent.left
                font.pixelSize: 40;color: "#29afe2"
            }
            Image {
                id: netIcon1
                source: "qrc:/main/images/net-on.png"
                anchors.top: parent.top
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: netText1.right
                anchors.leftMargin: 8
            }
        }
        Item {
            id: time
            anchors.top: topNet.bottom
            width: timetext.width
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            property alias text: timetext.text
            visible: true
            Text {
                id: timetext
                height: 30
                text: ""
                font.pixelSize: 30
                color: "#29afe2"
                anchors.centerIn: parent
                horizontalAlignment: Qt.AlignRight
            }
        }

        Rectangle {
            id: bottomback
            anchors.bottom: parent.bottom
            height: 480
            width: parent.width
            color: "#ffffff"
            //source: "qrc:/main/images/bottom-back.png"
            //visible: false
        }

        Row {
            id: row1
            height: 100
            width: 660
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: bottomback.top
            anchors.topMargin: 48
            CButton {
                id: button1
                height: 100;width: 220
                iconSource: "qrc:/main/images/1.png"
                backgroundEnabled: false
                onClicked: {
                    pageStack.push("qrc:/qml/PersonList.qml",{parentSource:talking})
                }
            }
            CButton {
                id: button2
                height: 100;width: 220
                iconSource: "qrc:/main/images/2.png"
                backgroundEnabled: false
                onClicked: {
                    var net
                    if (qmlState == 1 || qmlState == 2)
                        net = myHandlerList.get(0).network
                    else if (qmlState == 3)
                        net = myHandlerList.get(0).network
                    csDbus.setMuteMicrophone(netFlag,!csDbus.getIsMicrophoneMuted(net))
                }
            }
            CButton {
                id: button3
                height: 100;width: 220
                iconSource: "qrc:/main/images/3.png"
                backgroundEnabled: false
                onClicked: {
                    var net
                    if (qmlState == 1 || qmlState == 2)
                        net = myHandlerList.get(0).network
                    else if (qmlState == 3)
                        net = myHandlerList.get(0).network
                    var value = csDbus.getAudioMode(net);       //免提"ihf"表示免提， "earpiece"表示听筒
                    if (value == "ihf")
                        value = "earpiece"
                    else if (value == "earpiece")
                        value = "ihf"
                    csDbus.setAudioMode(netFlag,value)
                }
            }
        }

        CButton {
            id: button4
            height: 100;width: 220
            iconSource: "qrc:/main/images/4.png"
            backgroundEnabled: false
            anchors.top: row1.bottom
            anchors.topMargin: 48
            anchors.left: row1.left
            onPressed: {
                if (qmlState == 1 || qmlState == 3)
                    iconSource = "qrc:/main/images/4.png"
                else if (qmlState == 2 || qmlState ==4 )
                    iconSource = "qrc:/main/images/change-click.png"
            }
            onReleased: {
                if (qmlState == 1 || qmlState == 3)
                    iconSource = "qrc:/main/images/4.png"
                else if (qmlState == 2 || qmlState ==4 )
                    iconSource = "qrc:/main/images/change.png"
            }

            onClicked: {
                //切换
                if (myHandlerList.count == 2 && qmlState == 2)
                {
                    var handler1 = myHandlerList.get(0).handlerId
                    var handler2 = myHandlerList.get(1).handlerId
                    if (myHandlerList.get(0).state == 1 || myHandlerList.get(0).state == 2)
                        csDbus.hold(handler1, true)
                    if (myHandlerList.get(1).state == 1 || myHandlerList.get(1).state == 2)
                        csDbus.hold(handler2, false)
                    //                    for (var i = 0 ;i < myHandlerList.count; i++)
                    //                    {
                    //                        if (myHandlerList.get(i).state == 1)
                    //                            csDbus.hold(myHandlerList.get(i).handlerId, true)
                    //                        if (myHandlerList.get(i).state == 2)
                    //                            csDbus.hold(myHandlerList.get(i).handlerId, false)
                    //                    }
                }
                else if (myHandlerList.count == 1 && qmlState == 1)
                {
                    if (myHandlerList.get(0).state == 1)
                        csDbus.hold(myHandlerList.get(0).handlerId, true)
                    if (myHandlerList.get(0).state == 2)
                        csDbus.hold(myHandlerList.get(0).handlerId, false)
                }
            }
        }
        CButton {
            id: button5
            height: 100;width: 220
            iconSource: "qrc:/main/images/5.png"
            backgroundEnabled: false
            anchors.left: button4.right
            anchors.top: button4.top
            onClicked: {
                loaderr.visible = true
            }
        }
        CButton {
            id: button6
            height: 100;width: 220
            iconSource: "qrc:/main/images/6.png"
            backgroundEnabled: false
            anchors.left: button5.right
            anchors.top: button4.top
            onClicked: {
                if (button6.iconSource == "qrc:/main/images/6-1.png"){
                    button6.iconSource = "qrc:/main/images/6.png"
                    loader6.visible = false
                    if (myHandlerList.get(0).network == 1 || myHandlerList.get(0).network == 16)
                        csDbus.onRecord(myHandlerList.get(0).network,true);
                }
                else{
                    button6.iconSource = "qrc:/main/images/6-1.png"
                    loader6.anchors.right = row1.right
                    loader6.anchors.rightMargin = 0
                    loader6.anchors.top = button6.top
                    loader6.visible = true
                    if (myHandlerList.get(0).network == 1 || myHandlerList.get(0).network == 16)
                        csDbus.onRecord(myHandlerList.get(0).network,false);
                }
            }
        }


        CButton {
            id: hangUp
            backgroundEnabled: false
            width: 130; height: 130
            iconSource: "qrc:/main/images/hangup.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: button6.bottom
            anchors.topMargin: 18
            onClicked: {
                console.log("hangup in qml")
                if (myHandlerList.count == 0 && myMettingList.count == 0)
                {
                    csDbus.hangup("")
                }
                else{
                    if (qmlState == 1)
                    {
                        handId = myHandlerList.get(0).handlerId
                        csDbus.hangup(handId)
                    }
                    else if (qmlState == 2)
                    {
                        //if (myHandlerList.get(0).state == )
                        handId = myHandlerList.get(0).handlerId
                        //var activate = myHandlerList.get(1).handlerId
                        csDbus.hangup(handId)
                        //csDbus.hold(activate, false)
                    }
                    else if (qmlState == 3)
                    {
                        csDbus.onHangupMultipartyCalls(myHandlerList.get(0).network)
                    }
                    else if (qmlState == 4)
                    {
                        handId = myHandlerList.get(0).handlerId
                        csDbus.hangup(handId)
                    }
                }
            }
        }

        Rectangle {
            id: deleteback
            anchors.fill: parent
            color: "black"
            opacity: 0.7
            visible: dialog.visible == true ? true : false
            MouseArea {
                anchors.fill: parent
            }
        }
        Rectangle {
            id: dialog
            visible: false
            anchors.centerIn: parent
            width: 492
            height: 386
            z: 5
            property var dialogHandId: ""
            color: "#ffffff"
            radius: 10
            CMaskImage {
                id: anotherphoto
                width: 100
                height: 100
                iconSource: "qrc:/main/images/init_photo.jpg"
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.horizontalCenter: parent.horizontalCenter
                maskComponent: Rectangle{
                    id: anothermsk
                    parent: anotherphoto
                    radius: anotherphoto.width  / 2
                    anchors.fill: parent
                    smooth: true
                    visible: false
                }
            }
            Text {
                id: dialogname
                height: 32
                font.pixelSize: 32
                color: "#29afe2"
                anchors.top: anotherphoto.bottom
                anchors.topMargin: 16
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: dialognum
                height: 32
                font.pixelSize: 32
                color: "#29afe2"
                anchors.top: dialogname.bottom
                anchors.topMargin: 8
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row {
                spacing: 48
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16
                anchors.horizontalCenter: parent.horizontalCenter

                CButton {
                    id: deletecancel
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/reject.png"
                    onClicked: {
                        csDbus.hangup(dialog.dialogHandId)
                        dialog.visible = false
                    }
                }
                CButton {
                    id: deletesure
                    width: 150
                    height: 150
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/accept.png"
                    onClicked: {
                        /*
                        for (var i = 0; i < myHandlerList.count ;i++)
                        {
                            if (dialog.dialogHandId == myHandlerList.get(i).handlerId)
                            {
                                if (qmlState == 2)
                                {
                                    if (myHandlerList.get(1).network == myHandlerList.get(0).network == 1)
                                    {
                                        break
                                    }
                                    else if (myHandlerList.get(1).network != myHandlerList.get(0).network)
                                    {
                                        if (i == 0 )
                                            csDbus.hold(myHandlerList.get(1).handlerId, true)
                                        else if (i == 1)
                                            csDbus.hold(myHandlerList.get(0).handlerId, true)
                                        break
                                    }
                                }
                                else if (qmlState == 4)
                                {
                                    if (i == 0){
                                        if (myHandlerList.get(i).network == myHandlerList.get(1).network == 1)
                                            break
                                        else {
                                            for (var j = 0; j < myHandlerList.count ;j++)
                                            {
                                                if (dialog.dialogHandId != myHandlerList.get(j).handlerId)
                                                    csDbus.hold(myHandlerList.get(0).handlerId, true)
                                            }
                                            break
                                        }
                                    }
                                    else if (i != 0 ){
                                        if (myHandlerList.get(i).network == myHandlerList.get(0).network == 1)
                                            break
                                        else {
                                            for (var m = 0; m < myHandlerList.count ;m++)
                                            {
                                                if (dialog.dialogHandId != myHandlerList.get(m).handlerId)
                                                    csDbus.hold(myHandlerList.get(0).handlerId, true)
                                            }
                                            break
                                        }
                                    }

                                }

//                                if (myHandlerList.get(i).network == myHandlerList.get(0).network){
//                                    csDbus.answer(dialog.dialogHandId)
//                                    dialog.visible = false
//                                    break;
//                                }
//                                else {
//                                    csDbus.hold(myHandlerList.get(0).handlerId, true)
//                                    csDbus.answer(dialog.dialogHandId)
//                                    dialog.visible = false
//                                    break;
//                                }
                            }
                        }
                        */
                        csDbus.answer(dialog.dialogHandId)
                        dialog.visible = false
                    }
                }
            }
        }

        Loader {
            id: loader
            z: 3
            sourceComponent: buttonBack
            width: 220
            height: 100
            visible: false
        }
        Loader {
            id: loader2
            z: 3
            sourceComponent: buttonBack
            width: 220
            height: 100
            visible: false
        }
        Loader {
            id: loader6
            z: 3
            sourceComponent: buttonBack
            width: 220
            height: 100
            visible: false
        }

        Component {
            id: buttonBack
            Rectangle {
                color: Qt.rgba(0,0,0,0)
                AnimatedImage {
                    anchors.fill: parent
                    //source: "qrc:/main/images/gif.gif"
                }
            }
        }

        ListModel{
            id: listModel
        }
        Loader {
            id: loaderr
            z: 4
            visible: false
            anchors.bottom: parent.bottom
            sourceComponent: keyboard
            Component.onCompleted: {
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/1-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/1.png","value":"1"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/2-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/2.png","value":"2"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/3-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/3.png","value":"3"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/4-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/4.png","value":"4"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/5-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/5.png","value":"5"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/6-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/6.png","value":"6"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/7-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/7.png","value":"7"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/8-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/8.png","value":"8"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/9-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/9.png","value":"9"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/xing_click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/xing.png","value":"*"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/0-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/0.png","value":"0"})
                listModel.append({"pressadr":"qrc:/keyboard/images/keyboard/jing-click.png",
                                     "numadr":"qrc:/keyboard/images/keyboard/jing.png","value":"#"})
            }
        }

        Component {
            id:keyboard
            Item {
                id: backitem
                width: bIsChange ? 1280 : 720
                height: bIsChange ? 400 : 631
                property alias recta: rect
                Rectangle {
                    id: background
                    //source: "qrc:/keyboard/images/keyboard/num_back.png"
                    width: backitem.width
                    height: backitem.height
                    MouseArea {
                        anchors.fill: parent
                    }
                    Connections {
                        target:gScreenInfo
                        onCurrentOrientationChanged: {
                            if (inputText.text != "") {
                                if (bIsChange) {
                                    backitem.height = 492
                                    //line.anchors.topMargin = 145
                                    inputText.visible = true
                                    key.anchors.top = inputText.bottom
                                } else {
                                    backitem.height = 718
                                    //line.anchors.topMargin = 186
                                }
                                key.anchors.top = inputText.bottom
                            } else {
                                if (bIsChange) {
                                    backitem.height = 400
                                    //line.anchors.topMargin = 145
                                    inputText.visible = true
                                    key.anchors.top = backitem.top
                                } else {
                                    backitem.height = 631
                                    mousearea.visible = false
                                    inputLine.visible = false
                                    inputText.visible = false
                                    key.anchors.top = backitem.top
                                    //line.anchors.top = background.top
                                    //line.anchors.topMargin = 100
                                }
                            }
                        }
                    }
                    Text {
                        id: inputText
                        anchors.left: parent.left
                        anchors.leftMargin: 48
                        width: 562
                        height: 86
                        clip: true
                        font.pixelSize: 50
                        visible: false
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        color: "black"
                        text: ""
                        onTextChanged: {
                            inputText.visible = true
                            mousearea.visible = true
                            inputLine.visible = true
                        }
                    }
                    MouseArea {
                        id: mousearea
                        anchors.left: inputText.right
                        anchors.leftMargin: 19
                        width: 61
                        height: 86
                        visible: false
                        onClicked: {
                            inputText.text = inputText.text.substring(0,inputText.text.length - 1)
                            if (inputText.text == "") {
                                inputText.visible = false
                                mousearea.visible = false
                                inputLine.visible = false
                                backitem.height = bIsChange ? 400 : 631
                                key.anchors.top = parent.top
                                line.anchors.top = parent.top
                                line.anchors.topMargin = 100
                            }
                        }
                        onPressAndHold: {
                            inputText.text = ""
                            inputText.visible = false
                            mousearea.visible = false
                            inputLine.visible = false
                            backitem.height = bIsChange ? 400 : 631
                            key.anchors.top = parent.top
                            line.anchors.top = parent.top
                            line.anchors.topMargin = 100
                        }
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/main/images/icon_delete2.png"
                        }
                    }
                    CLine {
                        id: inputLine
                        anchors.top: inputText.bottom
                        width: 720
                        color: "#121212"
                        visible: false
                    }
                    Column {
                        id: line
                        anchors.top: parent.top
                        anchors.topMargin: 100
                        spacing: 100
                        Repeater {
                            model: 3
                            delegate: twoLine
                        }
                    }
                    Grid {
                        id: key
                        columns: 3
                        rows: 4
                        spacing: 2
                        Repeater {
                            model: listModel
                            CButton {
                                id: num
                                width: 240
                                height: 100
                                backgroundEnabled: false
                                //iconSource: pressed == true ? pressadr : numadr
                                text: value
                                pixelSize: 40
                                textColor: "black"
                                onClicked: {
                                    inputText.text += listModel.get(index).value
                                    backitem.height = bIsChange ? 492 : 718
                                    key.anchors.top = inputText.bottom
                                    line.anchors.topMargin = 186
                                }
                            }
                        }
                    }

//                    Image {
//                        id: numkey
//                        anchors.top: rect.top
//                        width: 70
//                        height: 70
//                        source: "qrc:/main/images/5.png"
//                        MouseArea {
//                            anchors.fill: parent
//                            onClicked: {
//                                loaderr.visible = false
//                            }
//                        }
//                    }
                    Item {
                        id: rect
                        width: 300
                        height: 185
                        anchors.top: bIsChange ? parent.top : key.bottom
                        anchors.topMargin: bIsChange ? 150 : 20
                        anchors.right: parent.right
                        anchors.rightMargin: bIsChange ? 130 : 210
                        anchors.bottom: parent.bottom

                        Image {
                            id: numkey
                            anchors.top: rect.top
                            width: 47
                            height: 47
                            source: "qrc:/main/images/5.png"
                            anchors.horizontalCenter: parent.horizontalCenter
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    loaderr.visible = false
                                }
                            }
                        }
                        Image {
                            id: call
                            anchors.top: numkey.bottom
                            anchors.topMargin: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 130
                            height: 130
                            source: "qrc:/main/images/hangup.png"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    handId = myHandlerList.get(0).handlerId
                                    csDbus.hangup(handId)
                                }
//                                onPressed: {
//                                    call.source = "qrc:/main/images/hangup-click.png"
//                                }
//                                onReleased: {
//                                    call.source = "qrc:/main/images/hangup.png"
//                                }
                            }
                        }
                    }
//                    Image {
//                        id: numkey
//                        anchors.top: key.bottom
//                        anchors.topMargin: 30
//                        width: 70
//                        height: 70
//                        source: "qrc:/keyboard/images/keyboard/numkey.png"
//                        anchors.horizontalCenter: parent.horizontalCenter
//                        MouseArea {
//                            anchors.fill: parent
//                            onClicked: {
//                                loaderr.visible = false
//                            }
//                        }
//                    }
//                    Image {
//                        id: call
//                        anchors.top: numkey.bottom
//                        anchors.topMargin: 16
//                        anchors.horizontalCenter: parent.horizontalCenter
//                        width: 300
//                        height: 99
//                        source: "qrc:/main/images/hangup.png"
//                        MouseArea {
//                            anchors.fill: parent
//                            onClicked: {
//                                handId = myHandlerList.get(0).handlerId
//                                csDbus.hangup(handId)
//                            }
//                        }
//                    }
                }
            }
        }
        Component {
            id: twoLine
            Item {
                width: 720
                height: 2
                CLine {
                    id: line1
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    width: parent.width - 60
                    color: "#cbcbcb"
                }
                CLine {
                    id: line2
                    anchors.top: line1.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    width: parent.width - 60
                    color: "#121212"
                }
            }
        }

        Component.onCompleted: {
            //setHandlerList()
            console.log(qmlState)
        }



    }

}

