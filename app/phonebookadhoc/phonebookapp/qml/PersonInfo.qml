import QtQuick 2.0
import com.syberos.basewidgets 2.0
CPage {
    id: personInfo
    property alias fromText: fromText.text
    default property var userId
    property string contactertype: "1"
    property var m_name: "value"
    property bool bluetoothShare: false
    property bool networkShare: false
    property bool bIsNormalPerson: false
    property bool bIsdelete: false
    property bool bIs400MHZ: false
    property bool bIsEditDialog: false
    property bool bIschange: false
    property bool bIsInput: false
    property bool bIsUpdate: false
    property bool bIsEdit: false
    property bool bIsFromHistory: false
    property int netnum: 0

    property bool bIsBluetooth: false
    function addNetworkType(network)
    {
        if (bIsNormalPerson)
            listModel.insert(listModel.count - 1,{"textTitle":network})
        else
            listModel.insert(listModel.count - 2,{"textTitle":network})

    }
    function setBluetoothShare(bIsShareing, name, address)
    {
        console.log("startPairing")
        getData.startPairing(name, address)
    }
    function setSource(url)
    {
        addPhoto.item.myIconSource = url
        gScreenInfo.setStatusBar(true)
        gScreenInfo.setStatusBarStyle("black")
    }
    function getputinfo(head , text) {
        for (var i = 0 ;i < listModel.count ; i++)
        {
            if (head == listModel.get(i).textTitle)
            {
                listModel.remove(i,1)
                listModel.insert(i,{"textTitle":head,"value":text})
                break;
            }
        }
    }

    function contacterShareNetworkSelected(selectedLine)
    {
        console.debug("selectedLine: ", selectedLine)

        if (selectedLine == 0){
            if (getData.isBlutoothOpened()) {
                getData.findblueToothDevice()
                pageStack.push("qrc:/qml/bluetooth.qml",{"containerqml":personInfo })
            } else {
                deleteback.visible = true
                deleteback.opacity = 0.7
                bluedialog.visible = true
                bluedialog.dilogText = "如果使用蓝牙服务，请先开启蓝牙立即开启蓝牙?"
                pageStack.pop(personInfo)
            }
        }
        else
        {
            console.debug("start select contacter")

            var netType = 0;

            console.debug("selectedLine: " + selectedLine)
            switch(selectedLine)
            {
            case 1:
                netType = getData.intToNetworkType(0);
                break;
            case 2:
                netType = getData.intToNetworkType(1);
                break;
            case 3:
                netType = getData.intToNetworkType(5);
                break;
            }

            pageStack.push("qrc:/qml/PersonList.qml",{"netType": netType, "parentPage": personInfo})
        }
    }

    function contacterShareContacterSelected(contacterId)
    {
        console.log("start contacter share")

        bluetoothShare = true
        deleteback.visible = true
        deleteback.opacity = 0.7
        bluedialog.visible = true
        bluedialog.dilogText = "连接中...";

        getData.contacterShareStartConnectNetwork(contacterId);
    }

    Connections {
        target: getData
        onSignalSosReturn: {
            var sosstring
            if (sosret == 1)
                sosstring = "应急消息发送成功"
            else
                sosstring = "应急消息发送失败"
            bIsEditDialog = true
            deleteback.visible = true
            deleteback.opacity = 0.7
            bluedialog.visible = true
            bluedialog.dilogText = sosstring
        }

        onSignalPairConfirmation: {
            if (bIsBluetooth) {
                selectDialog.pin = pin
                selectDialog.visible = true
            }
        }

        onSignalConnectServerFinished: {
            if (bIsBluetooth) {
                console.log("connect server finished")
                getData.sendContact(userId)
                console.log("sendcontact..." + userId)
            }
        }

        onSignalConnectServerFailed:
        {
            if (bIsBluetooth) {
                console.log("connect server failed, error code: " + error);
                infoDialog.text = "连接失败";

                console.log("onSignalContactReceived")
                getData.disconnectDevice()
                console.log("disconnect")

                bluetoothShare = false
                deleteback.visible = false
                bluedialog.visible = false

                infoDialog.visible = true;
            }
        }

        onSignalContacterSended:{

            if (bIsBluetooth) {
                console.debug("send status is: " + sendStatus)

                if (sendStatus == 0) {
                    infoDialog.text = "共享联系人成功";
                } else {
                    infoDialog.text = "共享联系人失败";
                }

                console.log("onSignalContactReceived")
                getData.disconnectDevice()
                console.log("disconnect")

                bluetoothShare = false
                deleteback.visible = false
                bluedialog.visible = false

                infoDialog.visible = true;
            }
        }

        onSignalPairFinished: {
            if (bIsBluetooth) {
                if(pairing != 0) //paring success
                {
                    console.log("paring success")

                    bluetoothShare = true
                    deleteback.visible = true
                    deleteback.opacity = 0.7
                    bluedialog.visible = true
                    bluedialog.dilogText = "正在通过蓝牙共享该联系人名片..."

                    getData.disconnectDevice();

                    console.log("onSignalPairFinish")
                    if (getData.connectDevice(name, address)) {
                        console.log("success:connectDevice:"+ name + address)
                    } else {
                        console.log("connect device failed")
                    }
                }
                else
                {
                    console.log("paring failed")
                }
            }
        }

        onSignalContacterShareConnectNetworkFinished:
        {
            if (bIsBluetooth) {
                console.debug("onSignalContacterShareConnectNetworkFinished  " + contactConnectStatus)
                if(contactConnectStatus)
                {
                    getData.contacterShareStartShare(userId)
                }
                else
                {
                    infoDialog.text = "未能连接该联系人";

                    getData.contacterShareDisconnectNetwork();
                    console.log("disconnect")

                    bluetoothShare = false
                    deleteback.visible = false
                    bluedialog.visible = false

                    infoDialog.visible = true;
                }
            }
        }

        onSignalContacterShareSendPercent:
        {
            if (bIsBluetooth) {
                console.debug("onSignalContacterShareSendPercent" + size + "  " + maxSize);
                if(size == maxSize)
                {
                    console.debug("completed: ")

                    console.debug("send status is: ")

                    infoDialog.text = "共享联系人成功";

                    getData.contacterShareDisconnectNetwork();
                    console.log("disconnect")

                    bluetoothShare = false
                    deleteback.visible = false
                    bluedialog.visible = false

                    infoDialog.visible = true;
                }
            }
        }

    }
    Connections {
        target: getData
        onSignalContacterInfo: {
            if (contactertype == "2" ) {
                bIsNormalPerson = true
            }
            else if (contactertype == "1")
            {
                if (!getData.getContacterLeader(userId))
                    titleBar.rightItemEnabled = false
            }

            console.log("onSignalContacterInfo",key,value)
            listModel.append({"textTitle":key,"value":value})
        }
        onSignalSendNameToPersonInfo: {
            myName.text = name
            m_name = name
            if (getData.isFileExists(photoPath)){
                addPhoto.item.myIconSource = photoPath
            }else {
                addPhoto.item.myIconSource = "qrc:/main/images/init_photo.png"
            }
        }
        onSignalSendLocalInfoToPersonInfo: {
            listModel.append({"textTitle":key,"value":value})
        }
    }
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            personInfo.orientationPolicy = CPageOrientation.LockLandscape
        if (fromText.text == "local")
        {
            listView.delegate = myLine
            titleBar.rightItemEnabled = false
        }
        personInfo.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personInfo.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            bIschange = true
        } else {
            personInfo.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            bIschange = false
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personInfo.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                bIschange = true
            } else {
                personInfo.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                bIschange = false
            }
        }
    }

    contentAreaItem: Item {
        anchors.fill: parent

        Rectangle {
            id: infoDialog
            width: 492
            height: 386
            z: 10
            anchors.centerIn: parent
            visible: false

            property alias text: infoText.text

            Image {
                anchors.fill: parent
                source: "qrc:/bluetooth/images/bluetoothblack.png"
            }

            Text {
                id: infoText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 100
                font.pixelSize: 32
                text: "HELLO WORLD"
                color: "white"
            }

            CButton {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                width: 250
                height: 100
                backgroundComponent: Rectangle {
                    color: "black"
                    radius: 30
                    Image {
                        anchors.fill: parent
                        source: "qrc:/main/images/sure.png"
                    }
                }

                onClicked: {
                    infoDialog.visible = false;
                }
            }
        }

        Rectangle {
            id: selectDialog
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width; height: 400
            z: 10
            visible: false
            border.color: "black"

            property string pin: "0000"

            Text {
                anchors.centerIn: parent
                text: "蓝牙配对请求,请确保现实的配对密钥为:" + selectDialog.pin
            }
            CButton {
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                width: 250; height: 100
                text: "是"
                onClicked: {
                    getData.pairingConfirmation(true)
                    console.log("pairingConfirmation")
                    selectDialog.visible = false;
                }
            }
            CButton {
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                width: 250; height: 100
                text: "否"
                onClicked: {
                    getData.pairingConfirmation(false)
                    selectDialog.visible = false;
                }
            }

        }

        Text {
            id: fromText
            text: qsTr("text")
            visible: false
        }

        CEditTitleBar {
            id:titleBar
            anchors.top: parent.top
            height: 96
            width: parent.width
            rightItemEnabled: bIsNormalPerson
            leftItemSize: Qt.size(93,74)
            rightItemSize: Qt.size(74,74)
            rightItemBackgroundEnabled: false
            leftItemComponent: leftTitleBar
            rightItemComponent: rightTitleBar
            titlePixelSize: 30
            titleTextColor: "#f9f9f9"
            titleAreaCentered: true
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
                console.log("fromText.text",fromText.text)
                if (fromText.text == "local"){
                    titleBar.rightItemEnabled = false
                }
            }
        }
        Item {
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
        Loader {
            id: addPhoto
            width: 150
            height: 150
            anchors.top: titleBar.bottom
            anchors.topMargin: 48
            anchors.left: parent.left
            anchors.leftMargin: 48
            source: "qrc:/qml/Photo.qml"
            onLoaded: addPhoto.item.myIconSource
            MouseArea {
                id: mousearea
                visible: false
                anchors.fill: parent
                onClicked: {
                    pageStack.push("qrc:/qml/ChoicePhoto.qml",{parentSource:personInfo})
                }
            }
        }

        Text {
            id: myName
            text: qsTr("text ttt")
            color: "#ffffff"
            font.pixelSize: 40
            anchors.left: addPhoto.right
            anchors.leftMargin: 48
            anchors.top: addPhoto.top
            anchors.topMargin: 55
            width: 300
            elide: Text.ElideRight
        }
        Item {
            id: setMyName
            anchors.top: titleBar.bottom
            anchors.topMargin: 46
            anchors.left: myName.left
            anchors.right: parent.right
            anchors.rightMargin: 48
            visible: false
            Text {
                id: firstName
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 16
                font.pixelSize: 30
                color: "#b6b6b6"
                text: qsTr("姓")
            }
            TextInput {
                id: firstNameEdit
                anchors.left: firstName.right
                anchors.leftMargin: 24
                anchors.right: parent.right
                anchors.top: firstName.top
                clip: true
                validator:RegExpValidator{regExp:/(([\xB0-\xF7][\xA1-\xFE])|([\x81-\xA0][\x40-\xFE])|([\xAA-\xFE][\x40-\xA0])|(\w))+/}
                font.pixelSize: 30
                color: "#ffffff"
                text: myName.text.split(" ")[0]
                onTextChanged: {
                    m_name = firstNameEdit.text + " " + secondNameEdit.text
                }
                onFocusChanged: {
                    if (firstNameEdit.focus) {
                        firstName.visible = false
                    } else {
                        firstName.visible = true
                    }
                }
            }

            CLine {
                id: lineFirst
                anchors.top: firstName.bottom
                anchors.topMargin: 24
                width: parent.width
                color: "#9b9b9b"
            }
            Text {
                id: secondName
                anchors.top: lineFirst.bottom
                anchors.topMargin: 24
                anchors.left: firstName.left
                font.pixelSize: 30
                color: "#b6b6b6"
                text: qsTr("名")

            }
            TextInput {
                id: secondNameEdit
                anchors.left: secondName.right
                anchors.leftMargin: 24
                anchors.right: parent.right
                anchors.top: secondName.top
                font.pixelSize: 30
                clip: true
                validator:RegExpValidator{regExp:/(([\xB0-\xF7][\xA1-\xFE])|([\x81-\xA0][\x40-\xFE])|([\xAA-\xFE][\x40-\xA0])|(\w))+/}
                color: "#ffffff"
                text: myName.text.split(" ")[1]
                onTextChanged: {
                    m_name = firstNameEdit.text + " " + secondNameEdit.text
                }
                onFocusChanged: {
                    if (secondNameEdit.focus) {
                        secondName.visible = false
                    } else {
                        secondName.visible = true
                    }
                }
            }

            CLine {
                id: lineSecond
                anchors.top: secondName.bottom
                anchors.topMargin: 24
                width: parent.width
                color: "#9b9b9b"
            }
        }
        Component {
            id: myLine
            Item {
                id: tempItem
                height: 124
                width: listView.width
                Text {
                    id: lineHead
                    color: "#b6b6b6"
                    text: textTitle
                    font.pixelSize: 30
                    height: 30
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    onTextChanged: {
                        if (text == "自组网" || text == "战互网" || text == "天通网" || text == "LTE" || text == "模块数字对讲" || text == "WIFI")
                        {
                            if (fromText.text == "local" && text != "天通网")
                                loadButtonBox1.sourceComponent = null
                            if (bIsNormalPerson) {
                                lineHead.anchors.topMargin = -100
                            }
                            if (text != "模块数字对讲")
                                network.visible = true
                            if (fromText.text == "local") {
                                if (getData.getCurrentNetworkIsOnLine(getData.getLocalId(),text)) {
                                    network.source =  "qrc:/main/images/online.png"
                                } else {
                                    network.source =  "qrc:/main/images/missline.png"
                                }
                            } else {
                                if (getData.getCurrentNetworkIsOnLine(userId,text)) {
                                    network.source =  "qrc:/main/images/online.png"
                                } else {
                                    network.source =  "qrc:/main/images/missline.png"
                                }
                            }
                        } else if (bIsNormalPerson && text == "编号") {
                            lineHead.visible = false
                            lineValue.visible = false
                            line.visible = false
                            loadButtonBox1.sourceComponent = null
                        }
                        else if (text == "设备序列号")
                        {
                            if (bIsNormalPerson) {
                                lineHead.visible = false
                                lineValue.visible = false
                                line.visible = false
                            } else {
                                lineValue.width = 400
                            }
                            loadButtonBox1.sourceComponent = null
                            if (fromText.text != "local") {
                                buttonline.anchors.top = line.bottom
                                buttonline.visible = true
                                tempItem.height = 472
                            }
                        }
                        else if (text == "textTitle")
                        {
                            lineHead.visible = false
                            lineValue.visible = false
                            line.visible = false
                            loadButtonBox1.sourceComponent = null
                            //if (bIsNormalPerson) {
                                tempItem.height = 372
                                buttonline.anchors.top = parent.top
                                buttonline.anchors.topMargin = -124
                                buttonline.visible = true
                            //}
                        }
                        else
                        {
                            loadButtonBox1.sourceComponent = null
                        }
                    }
                }
                Image {
                    id: network
                    anchors.top: lineHead.top
                    anchors.topMargin: 4
                    anchors.left: lineHead.right
                    anchors.leftMargin: 16
                    visible: false
                    source:  ""
                }
                Text {
                    id: lineValue
                    text: value
                    color: "#ffffff"
                    font.pixelSize: 30
                    anchors.top: lineHead.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    width: 300
                    height: 30
                    elide: Text.ElideRight
                }
                CLine {
                    id: line
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    width: parent.width - 96
                    anchors.top: lineValue.bottom
                    anchors.topMargin: 24
                    color: "#9b9b9b"
                }
                Loader {
                    id: loadButtonBox1
                    anchors.top: lineHead.top
                    anchors.topMargin: -24
                    anchors.right: line.right
                    anchors.rightMargin: 24
                    sourceComponent: buttonBox
                    onLoaded: {
                        loadButtonBox1.item.myip = lineValue.text
                        if (lineHead.text == "模块数字对讲") {
                            loadButtonBox1.item.dialicon = "qrc:/group/images/PTT.png"
                            loadButtonBox1.item.thirdEnable = false
                        }
                        else if (lineHead.text == "自组网" || lineHead.text == "LTE" || lineHead.text == "WIFI")
                        {
                            loadButtonBox1.item.thirdIcon = "qrc:/main/images/makevideo.png"
                        }
                        else
                        {
                            loadButtonBox1.item.thirdEnable = false
                        }
                        if (lineHead.text == "自组网" || lineHead.text == "战互网" || lineHead.text == "LTE" || lineHead.text == "WIFI")
                        {
                            loadButtonBox1.item.fourthEnable = true
                        }
                        console.log("11111111111111111",lineHead.text,personInfo.fromText)
                        if (lineHead.text == "天通网" && personInfo.fromText == "local")
                        {
                            loadButtonBox1.item.fifthEnable = true
                            loadButtonBox1.item.first = false
                            loadButtonBox1.item.second = false
                        }

                        loadButtonBox1.item.index = index
                    }
                }
                Column {
                    id: buttonline
                    visible: false
                    Repeater {
                        //"自组网共享联系人","LTE共享联系人","战互网共享联系人", "WIFI共享联系人"
                        model: ["通话记录","状态查询","共享联系人","传输文件"]
                        delegate: Component {
                            Item {
                                width: listView.width
                                height: 79
                                CButton {
                                    id: thebutton
                                    backgroundEnabled: false
                                    anchors.top: parent.top
                                    anchors.topMargin: 24
                                    anchors.left: parent.left
                                    anchors.leftMargin: 36
                                    text: modelData
                                    height: 30
                                    pixelSize: 30
                                    textColor: "#31bd2c"
                                    onClicked: {
                                        if (modelData == "通话记录" && !bIsFromHistory) {
                                            pageStack.push("qrc:/qml/CallRecordInfo.qml",{"personName":myName.text,"personPost":listModel.get(0).value,
                                                               "usrid":userId,"type":contactertype,"bIsFromPerson":true})
                                            getData.getCallHistoryBytarget(userId,false)
                                        } else if (modelData == "状态查询") {
                                            pageStack.push("qrc:/qml/PersonInfoSearch.qml")
                                        } else if (modelData == "传输文件"){
                                            bIsBluetooth = false
                                            pageStack.push("qrc:/qml/FileNetwork.qml",{"userId":userId})
                                        }else if (modelData == "共享联系人"){
                                            bIsBluetooth = true
                                            pageStack.push("qrc:/qml/FileNetwork.qml",{"userId":userId, "isContacterShare":true, "parentPage": personInfo})
                                        }
                                    }
                                }
                                CLine {
                                    id: line1
                                    anchors.top: thebutton.bottom
                                    anchors.topMargin: 24
                                    anchors.left: parent.left
                                    anchors.leftMargin: 48
                                    width: parent.width - 96
                                    color: "#9b9b9b"
                                }
                            }
                        }
                    }
                }
            }
        }
        Component {
            id: deleteItem
            Item {
                width: listView.width
                height: 79
                CButton {
                    id: deleteContacter
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 88
                    backgroundEnabled: false
                    text: "删除联系人"
                    pixelSize: 30
                    height: 30
                    textColor: "#b61f18"
                    onClicked: {
                        bIsdelete = true
                        deleteback.visible = true
                        deleteback.opacity = 0.7
                        bluedialog.visible = true
                        bluedialog.dilogText = "是否删除此联系人?"
                    }
                }
                CLine {
                    id: lineState
                    anchors.top: deleteContacter.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 96
                    width: parent.width - 144
                    color: "#9b9b9b"
                }
            }
        }
        Component {
            id: myLineEdit
            Item {
                id: tempItem
                height: 124
                width: listView.width
                Text {
                    id: lineHead
                    color: "#b6b6b6"
                    text: textTitle
                    font.pixelSize: 30
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 96
                    onTextChanged: {
                        if (text == "自组网" || text == "战互网" || text == "天通网" || text == "LTE"
                                || text == "模块数字对讲"  || text == "未知网络" || text == "WIFI")
                        {
                            if(bIsNormalPerson) {
                                lineHead.anchors.topMargin = -100
                            }
                            editDelete.visible = true
                        }
                        else if (text == "编号")
                        {
                            if (bIsNormalPerson) {
                                lineHead.visible = false
                                lineValue.visible = false
                                line.visible = false
                            } else {
                                lineValue.readOnly = true
                                numLock.visible = true
                            }
                        }
                        else if (text == "添加"){
                            if(bIsNormalPerson) {
                                lineHead.anchors.topMargin = -100
                            }
                            editAdd.visible = true
                            lineValue.visible = false
                            line.anchors.top = lineHead.bottom
                            deleteperson.visible = true
                            tempItem.height = 155
                        } else if (text == "设备序列号") {
                            //if(bIsNormalPerson) {
                                lineHead.visible = false
                                lineValue.visible = false
                                line.visible = false
                            //}
                        }
                        if (text == "天通网" || text == "模块数字对讲")
                            lineValueNum.visible = true;
                        else if (text == "自组网" || text == "LTE" || text == "WIFI" || text == "战互网")
                            lineValueIp.visible = true
                        else if (text == "职位")
                            lineValue.visible = true
                    }
                }
                TextInput {
                    id: lineValue
                    visible: false
                    text: value
                    color: "#ffffff"
                    font.pixelSize: 30
                    anchors.top: lineHead.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 96
                    width: listView.width - 144
                    verticalAlignment: Qt.AlignVCenter
                    clip: true
                    onTextChanged: {
                        if (bIsInput || index == 6) {
                            if (bIsUpdate) {
                                lineValue.text = keyboardground.inputinfo.text
                            } else {
                                keyboardground.inputinfo.text = lineValue.text
                            }
                        }
                        if (!bIsUpdate) {
                            if (listModel.get(index).textTitle == lineHead.text ){
                                listModel.set(index,{"value":lineValue.text})
                            }
                        }
                    }
                    onFocusChanged: {
                        if (bIschange || (!bIschange && index > 2)) {
                            bIsInput = !bIsInput
                            if (bIsInput) {
                                if (index == 6) {
                                    listView.positionViewAtIndex(6,listView.Center)
                                }
                                keyboardground.visible = true
                                keyboardground.inputinfo.cursorVisible = true
                                keyboardground.inputinfo.text = lineValue.text
                                keyboardground.inputinfo.myindex = lineHead.text
                            } else {
                                bIsUpdate = false
                                keyboardground.visible = false
                                closeSoftKeyboard()
                            }
                        }
                    }
                }
                TextInput {
                    id: lineValueNum
                    validator: RegExpValidator{regExp: /^[0-9]{11}$/}
                    visible: false
                    text: value
                    color: "#ffffff"
                    font.pixelSize: 30
                    anchors.top: lineHead.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 96
                    width: listView.width - 144
                    verticalAlignment: Qt.AlignVCenter
                    clip: true
                    onTextChanged: {
                        if (bIsInput || index == 6) {
                            if (bIsUpdate) {
                                lineValueNum.text = keyboardground.inputinfo.text
                            } else {
                                keyboardground.inputinfo.text = lineValueNum.text
                            }
                        }
                        if (!bIsUpdate) {
                            if (listModel.get(index).textTitle == lineHead.text ){
                                listModel.set(index,{"value":lineValueNum.text})
                            }
                        }
                    }
                    onFocusChanged: {
                        if (bIschange || (!bIschange && index > 2)) {
                            bIsInput = !bIsInput
                            if (bIsInput) {
                                if (index == 6) {
                                    listView.positionViewAtIndex(6,listView.Center)
                                }
                                keyboardground.visible = true
                                keyboardground.inputinfo.cursorVisible = true
                                keyboardground.inputinfo.text = lineValueNum.text
                                keyboardground.inputinfo.myindex = lineHead.text
                            } else {
                                bIsUpdate = false
                                keyboardground.visible = false
                                closeSoftKeyboard()
                            }
                        }
                    }
                }
                TextInput {
                    id: lineValueIp
//                    validator: RegExpValidator{regExp: /[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}/}
                    validator: RegExpValidator{regExp: /((?:(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d))))/}

                    visible: false
                    text: value
                    color: "#ffffff"
                    font.pixelSize: 30
                    anchors.top: lineHead.bottom
                    anchors.topMargin: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 96
                    width: listView.width - 144
                    verticalAlignment: Qt.AlignVCenter
                    clip: true
                    onTextChanged: {
                        if (bIsInput || index == 6) {
                            if (bIsUpdate) {
                                lineValueIp.text = keyboardground.inputinfo.text
                            } else {
                                keyboardground.inputinfo.text = lineValueIp.text
                            }
                        }
                        if (!bIsUpdate) {
                            if (listModel.get(index).textTitle == lineHead.text ){
                                listModel.set(index,{"value":lineValueIp.text})
                            }
                        }
                    }
                    onFocusChanged: {
                        if (bIschange || (!bIschange && index > 2)) {
                            bIsInput = !bIsInput
                            if (bIsInput) {
                                if (index == 6) {
                                    listView.positionViewAtIndex(6,listView.Center)
                                }
                                keyboardground.visible = true
                                keyboardground.inputinfo.cursorVisible = true
                                keyboardground.inputinfo.text = lineValueIp.text
                                keyboardground.inputinfo.myindex = lineHead.text
                            } else {
                                bIsUpdate = false
                                keyboardground.visible = false
                                closeSoftKeyboard()
                            }
                        }
                    }
                }
                CLine {
                    id: line
                    anchors.left: parent.left
                    anchors.leftMargin: 96
                    width: listView.width - 144
                    anchors.top: lineValue.bottom
                    anchors.topMargin: 24
                    color: "#9b9b9b"
                }
                Image {
                    id: numLock
                    width: 40
                    height: 50
                    source: "qrc:/main/images/num_lock.png"
                    anchors.right: line.right
                    anchors.rightMargin: 24
                    anchors.verticalCenter: parent.verticalCenter
                    visible: false
                }
                CButton {
                    id: editDelete
                    width: 50
                    height: 50
                    iconSource: "qrc:/main/images/edit_delete.png"
                    backgroundEnabled: false
                    visible: false
                    anchors.right: line.left
                    anchors.rightMargin: 24
                    anchors.top: lineHead.top
                    onClicked: {
                        listModel.remove(index,1)
                    }
                }
                CButton {
                    id: editAdd
                    width: 50
                    height: 50
                    iconSource: "qrc:/main/images/edit_add.png"
                    backgroundEnabled: false
                    visible: false
                    anchors.top: lineHead.top
                    anchors.right: line.left
                    anchors.rightMargin: 24
                    onClicked: {
                        pageStack.push("qrc:/qml/PersonLable.qml",{"containerqml":personInfo,})
                        for (var i = 1; i < listModel.count - 1; i++) {
                            getData.getCurrentNetworkInfo(listModel.get(i).textTitle)
                        }
                    }
                }
                Loader {
                    id: deleteperson
                    visible: false
                    anchors.top: line.bottom
                    anchors.left: parent.left
                    sourceComponent: deleteItem
                }
            }
        }

        ListModel {
            id: listModel
        }
        ListView {
            id: listView
            anchors.top: addPhoto.bottom
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width
            delegate: myLine
            model: listModel
            clip: true
            cacheBuffer: listView.height * 4
            Component.onCompleted: {
                if ((fromText.text == "network") || (fromText.text == "group")){
                    getData.getContacter(userId)
                    if (bIsNormalPerson)
                        listModel.append({"textTitle":"textTitle","value":""})
                } else {
                    getData.getContacter("-1")
                }
            }
            onContentYChanged: {
                bIsInput = false
                keyboardground.visible = false
                closeSoftKeyboard()
            }
        }

        Rectangle {
            id: keyboardground
            property alias inputinfo: input
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
            MouseArea {
                anchors.fill: parent
                onClicked: {
                }
            }
            TextInput {
                id: input
                property string myindex: ""
                anchors.left: parent.left
                anchors.leftMargin: 7
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Qt.AlignLeft
                clip: true
                font.pixelSize: 45
                color: "white"
                text: ""
                onTextChanged: {
                    if (input.focus) {
                        getputinfo(input.myindex , input.text)
                    }
                }
                onFocusChanged: {
                    if (input.focus) {
                        bIsUpdate = true
                    } else {
                        bIsInput = false
                        bIsUpdate = false
                        keyboardground.visible = false
                        closeSoftKeyboard()
                    }
                }
            }
        }
    }
    Rectangle {
        id: deleteback
        visible: false
        anchors.fill: parent
        color: "black"
        z: 10
        opacity: 0
        MouseArea {
            anchors.fill: parent
            onClicked: {
                deleteback.visible = false
                deleteback.opacity = 0
                bluedialog.visible = false
                bIsEditDialog = false
                bluetoothShare = false
                bIsdelete = false
                bIs400MHZ = false
            }
        }
    }
    Rectangle {
        id: bluedialog
        property alias dilogText: message.text
        property var digitalNum: ""
        visible: false
        anchors.centerIn: parent
        width: 492
        height: 386
        z: 10
        Image {
            anchors.fill: parent
            source: "qrc:/bluetooth/images/bluetoothblack.png"
        }
        AnimatedImage {
            id: loadingicon
            anchors.top: parent.top
            anchors.topMargin: 42
            anchors.left: parent.left
            anchors.leftMargin: 175
            horizontalAlignment: Qt.AlignHCenter
            visible: bluetoothShare
            source: "qrc:/main/images/loading.gif"
        }
        Text {
            id: message
            anchors.top: bluetoothShare ? loadingicon.bottom : parent.top
            anchors.topMargin: bluetoothShare ? 24 : 52
            height: bluetoothShare ? 30 : 80
            width: parent.width
            wrapMode: Text.Wrap
            lineHeight: 2
            horizontalAlignment: Qt.AlignHCenter
            color: "white"
            font.pixelSize: bluetoothShare ? 30 : 40
            text: ""
        }
        CButton {
            id: deletecancel
            anchors.top: message.bottom
            anchors.topMargin: bluetoothShare ? 24 : 76
            anchors.left: parent.left
            anchors.leftMargin: bluetoothShare ? 159 : 48
            width: 174
            height: 100
            visible: bIsEditDialog ? false : true
            backgroundEnabled: false
            iconSource: pressed ? "qrc:/bluetooth/images/bluetooth_ccancel.png" :
                                  "qrc:/bluetooth/images/bluetooth_cancel.png"
            onClicked: {
                deleteback.visible = false
                bluedialog.visible = false
                bluetoothShare = false
                bIsdelete = false
                bIs400MHZ = false
                deleteback.opacity = 0
            }
        }
        CButton {
            id: deletesure
            anchors.top: message.bottom
            anchors.topMargin: 76
            anchors.left: bIsEditDialog ? parent.left : deletecancel.right
            anchors.leftMargin: bIsEditDialog ? 160 : 48
            width: 174
            height: 100
            visible: bIsdelete ? true : (bluetoothShare ? false : true)
            backgroundEnabled: false
            iconSource: pressed ? ((bIsdelete || bIsEditDialog || bIs400MHZ) ? "qrc:/history/images/sureclick.png" : "qrc:/bluetooth/images/bluetooth_copen.png") :
                                  ((bIsdelete || bIsEditDialog || bIs400MHZ) ? "qrc:/history/images/suredelete.png": "qrc:/bluetooth/images/bluetooth_open.png")
            onClicked: {
                deleteback.visible = false
                bluedialog.visible = false
                deleteback.opacity = 0
                if (bIsdelete) {
                    if (message.text == "是否删除此联系人?") {
                        getData.removeContacter(listModel.get(1).value)
                        pageStack.pop()
                    } else {
                        pageStack.pop()
                    }
                } else if (bIs400MHZ) {
                    getData.baoTongDial(bluedialog.digitalNum)
                    bIs400MHZ = false
                    pageStack.push("qrc:/qml/DigitalTalk.qml",{"bIsGroup":false,personname:m_name})
                } else if (bIsEditDialog) {
                    bIsEditDialog = false
                } else {
                    getData.openblueToothdevice()
                    getData.findblueToothDevice()
                    pageStack.push("qrc:/qml/bluetooth.qml",{"containerqml":personInfo})
                }
            }
        }
    }
    Component {
        id: leftTitleBar
        CButton {
            anchors.fill: parent
            iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
            backgroundEnabled: false
            onClicked: {
                closeSoftKeyboard()
                if (listView.delegate == myLineEdit) {
                    bIsdelete = true
                    deleteback.visible = true
                    deleteback.opacity = 0.7
                    bluedialog.visible = true
                    bluedialog.dilogText = "是否退出当前编辑页面?"
                } else {
                    pageStack.pop()
                }
            }
        }
    }

    Component {
        id: rightTitleBar
        CButton {
            id: rightText
            anchors.fill: parent
            pixelSize: 30
            textColor: "white"
            backgroundEnabled: false
            text: qsTr("编辑")
            iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
            spacing: -68
            onClicked: {
                closeSoftKeyboard()
                if (rightText.text == "编辑")
                {
                    listView.delegate = myLineEdit
                    rightText.text = "保存"
                    if (!bIsNormalPerson)
                        listModel.insert(listModel.count-1,{"textTitle":"添加","value":"xxxx"})
                    else {
                        listModel.remove(listModel.count-1,1)
                        listModel.append({"textTitle":"添加","value":"xxxx"})
                    }

                    setMyName.visible = true
                    myName.visible = false
                    mousearea.visible = true
                    bIsEdit = true
                } else {
                    var tipsText = ""
                    if (firstNameEdit.text == "")
                    {
                        tipsText = "请输入联系人姓"
                        bIsEditDialog = true
                    }
                    else if (secondNameEdit.text == "")
                    {
                        tipsText = "请输入联系人名"
                        bIsEditDialog = true
                    }

                    for (var i = 2; i < listModel.count - 1; i++) {
                        if (listModel.get(i).value == "" || listModel.get(i).value == null) {
                            if (tipsText == "")
                                tipsText = "请输入联系人" + listModel.get(i).textTitle + "号码"
                            bIsEditDialog = true
                            break
                        }
                    }
                    if (bIsEditDialog) {
                        deleteback.visible = true
                        deleteback.opacity = 0.7
                        bluedialog.visible = true
                        bluedialog.dilogText = tipsText
                    } else {
                        if (bIsInput) {
                            bIsInput = false
                            keyboardground.visible = false
                            keyboardground.inputinfo.text = ""
                        }
                        for (var i = 0;i< listModel.count;i++){
                            if (listModel.get(i).textTitle == "添加")
                                listModel.remove(i,1)
                        }
                        for (var i = 2;i<listModel.count;i++){
                            if (listModel.get(i).textTitle != "设备序列号")
                                getData.setPhoneList(listModel.get(i).textTitle,listModel.get(i).value)
                        }
                        myName.text = m_name;
                        if (fromText.text == "local") {
                            getData.updateLocalInfo(myName.text,listModel.get(0).value,listModel.get(1).value,
                                                    null,null)
                        }  else {
                            getData.updateContacter(myName.text,listModel.get(0).value,listModel.get(1).value,addPhoto.item.myIconSource)
                        }
                        listView.delegate = myLine
                        if (bIsNormalPerson)
                            listModel.append({"textTitle":"textTitle","value":""})
                        rightText.text = "编辑"
                        setMyName.visible = false
                        myName.visible = true
                        mousearea.visible = false
                    }
                }
            }
        }
    }

    Component {
        id: buttonBox
        Item {
            property int index: -1
            property alias dialicon: dial.iconSource
            property var myip: ""
            property alias first: mess.visible
            property alias second: dial.visible
            property alias thirdIcon : more.iconSource
            property alias thirdEnable: more.visible
            property alias fourthEnable: ptt.visible
            property alias fifthEnable: sos.visible
            width: 360
            height: 124
            CButton{
                id: mess
                width: 72
                height: 72
                anchors.verticalCenter: parent.verticalCenter
                iconSource: pressed ? "qrc:/main/images/message_click.png"
                                    : "qrc:/main/images/message.png"
                backgroundEnabled: false
                onClicked: {
                    var url = "sms:" + listModel.get(index).value + "?body=&backout=true"
                    qApp.openUrl(url)
                }
            }
            CButton{
                id: dial
                width: 72
                height: 72
                anchors.left: mess.right
                anchors.leftMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "qrc:/main/images/dial.png"
                backgroundEnabled: false
                onClicked: {
                    if (timecount.running == true)
                        return;
                    timecount.running = true
                    if (listModel.get(index).textTitle == "模块数字对讲") {
                        bIs400MHZ = true;
                        deleteback.visible = true
                        deleteback.opacity = 0.7
                        bluedialog.visible = true
                        bluedialog.dilogText = "是否进入400MHZ-直通脱网对讲模式?"
                        bluedialog.digitalNum = listModel.get(index).value
                    } else {
                        var url = "sinuxtel:makeCall?net=" + listModel.get(index).textTitle + "&value=" + listModel.get(index).value
                        qApp.openUrl(url)
                    }
                }
            }
            CButton{
                id: more
                width: 72
                height: 72
                visible: (dial.iconSource == "qrc:/group/images/PTT.png") ? false : true
                anchors.left: dial.right
                anchors.leftMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "qrc:/main/images/more.png"
                backgroundEnabled: false
                onClicked: {
                    if (listModel.get(index).textTitle == "自组网" || listModel.get(index).textTitle == "LTE" || listModel.get(index).textTitle == "WIFI") {
                        qApp.openUrl("sinuxtel:makeVideoCall?net=" + listModel.get(index).textTitle + "&phoneNum="+listModel.get(index).value)
                    }
                    else
                        pageStack.push("qrc:/qml/PersonInfoMoreInfo.qml",{thename:m_name,"myip":myip})
                }
            }
            CButton{
                id: ptt
                width: 72
                height: 72
                visible: false
                anchors.left: more.visible ? more.right : dial.right
                anchors.leftMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "qrc:/group/images/PTT.png"
                backgroundEnabled: false
                onClicked: {
                    var net = 0
                    if (listModel.get(index).textTitle == "自组网")
                        net = 1
                    else if (listModel.get(index).textTitle == "LTE")
                        net = 16
                    else if (listModel.get(index).textTitle == "战互网")
                        net = 65536
                    else if (listModel.get(index).textTitle == "WIFI")
                        net = 1048576
                    pageStack.push("qrc:/qml/PptChatRoom.qml",{"groupid":"p2pgroup_" + personInfo.userId + "_networktype_"+net,
                                       "groupname":"","bIsPtt":false,
                                       "groupnum":"",
                                       "netinfo":listModel.get(index).textTitle,
                                       "prepage":personInfo,"bIsP2P":true})
                }
            }
            CButton{
                id: sos
                width: 72
                height: 72
                visible: false
                anchors.verticalCenter: parent.verticalCenter
                iconSource: pressed ? "qrc:/images/sos-click.png" : "qrc:/images/sos.png"
                backgroundEnabled: false
                onClicked: {
                    getData.onSetPropertyType()
                }
            }
        }
    }
    Loader {
        id:dialog
    }
    Timer {
        id: timecount
        interval: 1000
        repeat: false
        onTriggered: {
        }
    }
}
