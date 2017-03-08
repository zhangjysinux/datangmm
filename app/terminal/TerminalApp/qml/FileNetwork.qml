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

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        personInfoMoreInfo.color = "#293443"
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
    }

    function readyTosend()
    {
        for (var i = 0;i < filelistmodel.count; i++)
        {
            if (filelistmodel.get(i).isSend == 0 && sendEnable == true)
            {
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
                    text: "返回"
                    textColor: "#29afe2"
                    spacing: 0
                    iconSource: "qrc:/main/images/back.png"
                    backgroundEnabled: false
                    onClicked: {
                        pageStack.pop()
                    }
                }
            }
            titleText: qsTr("传输文件")
            titlePixelSize: 30
            titleTextColor: "#29afe2"
            titleAreaCentered: true
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
            anchors.horizontalCenter: parent.horizontalCenter
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
            width: parent.width - 148
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
            width: 504
            height: 296
            color: "#ffffff"
            radius: 10
//            Image {
//                anchors.fill: parent
//                source: "qrc:/bluetooth/images/bluetoothblack.png"
//            }
            Text {
                id: message
                anchors.top: parent.top
                anchors.topMargin: 79
                height: 33
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: "black"
                font.pixelSize: 32
                text: qsTr("是否进入400MHZ-直通脱网对讲模式?")
            }
            CButton {
                id: surecancel
                anchors.top: message.bottom
                anchors.topMargin: 69
                anchors.left: parent.left
                anchors.leftMargin: 62
                width: 150
                height: 67
                backgroundEnabled: true
                backgroundComponent: Rectangle {
                    anchors.fill: parent
                    radius: 10
                    color: "#29afe2"
                }
                text: "取消"
                textColor: "#ffffff"

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
                anchors.topMargin: 69
                anchors.left: surecancel.right
                anchors.leftMargin: 80
                width: 150
                height: 67
                backgroundEnabled: true
                backgroundComponent: Rectangle {
                    anchors.fill: parent
                    radius: 10
                    color: "#29afe2"
                }
                text: "确认"
                textColor: "#ffffff"
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
                    color: "#29afe2"
                    text: nameinfo
                    font.pixelSize: 40
                    elide: Text.ElideRight
                }
                CProgressBar {
                    id: progress
                    height: 12
                    width: 244
                    anchors.left: filename.right
                    anchors.leftMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                    value: filePercent
                    minimum: 0
                    maximum: 100
                    anchors.verticalCenterOffset: -10
                    progressColor: "#29afe2"
                    backgroundComponent: Rectangle {
                        anchors.fill: parent
                        color: Qt.rgba(0,0,0,0)
                        border.width: 1
                        radius: 30
                        border.color: "#29afe2"
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
                    color: "#36465a"
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
                            myip = getData.getAddress(userId, name.text)
                            getData.setAddresses(myip)
                            getData.fileConnect()
                            pageStack.push(filesPickerCom, {"category": 0});
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
                    color: "#36465a"
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

    }
}
