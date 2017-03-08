import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: callRecord
    property string calledPhone: "qrc:/history/images/called.png"
    property string dialedPhone: "qrc:/history/images/dialed.png"
    property string uncallPhone: "qrc:/history/images/unCall.png"
    property int calltype: 0
    property string inputinfo: ""
    property bool bIsFirst: true/*是否第一次*/
    property bool bIsChange: false/*是否横屏*/
    property bool bIsUpdate: false/*是否更新*/
    property bool bIsKeyClick: false/*键盘是否点击*/
    property bool bIsSlip: false/*是否滑动*/
    property bool bIsPush: false/*是否进入详情*/
    function historyPersonDelte(bIsDelete)
    {
        loader.item.clearLoader()
        listview.editing = bIsDelete
        if (bIsDelete) {
            listview.deselect()
            loader.visible = false
            toolBar.visible = false
            numkey2.visible = false
        } else {
            deleteBar.visible = false
            loader.visible = true
            toolBar.visible = bIsChange
            numkey2.visible = !bIsChange
        }
    }
    Connections {
        target: getData
        onSignalSendDeleteHistoryInfo: {
            if (bIsPush) {
                bIsPush = false
                historyTable.removeTheCallHistory(id,historyTable.count())
            }
        }
        onSignalCallHistoryChanged: {
            getData.getSearchCallHistory(calltype,inputinfo)
        }
    }

    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        callRecord.color = "#293443"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            toolBar.direction = true
            bIsChange = true
            callRecord.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
        } else {
            toolBar.direction = false
            bIsChange = false
            callRecord.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                bIsChange = true
                callRecord.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                if (listview.editing) {
                    loader.visible = false
                    toolBar.visible = false
                    listview.width = parent.width
                } else {
                    toolBar.visible = true
                    toolBar.direction = true
                    loader.visible = true
                }
                numkey2.visible = false
            } else {
                toolBar.direction = false
                bIsChange = false
                callRecord.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                if (listview.editing) {
                    loader.visible = false
                    listview.width = parent.width
                }
            }
        }
    }
    contentAreaItem: Item{
        CEditTitleBar {
            id:titleBar
            anchors.top: parent.top
            height: 96
            width: parent.width
            leftItemEnabled: false
            rightItemEnabled: true
            leftItemComponent: Component {
                id: leftTitle
                CButton {
                    property bool checked: false
                    anchors.fill: parent
                    backgroundEnabled: false
                    iconSource: "qrc:/main/images/icon_set.png"
                    spacing: -68
                    onClicked: {
                    }
                }
            }
            leftItemSize: Qt.size(74,74)
            rightItemSize: Qt.size(74,74)
            rightItemComponent: Component {
                id: rightTitle
                CButton {
                    property bool checked: false
                    anchors.fill: parent
                    pixelSize: 30
                    textColor: "#29afe2"
                    backgroundEnabled: false
                    text: qsTr("编辑")
                    iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                    spacing: -68
                    onClicked: {
                        listview.deselect()
                        historyPersonDelte(true)
                    }
                }
            }
            editView: listview
            editLeftItemLeftMargin: 2
            editLeftComponent: CButton {
                backgroundEnabled: false
                iconSource: "qrc:/main/images/back.png"
                text: "返回"
                spacing: 0
                onClicked: {
                    listview.deselect()
                    historyPersonDelte(false)
                }
            }
            editRightComponent: CButton {
                id: rightButton
                property bool checked: false
                anchors.fill: parent
                pixelSize: 30
                textColor: "#29afe2"
                backgroundEnabled: false
                text: (listview.selectedCount == listview.count) ? "取消" : "全选"
                iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
                spacing: -68
                onClicked: {
                    if (rightButton.text == "全选"){
                        listview.selectAll()
                    } else {
                        listview.deselect()
                    }
                }
            }
            titleAreaCentered: true
            titleItemComponent: title
            titleItemEnabled: true
            titleTextColor: "#29afe2"
            editTitleTextColor: "#29afe2"
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
            anchors.fill: parent
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
            id: listmodel
        }

        CEditListView {
            id: listview
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.bottom: bIsChange ? toolBar.top : numkey2.top
            width: bIsChange ? (listview.editing ? parent.width : parent.width - 560) : parent.width
            model: historyTable
            clip: true
            delegate: CEditListViewDelegate {
                id: listText
                height: 164
                width: listview.width
                editLabelRightMargin: bIsChange ? listview.width - 74 : 646
                editLabelSize: Qt.size(50,50)
                editLabelComponent: Image {
                    id:checkImage
                    width: 50
                    height: 50
                    anchors.centerIn: parent
                    source: listText.checked ? "qrc:/main/images/checkTrue.png" : "qrc:/main/images/checkFalse.png"
                }
                Item {
                    height: parent.height
                    width: parent.width
                    Image {
                        id: callflag
                        anchors.left: parent.left
                        anchors.leftMargin: 16
                        anchors.top: parent.top
                        anchors.topMargin: 47
                        width: 70
                        height: 70
                        source: ""
                        visible: listview.editing ? false : true
                    }
                    Text {
                        id: personName
                        anchors.top: parent.top
                        anchors.topMargin: 14
                        anchors.left: callflag.right
                        anchors.leftMargin: 16
                        width: 200
                        height: 42
                        clip: true
                        elide: Text.ElideRight
                        text: theName
                        color: "#ffffff"
                        font.pixelSize: 38
                    }
                    Text {
                        id: position
                        anchors.top: personName.bottom
                        anchors.topMargin: 16
                        anchors.left: callflag.right
                        anchors.leftMargin: 16
                        clip: true
                        width: 160
                        height: 30
                        text: info
                        color: "#ffffff"
                        font.pixelSize: 30
                    }
                    Text {
                        id: network
                        anchors.top: position.bottom
                        anchors.topMargin: 16
                        anchors.left: callflag.right
                        anchors.leftMargin: 16
                        width: 110
                        clip: true
                        text: netinfo
                        color: "#ffffff"
                        font.pixelSize: 30
                    }
                    Text {
                        id: phonenum
                        anchors.top: position.bottom
                        anchors.topMargin: 16
                        anchors.left: network.right
                        anchors.leftMargin: 30
                        horizontalAlignment: Qt.AlignLeft
                        width: 220
                        height: 30
                        clip: true
                        text: num
                        color: "#ffffff"
                        font.pixelSize: 30
                        elide: Text.ElideRight
                    }
                    MouseArea {
                        anchors.right: more.left
                        anchors.rightMargin: 50
                        anchors.left: parent.left
                        height: parent.height
                        visible: listview.editing ? false : true
                        onPressed: {
                            clickeffects.opacity = 0.3
                        }
                        onReleased: {
                            clickeffects.opacity = 0
                            var url
                            if (getData.getCallHistoryNet(historyTable.getCallHistoryValue(index,"id")) == 7){

                                url = "sinuxtel:makeVideoCall?phoneNum="+num
                            }
                            else
                                url = "sinuxtel:makeCall?net="+ netinfo +"&value=" + num
                            qApp.openUrl(url)
                        }
                        onPositionChanged: {
                            clickeffects.opacity = 0
                        }
                    }
                    Text {
                        id: time
                        anchors.right: parent.right
                        anchors.rightMargin: 87
                        width: 320
                        height: 164
                        horizontalAlignment: Qt.AlignRight
                        verticalAlignment: Qt.AlignVCenter
                        text: timeinfo
                        clip: true
                        color: "#ffffff"
                        font.pixelSize: 30
                        onTextChanged: {
                            var iconflag = historyTable.getCallHistoryValue(index,"iconValue")
                            if (iconflag == 1) {
                                callflag.source = calledPhone
                            } else if (iconflag == 2) {
                                callflag.source = dialedPhone
                            } else if (iconflag == 3) {
                                callflag.source = uncallPhone
                                personName.color = "#ff4351"
                            }
                        }
                    }
                    MouseArea {
                        anchors.top: parent.top
                        anchors.topMargin: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        width: 72
                        height: 72
                        visible: (listview.editing || iconValue == 0) ? true : false
                    }
                    Image {
                        id: more
                        anchors.top: parent.top
                        anchors.topMargin: 50
                        anchors.right: parent.right
                        anchors.rightMargin: 32
                        width: 41
                        height: 41
                        visible: (listview.editing || iconValue == 0) ? false : true
                        source: "qrc:/main/images/icon_more.png"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                bIsPush = true
                                if (historyTable.getCallHistoryValue(index,"serialnum") == "null") {
                                    pageStack.push("qrc:/qml/CallRecordInfo.qml",{"bIsStranger":true,"personName":historyTable.getCallHistoryValue(index,"num"),"myIndex":index})
                                    getData.getCallHistoryBytarget(historyTable.getCallHistoryValue(index,"num"),true)
                                } else {
                                    pageStack.push("qrc:/qml/CallRecordInfo.qml",{"personPost":historyTable.getCallHistoryValue(index,"info"),
                                                       "usrid":historyTable.getCallHistoryValue(index,"serialnum"),"myIndex":index})
                                    getData.getCallHistoryBytarget(historyTable.getCallHistoryValue(index,"serialnum"),false)
                                }
                            }
                        }
                    }
                    CLine {
                        id: line
                        anchors.top: phonenum.bottom
                        anchors.topMargin: 16
                        anchors.left: callflag.right
                        anchors.leftMargin: 16
                        width: parent.width - 126
                        color: "#36465a"
                    }
                    Rectangle {
                        id: clickeffects
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: parent.height
                        opacity: 0
                        color: "#546e8f"
                    }
                }
            }
//            onContentYChanged: {
//                bIsSlip = true
//                if (!bIsUpdate) {
//                    if (loader.visible&&!bIsChange&&!bIsKeyClick) {
//                        loader.visible = false
//                        toolBar.visible = true
//                        numkey2.visible = true
//                    }
//                } else {
//                    bIsUpdate = false
//                }
//            }
            onMovementStarted: {
                bIsSlip = true
//                if (!bIsUpdate) {
                    if (loader.visible ){ // && !bIsChange && !bIsKeyClick) {
                        loader.visible = false
                        toolBar.visible = true
                        numkey2.visible = true
                    }
//                } else {
//                    bIsUpdate = false
//                }
            }

            onFlickEnded: {
                bIsSlip = false
                bIsKeyClick = false
            }

            onEditingChanged: {
                if (listview.editing) {
                    loader.visible = false
                    numkey2.visible = false
                    toolBar.visible = false
                    deleteBar.visible = true
                    listview.width = parent.width
                } else {
                    if (bIsChange) {
                        listview.width = parent.width - 560
                        toolBar.visible = true
                    } else {
                        listview.width = parent.width
                    }
                    loader.visible = true
                }
            }
        }


        CToolBar {
            id: deleteBar
            anchors.bottom: parent.bottom
            height: 120
            width: parent.width
            backgroundEnabled: true
            visible: listview.editing ? true : false
            backgroundComponent: Component {
                Rectangle {
                    anchors.fill: parent
                    color: "#546e8f"
                    opacity: 0.3
                }
            }
            CButton {
                height: 80
                width: 200
                backgroundEnabled: false
                text: "删除"
                textColor: "#29afe2"
//                backgroundComponent: Component {
//                    Image {
//                        anchors.fill: parent
//                        source: "qrc:/main/images/delete.png"
//                    }
//                }
                onClicked: {
                    if (listview.selectedCount) {
                        deleteback.opacity = 0.8
                        deleteback.visible = true
                        dialog.visible = true
                    }
                }
            }
        }
        Item {
            id: numkey2
            anchors.right: parent.right
            anchors.bottom: toolBar.top
            anchors.rightMargin: 32
            anchors.bottomMargin: 23
            height: 126
            width: 126
            visible: bIsChange ? false : true
            Image {
                id: numkey3
                anchors.centerIn: parent
                source: "qrc:/main/images/icon_jianpan2.png"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (bIsSlip)
                        bIsKeyClick = true
                    listview.editing = false
                    toolBar.visible = false
                    loader.visible = true
                }
                onPressed: {
                    //numkey3.source = "qrc:/history/images/numkey_click.png"
                }
                onReleased: {
                    //numkey3.source = "qrc:/history/images/numkey.png"
                }
            }
        }
        Menu {
            id: toolBar
            visible: bIsChange ? true : false
            anchors.bottom: parent.bottom
            height: 120
            width: parent.width
            group.iconSource: "qrc:/main/images/group.png"
            home.iconSource: "qrc:/main/images/home.png"
            history.iconSource: "qrc:/main/images/history_click.png"
            bIsGroup: false
            bIsHistory: true
        }
        ListModel{
            id: listModel
        }
        Loader {
            id: loader
            anchors.bottom: bIsChange ? toolBar.top : parent.bottom
            anchors.right: parent.right
            sourceComponent: keyboard
            Component.onCompleted: {
                listModel.append({"pressadr":"qrc:/history/images/1-click.png",
                                     "numadr":"qrc:/history/images/1.png","value":"1"})
                listModel.append({"pressadr":"qrc:/history/images/2-click.png",
                                     "numadr":"qrc:/history/images/2.png","value":"2"})
                listModel.append({"pressadr":"qrc:/history/images/3-click.png",
                                     "numadr":"qrc:/history/images/3.png","value":"3"})
                listModel.append({"pressadr":"qrc:/history/images/4-click.png",
                                     "numadr":"qrc:/history/images/4.png","value":"4"})
                listModel.append({"pressadr":"qrc:/history/images/5-click.png",
                                     "numadr":"qrc:/history/images/5.png","value":"5"})
                listModel.append({"pressadr":"qrc:/history/images/6-click.png",
                                     "numadr":"qrc:/history/images/6.png","value":"6"})
                listModel.append({"pressadr":"qrc:/history/images/7-click.png",
                                     "numadr":"qrc:/history/images/7.png","value":"7"})
                listModel.append({"pressadr":"qrc:/history/images/8-click.png",
                                     "numadr":"qrc:/history/images/8.png","value":"8"})
                listModel.append({"pressadr":"qrc:/history/images/9-click.png",
                                     "numadr":"qrc:/history/images/9.png","value":"9"})
                listModel.append({"pressadr":"qrc:/history/images/xing_click.png",
                                     "numadr":"qrc:/history/images/xing.png","value":"*"})
                listModel.append({"pressadr":"qrc:/history/images/0-click.png",
                                     "numadr":"qrc:/history/images/0.png","value":"0"})
                listModel.append({"pressadr":"qrc:/history/images/jing-click.png",
                                     "numadr":"qrc:/history/images/jing.png","value":"#"})
            }
        }
    }
    Rectangle {
        id: deleteback
        anchors.fill: parent
        color: "black"
        visible: false
        opacity: 0.3
        radius: 10
        z:10
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
        visible: false
        anchors.centerIn: parent
        width: 504
        height: 296
        radius: 10
        z:10
//        Image {
//            anchors.fill: parent
//            source: "qrc:/history/images/dialog.png"
//        }
        Text {
            id: message
            anchors.top: parent.top
            anchors.topMargin: 79
            height: 32
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
            backgroundComponent: Component {
                Rectangle {
                    anchors.fill: parent
                    radius: 10
                    color: "#29afe2"
                }
            }
            width: 150
            height: 67
            text: "取消"
            textColor: "#ffffff"
            //iconSource: pressed ? "qrc:/history/images/cancelclick.png" : "qrc:/history/images/canceldelete.png"
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
            backgroundComponent: Component {
                Rectangle {
                    anchors.fill: parent
                    radius: 10
                    color: "#29afe2"
                }
            }

            text: "确认"
            textColor: "#ffffff"
            //iconSource: pressed ? "qrc:/history/images/sureclick.png" : "qrc:/history/images/suredelete.png"
            onClicked: {
                var indexs = listview.selectedIndexes
                for (var i = indexs.length - 1; i >= 0; --i){
                    if (historyTable.getCallHistoryValue(indexs[i],"serialnum") == "null") {
                        getData.removeCallHistory(historyTable.getCallHistoryValue(indexs[i],"num"),true)
                    } else {
                        getData.removeCallHistory(historyTable.getCallHistoryValue(indexs[i],"serialnum"),false)
                    }
                }
                getData.getSearchCallHistory(calltype,inputinfo)
                dialog.visible = false
                deleteback.opacity = 0
                deleteback.visible = false
                historyPersonDelte(false)
                listview.deselect()
            }
        }
    }
    Component {
        id: twoLine
        Item {
            width: bIsChange ? 540 : listview.width
            height: 2
            CLine {
                id: line1
                anchors.left: parent.left
                anchors.leftMargin: bIsChange ? 18 : 30
                width: bIsChange ? parent.width - 36 : parent.width - 60
                color: "#cbcbcb"
            }
            CLine {
                id: line2
                anchors.top: line1.bottom
                anchors.left: parent.left
                anchors.leftMargin: bIsChange ? 18 : 30
                width: bIsChange ? parent.width - 36 : parent.width - 60
                color: "#121212"
            }
        }
    }
    Component {
        id:keyboard
        Rectangle {
            id: background
            function clearLoader()
            {
                inputText.text = ""
                bIsFirst = true
                inputinfo = inputText.text
                mousearea.visible = false
                inputLine.visible = false
                line.anchors.top = background.top
                if (bIsChange) {
                    background.height = 504
                    line.anchors.topMargin = 145
                    inputText.visible = true
//                    deleteinput.visible = false
                    key.anchors.top = inputText.bottom
                } else {
                    background.height = 531
                    line.anchors.topMargin = 100
                    inputText.visible = false
                    key.anchors.top = background.top
                }
            }
            color: "#ffffff"
            //source: "qrc:/history/images/num_back.png"
            width: bIsChange ? 540 : listview.width
            height: bIsChange ? 504 : 531
            Connections {
                target:gScreenInfo
                onCurrentOrientationChanged: {
                    if (inputinfo != "") {
                        if (bIsChange) {
                            background.height = 504
                            line.anchors.topMargin = 145
                            inputText.visible = true
                        } else {
                            background.height = 617
                            line.anchors.topMargin = 186
                        }
                        key.anchors.top = inputText.bottom
                    } else {
                        if (bIsChange) {
                            background.height = 504
                            line.anchors.topMargin = 145
                            inputText.visible = true
                            key.anchors.top = inputText.bottom
                        } else {
                            background.height = 531
                            mousearea.visible = false
                            inputLine.visible = false
                            inputText.visible = false
                            key.anchors.top = background.top
                            line.anchors.top = background.top
                            line.anchors.topMargin = 100
                        }
                    }
                }
            }
            Text {
                id: inputText
                anchors.left: parent.left
                anchors.leftMargin: bIsChange ? 0 : 48
                anchors.top: parent.top
                width: bIsChange ? 436 : 562
                height: bIsChange ? 70 : 86
                clip: true
                font.pixelSize: 50
                visible: bIsChange ? true : false
                horizontalAlignment: Qt.AlignRight
                verticalAlignment: Qt.AlignVCenter
                color: "black"
                text: ""
//                Rectangle {
//                    width: inputText.width - 50
//                    height: inputText.height
//                    color: "#546e8f"
//                    Rectangle {
//                        anchors.centerIn: parent
//                        width: parent.width - 10
//                        height: parent.height - 5
//                        radius: 5
//                        color: "#ffffff"
//                    }
//                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
                onTextChanged: {
                    bIsUpdate = true
                    getData.getSearchCallHistory(calltype,inputText.text)
                    inputText.visible = true
                    mousearea.visible = true
                    deleteinput.visible = true
                    inputLine.visible = true
                }
            }

            MouseArea {
                id: mousearea
                anchors.left: inputText.right
                anchors.leftMargin: 19
                anchors.top: inputText.top
                width: 61
                height: bIsChange ? 70 : 86
                visible: false
                onClicked: {
                    inputText.text = inputText.text.substring(0,inputText.text.length - 1)
                    inputinfo = inputText.text
                    if (inputText.text == "") {
                        bIsFirst = true
                        mousearea.visible = false

                        line.anchors.top = parent.top
                        if (bIsChange) {
                            background.height = 504
                            line.anchors.topMargin = 145
                            inputText.visible = true
                            deleteinput.visible = false
                            inputLine.visible = true
                            key.anchors.top = inputText.bottom
                        } else {
                            background.height = 531
                            line.anchors.topMargin = 100
                            inputText.visible = false
                            inputLine.visible = false
                            key.anchors.top = parent.top
                        }
                    }
                }
                onPressAndHold: {
                    inputText.text = ""
                    bIsFirst = true
                    inputinfo = inputText.text
                    mousearea.visible = false
                    inputLine.visible = false
                    line.anchors.top = parent.top
                    if (bIsChange) {
                        background.height = 504
                        line.anchors.topMargin = 145
                        inputText.visible = true
                        deleteinput.visible = false
                        key.anchors.top = inputText.bottom
                    } else {
                        background.height = 531
                        line.anchors.topMargin = 100
                        inputText.visible = false
                        key.anchors.top = parent.top
                    }
                }
                onPressed: {
                    //deleteinput.source = "qrc:/history/images/input_delete_click.png"
                }
                onReleased: {
                    //deleteinput.source = "qrc:/history/images/input_delete.png"
                }
                Image {
                    id: deleteinput
                    width: 43
                    height: 30
                    anchors.centerIn: parent
                    source: "qrc:/main/images/icon_delete2.png"
                }
            }
            CLine {
                id: inputLine
                anchors.top: inputText.bottom
                width: parent.width
                color: "#121212"
                visible: false
            }
            Column {
                id: line
                anchors.top: parent.top
                anchors.topMargin: bIsChange ? 75 : 100
                spacing: bIsChange ? 75 : 100
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
                anchors.top: bIsChange ? inputText.bottom : parent.top
                Repeater {
                    model: listModel
                    Image {
                        id: num
                        width: bIsChange ? 180 : 240
                        height: bIsChange ? 75 : 100
                        //source: numadr
                        Text {
                            anchors.centerIn: parent
                            text: value
                            font.pixelSize: 40
                        }
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                if (inputText.text == "") {
                                    if (bIsChange) {
                                        background.height = 504
                                        line.anchors.topMargin = 145
                                    } else {
                                        background.height = 617
                                        line.anchors.topMargin = 186
                                    }
                                    key.anchors.top = inputText.bottom
                                }
                                num.source = pressadr
                                inputText.text += listModel.get(index).value
                                inputinfo = inputText.text
                            }
                            onReleased: {
                                num.source = numadr
                            }
                        }
                    }
                }
            }
            Image {
                id: numkey
                anchors.right: parent.right
                anchors.rightMargin: 50
                anchors.top: key.bottom
                anchors.topMargin: 30
                width: 80
                height: 80
                visible: bIsChange ? false : true
                source: "qrc:/main/images/icon_jianpan.png"
                Rectangle {
                    id: numkeyClick
                    anchors.fill: parent
                    color: "#546e8f"
                    opacity: 0
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        bIsKeyClick = false
                        loader.visible = false
                        numkey2.visible = true
                        toolBar.visible = true
                    }
                    onPressed: {
                        numkeyClick.opacity = 0.3
                        //numkey.source = "qrc:/history/images/numkey_click.png"
                    }
                    onReleased: {
                        numkeyClick.opacity = 0
                        //numkey.source = "qrc:/history/images/numkey.png"
                    }
                }
            }

//            Image {
//                id: saveKey
//                anchors.right: parent.right
//                anchors.rightMargin: 50
//                anchors.top: key.bottom
//                anchors.topMargin: 30
//                width: 80
//                height: 80
//                visible: bIsChange ? false : true
//                source: "qrc:/main/images/icon_save.png"
//                Rectangle {
//                    id: saveKeyClick
//                    anchors.fill: parent
//                    color: "#546e8f"
//                    opacity: 0
//                }

//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        bIsKeyClick = false
//                        loader.visible = false
//                        numkey2.visible = true
//                        toolBar.visible = true
//                    }
//                    onPressed: {
//                        saveKeyClick.opacity = 0.3
//                        //numkey.source = "qrc:/history/images/numkey_click.png"
//                    }
//                    onReleased: {
//                        saveKeyClick.opacity = 0
//                        //numkey.source = "qrc:/history/images/numkey.png"
//                    }
//                }
//            }

            Image {
                id: call
                anchors.top: key.bottom
                anchors.topMargin: 30//bIsChange ? 28 : 16
                anchors.horizontalCenter: parent.horizontalCenter
//                anchors.left: bIsChange ? parent.left : numkey.right
//                anchors.leftMargin: bIsChange ? 170 : 90
                width: 80//bIsChange ? 200 : 300
                height: 80//bIsChange ? 67 : 99
                source: "qrc:/main/images/icon_call.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var bIsInList = false
                        var url;
                        if (bIsFirst && inputText.text == "" && historyTable.count() != 0 ) {
                            bIsFirst = false
                            inputText.text = historyTable.getCallHistoryValue(0,"num")
                            inputinfo = inputText.text
                            if (bIsChange) {
                                background.height = 504
                                line.anchors.topMargin = 145
                                key.anchors.top = inputText.bottom
                            } else {
                                background.height = 617
                                line.anchors.topMargin = 186
                                key.anchors.top = inputText.bottom
                            }

                        } else {
                            for (var i = 0; i < historyTable.count(); i++)
                            {
                                if (inputText.text == historyTable.getCallHistoryValue(i,"num")) {
                                    bIsInList = true
                                    url = "sinuxtel:makeCall?net="+historyTable.getCallHistoryValue(i,"netinfo")+ "&value=" + historyTable.getCallHistoryValue(i,"num")
                                    break;
                                }
                            }
                            if (!bIsInList) {
                                if (inputText.text.substring(0,2) != "##")
                                    url = "sinuxtel:makeCall?net=天通网&value=" + inputText.text
                                else
                                {//获取本地有的网络，只有一个直接打，多个弹出提示框
                                   var list = getData.getLocalNumCount();
                                    if (list == 1)
                                    {
                                        url = "sinuxtel:makeCall?net=" + getData.getLocalNum(0) + "&value=" + getData.transtoIp(inputText.text)
                                        console.log("url === ", url)
                                    }
                                    else if (list > 1)
                                    {
                                        deleteback2.visible = true
                                        dialog2.visible = true
                                        deleteback2.opacity = 0.3
                                        dialog2.count = list
                                        dialog2.phoneNum = getData.transtoIp(inputText.text)
                                        return
                                    }
                                }

                            }
                            if (inputinfo != "")
                                qApp.openUrl(url)
                            inputText.text = ""
                            inputinfo = ""
                            bIsFirst = true
                            inputLine.visible = false
                        }
                    }
                    onReleased: {
                        //call.source = "qrc:/history/images/call.png"
                    }
                    onPressed: {
                        //call.source = "qrc:/history/images/call_click.png"
                    }
                }
            }
        }
    }

    Rectangle {
        id: deleteback2
        anchors.fill: parent
        color: "black"
        visible: false
        opacity: 0
        z: 4
        MouseArea {
            anchors.fill: parent
            onClicked: {
                deleteback2.visible = false
                dialog2.visible = false
                deleteback2.opacity = 0
            }
        }
    }
    Rectangle {
        id: dialog2
        property var count: 0
        property var phoneNum: ""
        visible: false
        anchors.centerIn: parent
        width: 492
        height: 120 * dialog2.count
        z: 4
        Image {
            anchors.fill: parent
            source: "qrc:/history/images/dialog.png"
        }
        Column {
            Repeater {
                model: dialog2.count
                CButton {
                    width: 492
                    height: 120
                    backgroundEnabled: false
                    text: getData.getLocalNum(index)
                    textColor: "#ffffff"
                    pixelSize: 30
                    onClicked: {
                        deleteback2.visible = false
                        dialog2.visible = false
                        deleteback2.opacity = 0

                        url = "sinuxtel:makeCall?net=" + getData.getLocalNum(index) + "&value=" + dialog2.phoneNum
                        qApp.openUrl(url)
                    }
                }
            }
        }
    }


    Component {
        id: title
        Rectangle {
            width: 360
            height: 50
            border.color: "#29afe2"
            border.width: 2
            color: Qt.rgba(0,0,0,0)
            radius: 10
            property bool allpressed: true
            property bool callpressed: false
            property bool dialpressed: false
            property bool uncallpressed: false
            CButton{
                id: allButton
                width: 90
                height: 50
                text: "全部"
                textColor: "#ffffff"
                backgroundEnabled: true
                backgroundComponent: Component {
                    Rectangle {
                        anchors.fill: parent
                        radius: 10
                        color: "#29afe2"
                    }
                }
                onClicked: {
                    calltype = 0
                    getData.getSearchCallHistory(calltype,inputinfo)
                    if (callpressed || dialpressed || uncallpressed) {
                        if (allpressed) {
                            allButton.textColor = "#ffffff"
                            allButton.backgroundEnabled = true
                        } else {
                            if (callpressed) {
                                callButton.textColor = "#29afe2"
                                callButton.backgroundEnabled = false
                                callpressed = !callpressed
                            } else if (dialpressed) {
                                dialButton.textColor = "#29afe2"
                                dialButton.backgroundEnabled = false
                                dialpressed = !dialpressed
                            } else {
                                uncallButton.textColor = "#29afe2"
                                uncallButton.backgroundEnabled = false
                                uncallpressed = !uncallpressed
                            }
                            allButton.textColor = "#ffffff"
                            allButton.backgroundEnabled = true
                        }
                        allpressed = !allpressed
                    }
                }
            }
            CButton{
                id: callButton
                anchors.left: allButton.right
                width: 90
                height: 50
                text: "已接"
                textColor: "#29afe2"
                backgroundEnabled: false
                backgroundComponent: Component {
                    Rectangle {
                        anchors.fill: parent
                        radius: 10
                        color: "#29afe2"
                    }
                }
                onClicked: {
                    calltype = 1
                    getData.getSearchCallHistory(calltype,inputinfo)
                    if (allpressed || dialpressed || uncallpressed) {
                        if (callpressed) {
                            callButton.textColor = "#ffffff"
                            callButton.backgroundEnabled = true
                        } else {
                            if (allpressed) {
                                allButton.textColor = "#29afe2"
                                allButton.backgroundEnabled = false
                                allpressed = !allpressed
                            } else if (dialpressed) {
                                dialButton.textColor = "#29afe2"
                                dialButton.backgroundEnabled = false
                                dialpressed = !dialpressed
                            } else {
                                uncallButton.textColor = "#29afe2"
                                uncallButton.backgroundEnabled = false
                                uncallpressed = !uncallpressed
                            }
                            callButton.textColor = "#ffffff"
                            callButton.backgroundEnabled = true
                        }
                        callpressed = !callpressed
                    }
                }
            }
            CButton{
                id: dialButton
                anchors.left: callButton.right
                width: 90
                height: 50
                text: "已拨"
                textColor: "#29afe2"
                backgroundEnabled: false
                backgroundComponent: Component {
                    Rectangle {
                        anchors.fill: parent
                        radius: 10
                        color: "#29afe2"
                    }
                }
                onClicked: {
                    calltype = 2
                    getData.getSearchCallHistory(calltype,inputinfo)
                    if (allpressed || uncallpressed || callpressed) {
                        if (dialpressed) {
                            dialButton.textColor = "#ffffff"
                            dialButton.backgroundEnabled = true
                        } else {
                            if (allpressed) {
                                allButton.textColor = "#29afe2"
                                allButton.backgroundEnabled = false
                                allpressed = !allpressed
                            } else if (uncallpressed) {
                                uncallButton.textColor = "#29afe2"
                                uncallButton.backgroundEnabled = false
                                uncallpressed = !uncallpressed
                            } else {
                                callButton.textColor = "#29afe2"
                                callButton.backgroundEnabled = false
                                callpressed = !callpressed
                            }
                            dialButton.textColor = "#ffffff"
                            dialButton.backgroundEnabled = true
                        }
                        dialpressed = !dialpressed
                    }
                }
            }
            CButton{
                id: uncallButton
                anchors.left: dialButton.right
                width: 90
                height: 50
                text: "未接"
                textColor: "#29afe2"
                backgroundEnabled: false
                backgroundComponent: Component {
                    Rectangle {
                        anchors.fill: parent
                        radius: 10
                        color: "#29afe2"
                    }
                }
                onClicked: {
                    calltype = 3
                    getData.getSearchCallHistory(calltype,inputinfo)
                    if (allpressed || dialpressed || callpressed) {
                        if (uncallpressed) {
                            uncallButton.textColor = "#ffffff"
                            uncallButton.backgroundEnabled = true
                        } else {
                            if (allpressed) {
                                allButton.textColor = "#29afe2"
                                allButton.backgroundEnabled = false
                                allpressed = !allpressed
                            } else if (dialpressed) {
                                dialButton.textColor = "#29afe2"
                                dialButton.backgroundEnabled = false
                                dialpressed = !dialpressed
                            } else {
                                callButton.textColor = "#29afe2"
                                callButton.backgroundEnabled = false
                                callpressed = !callpressed
                            }
                            uncallButton.textColor = "#ffffff"
                            uncallButton.backgroundEnabled = true
                        }
                        uncallpressed = !uncallpressed
                    }
                }
            }
        }
    }
}
