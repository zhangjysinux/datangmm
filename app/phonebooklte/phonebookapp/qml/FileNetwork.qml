import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.filemanager.filepicker 1.0

CPage {
    id: personInfoMoreInfo
    default property var userId
    property bool sendEnable: false
    property bool isContacterShare: false
    property int selectenum : -1
    property string thename: ""
    property var myip: ""
    property var parentPage: null
    property var blueToothNum: "00:02:E3:6E:A5:45"

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            personInfoMoreInfo.orientationPolicy = CPageOrientation.LockLandscape
        if (isContacterShare)
            titleBar.titleText = "共享联系人"
        personInfoMoreInfo.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            personInfoMoreInfo.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            personInfoMoreInfo.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Component.onDestruction: {
        getData.fileDisConnect()
    }

    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                personInfoMoreInfo.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
            } else {
                personInfoMoreInfo.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
            }
        }
    }
    Connections {
        target: getData
        onSignalFileConnectFinished: {
            console.log("onSignalFileConnectFinished",status)
            if (status > 0)
                sendEnable = true
        }
        onSignalFileSendPercent: {
            for (var i = 0;i < filelistmodel.count; i++)
            {
                if (filelistmodel.get(i).fileurl == fileName)
                {
                    if (size < maxSize)
                    {
                        var percent = size / maxSize * 100
                        filelistmodel.set(i,{"filePercent":percent})
                    }
                    else if (size == maxSize)
                    {
                        filelistmodel.set(i,{"filePercent":100,"isSend":1})
                        readyTosend()
                    }
                }
            }
        }
        onSignalSendBlueToothInfo: {
            console.log("onSignalSendBlueToothInfo====",address)
            if (blueToothNum == address)
            {
                getData.startPairing(name , address)
            }
        }
        onSignalPairFinished: {
            getData.disconnectDevice()
            console.log("connectDevice",name, address, getData.connectDevice(name, address))
            deleteback.visible = false
            deleteback.opacity = 0
            pageStack.push(filesPickerCom, {"category": 0});
            sendEnable = true
        }
        onSignalPairConfirmation: {
            getData.pairingConfirmation(true)
        }
        onSignalConnectServerFinished: {
            console.log("onSignalConnectServerFinished",address)
        }
        onSignalConnectServerFailed: {
            console.log("onSignalConnectServerFailed")
        }
    }

    function readyTosend()
    {
        for (var i = 0;i < filelistmodel.count; i++)
        {
            if (filelistmodel.get(i).isSend == 0 && sendEnable == true)
            {
                if (selectenum == 0)
                    getData.sendBlueToothFile(filelistmodel.get(i).fileurl)
                else
                    getData.sendFile(filelistmodel.get(i).fileurl)
                return
            }
        }
        pageStack.pop()
    }

    contentAreaItem: Item {
        anchors.fill: parent
        CEditTitleBar{
            id: titleBar
            anchors.top: parent.top
            width: parent.width
            height: 96
            leftItemEnabled: true
            leftItemSize: Qt.size(93,74)
            leftItemBackgroundEnabled: true
            leftItemComponent: Component {
                id: leftTitleBar
                CButton {
                    anchors.fill: parent
                    iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
                    backgroundEnabled: false
                    onClicked: {
                        console.log("back pre page")
                        pageStack.pop()
                    }
                }
            }
            titleText: qsTr("传输文件")
            titlePixelSize: 30
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

        ListModel {
            id: filelistmodel
        }
        ListModel {
            id: netlistmodel
        }
        ListView {
            id: listview
            anchors.top: titleBar.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 100
            //width: parent.width - 148
            anchors.right: parent.right
            anchors.rightMargin: 48
            anchors.bottom: parent.bottom
            clip: true
            model: netlistmodel
            delegate: netbuttonAndLine
            Component.onCompleted: {
                netlistmodel.append({"nameinfo":"蓝牙","opacitynum":"0"})
                netlistmodel.append({"nameinfo":"自组网","opacitynum":"0"})
                netlistmodel.append({"nameinfo":"LTE","opacitynum":"0"})
                netlistmodel.append({"nameinfo":"WIFI","opacitynum":"0"})
            }
        }
        Rectangle {
            id: sureback
            anchors.fill: parent
            visible: false
            color: "black"
            opacity: 0
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
            Image {
                anchors.fill: parent
                source: "qrc:/bluetooth/images/bluetoothblack.png"
            }
            Text {
                id: message
                anchors.top: parent.top
                anchors.topMargin: 52
                height: 80
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: 40
                text: qsTr("是否进入400MHZ-直通脱网对讲模式?")
            }
            CButton {
                id: surecancel
                anchors.top: message.bottom
                anchors.topMargin: 76
                anchors.left: parent.left
                anchors.leftMargin: 48
                width: 174
                height: 100
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/bluetooth/images/bluetooth_ccancel.png" :
                                      "qrc:/bluetooth/images/bluetooth_cancel.png"
                onClicked: {
                    sureback.visible = false
                    suredialog.visible = false
                    sureback.opacity = 0
                    if (netlistmodel.get(selectenum).nameinfo == "PTT") {
                        pageStack.push("qrc:/qml/DigitalTalk.qml",{bIs400MHZ:false,personname:thename})
                    }
                }
            }
            CButton {
                id: surebutton
                anchors.top: message.bottom
                anchors.topMargin: 76
                anchors.left: surecancel.right
                anchors.leftMargin: 48
                width: 174
                height: 100
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/history/images/sureclick.png" :
                                      "qrc:/history/images/suredelete.png"
                onClicked: {
                    sureback.visible = false
                    suredialog.visible = false
                    sureback.opacity = 0
                    if (netlistmodel.get(selectenum).nameinfo == "PTT") {
                        pageStack.push("qrc:/qml/DigitalTalk.qml",{bIs400MHZ:true})
                    }
                }
            }
        }

        Component {
            id: filebuttonAndLine
            Item {
                width: listview.width
                height: 89
                Image {
                    id: fileicon
                    source: "qrc:/main/images/fileicon.png"
                }
                Text {
                    id: filename
                    anchors.left: fileicon.right
                    anchors.leftMargin: 24
                    width: 206
                    height: 40
                    horizontalAlignment: Qt.AlignLeft
                    color: "#ffffff"
                    text: nameinfo
                    font.pixelSize: 40
                    elide: Text.ElideRight
                }
                CProgressBar {
                    id: progress
                    height: 8
//                    width: 244
                    anchors.right: line.right
//                    anchors.rightMargin: 40
                    anchors.left: filename.right
                    anchors.leftMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    value: filePercent
                    minimum: 0
                    maximum: 100
                    anchors.verticalCenterOffset: -10
                    progressColor: "green"
                    backgroundComponent: Item{
                        Image {
                            id: back
                            anchors.fill: parent
                            source: "qrc:/main/images/fileprogress.png"
                        }
                    }
                }
//                CButton {
//                    id: filestart
//                    width: 55;height: 55
//                    anchors.left: progress.right
//                    iconSource: "qrc:/main/images/filestart.png"
//                    backgroundEnabled: false
//                }
//                CButton {
//                    id: filesdelete
//                    width: 55;height: 55
//                    anchors.left: filestart.right
//                    iconSource: "qrc:/main/images/filedelete.png"
//                    backgroundEnabled: false
//                }

                CLine {
                    id: line
                    anchors.top: fileicon.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#9b9b9b"
                }
                Connections{
                    target: gScreenInfo
                    onCurrentOrientationChanged:{
                        if (gScreenInfo.currentOrientation == 2 ||
                                gScreenInfo.currentOrientation == 8) {
                            //                            filename.width = filename.width + 310
                            //                            progress.width = progress.width + 250
                        } else {
                            //                            filename.width = filename.width - 310
                            //                            progress.width = progress.width - 250
                        }
                    }
                }

            }
        }


        Component {
            id: netbuttonAndLine
            Item {
                width: listview.width
                height: 89
                Text {
                    id: name
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    width: 140
                    height: 40
                    horizontalAlignment: Qt.AlignLeft
                    color: "#ffffff"
                    text: nameinfo
                    font.pixelSize: 40
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (selectenum == -1) {
                            selectenum = index
                            netlistmodel.set(selectenum,{"opacitynum":"0.3"})
                        } else {
                            netlistmodel.set(selectenum,{"opacitynum":"0"})
                            selectenum = index
                            netlistmodel.set(selectenum,{"opacitynum":"0.3"})
                        }
                    }
                    onReleased: {
                        if(isContacterShare)
                        {
                            parentPage.contacterShareNetworkSelected(index)
                        }
                        else
                        {
                            if (selectenum == 0) {
                                if (getData.isBlutoothOpened()) {
                                    getData.findblueToothDevice()
                                    deleteback.visible = true
                                    deleteback.opacity = 0.7
                                    deleteback.loadEnabel = true
                                } else {
                                    deleteback.visible = true
                                    deleteback.opacity = 0.7
                                    bluedialog.visible = true
                                    bluedialog.dilogText = "如果使用蓝牙服务，请先开启蓝牙立即开启蓝牙?"
                                    return;
                                }
                            }
                            else {
                                myip = getData.getAddress(userId, name.text)
                                getData.setAddresses(myip)
                                getData.fileConnect()
                                pageStack.push(filesPickerCom, {"category": 0});
                            }
                        }
                    }
                    onClicked: {

                    }
                }
                Rectangle {
                    id: rec
                    width: parent.width
                    height: parent.height
                    opacity: opacitynum
                }
                CLine {
                    id: line
                    anchors.top: name.bottom
                    anchors.topMargin: 24
                    anchors.left: parent.left
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#9b9b9b"
                }

            }
        }
        Component {
            id: filesPickerCom
            SyberosFilesPicker {
                id: filesPicker
                onOk: { //选择完文件后，点击右上角的确定按钮则触发该信号
                    filelistmodel.clear()
                    gScreenInfo.setStatusBar(true)
                    gScreenInfo.setStatusBarStyle("black")
                    var size = 0;
                    var selectedSize = filesPicker.getFileSize() //获取选择文件的大小（返回值为数组，为每一个选中文件的大小列表）
                    for(var i = 0; i < selectedSize.length; i++) {
                        size += parseInt(selectedSize[i]) //获取所选中文件的总大小
                    }
                    for (var j = 0; j < filesPicker.filesPath.length; j++) { //filesPicker.filesPath为选中文件的完整路径（其值为数组，为每一个选中文件的路径的列表）
                        console.log("index----" + j + ", filePath= " + filesPicker.filesPath[j]);
                        filelistmodel.append({"fileurl":filesPicker.filesPath[j],"nameinfo":getData.getFileNmae(filesPicker.filesPath[j]), "isSend":0,"filePercent":0})
                        //getData.sendMessage(3,filesPicker.filesPath[j],getData.getLocalId(),groupid)
                    }
                    if (filesPicker.filesPath.length > 0 )
                    {
                        listview.model = filelistmodel
                        listview.delegate = filebuttonAndLine
                        listview.width = personInfoMoreInfo.width - 64
                        listview.anchors.leftMargin = 32
                        pageStack.pop()
                        readyTosend()
                        //pageStack.push("qrc:/qml/SendFiles.qml")
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
            property alias loadEnabel: loadingicon1.visible
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    deleteback.visible = false
                    deleteback.opacity = 0
                    bluedialog.visible = false
//                    bIsEditDialog = false
//                    bluetoothShare = false
//                    bIsdelete = false
//                    bIs400MHZ = false
                }
            }

            AnimatedImage {
                id: loadingicon1
                anchors.centerIn: parent
                visible: false
                source: "qrc:/main/images/loading.gif"
            }
        }
        Rectangle {
            id: bluedialog
            property alias dilogText: message1.text
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
                visible: false //bluetoothShare
                source: "qrc:/main/images/loading.gif"
            }
            Text {
                id: message1
                anchors.top: parent.top
                anchors.topMargin: 124
                height: 30
                width: parent.width
                wrapMode: Text.Wrap
                lineHeight: 2
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: 30
                text: ""
            }
            CButton {
                id: deletecancel
                anchors.top: message1.bottom
                anchors.topMargin: 76
                anchors.left: parent.left
                anchors.leftMargin: 48
                width: 174
                height: 100
                visible: true
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/bluetooth/images/bluetooth_ccancel.png" :
                                      "qrc:/bluetooth/images/bluetooth_cancel.png"
                onClicked: {
                    deleteback.visible = false
                    bluedialog.visible = false
                    deleteback.opacity = 0
                }
            }
            CButton {
                id: deletesure
                anchors.top: message1.bottom
                anchors.topMargin: 76
                anchors.left: deletecancel.right
                anchors.leftMargin: 48
                width: 174
                height: 100
                visible: true
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/bluetooth/images/bluetooth_copen.png" : "qrc:/bluetooth/images/bluetooth_open.png"
                onClicked: {
//                    deleteback.visible = false
                    bluedialog.visible = false
//                    deleteback.opacity = 0
                    getData.openblueToothdevice()
                    getData.findblueToothDevice()
                    deleteback.loadEnabel = true
                }
            }
        }

    }
}
