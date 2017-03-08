import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.0
import com.syberos.multimedia.photos 2.0

CPage {
    id: addPerson
    property var m_name: "value"
    property variant containerqml: null
    property string thenetinfo: "LTE"
    property string thenuminfo: ""
    property bool bIsInput: false
    property bool bIsUpdate: false
    property bool bIschange: false
    function setSource(url)
    {
        addPhoto.item.myIconSource = url
        gScreenInfo.setStatusBar(true)
        gScreenInfo.setStatusBarStyle("black")
    }
    orientationPolicy: CPageOrientation.Automatic
    Component.onCompleted:	{
        if (gScreenInfo.displayHeight == 800)
            addPerson.orientationPolicy = CPageOrientation.LockLandscape
        addPerson.color = "black"
        if (gScreenInfo.currentOrientation == 2 ||
                gScreenInfo.currentOrientation == 8) {
            addPerson.statusBarHoldEnabled = false
            gScreenInfo.setStatusBar(false);
            bIschange = true
        } else {
            addPerson.statusBarHoldEnabled = true
            gScreenInfo.setStatusBar(true);
            bIschange = false
        }
    }
    Connections{
        target: gScreenInfo
        onCurrentOrientationChanged:{
            if (gScreenInfo.currentOrientation == 2 ||
                    gScreenInfo.currentOrientation == 8) {
                addPerson.statusBarHoldEnabled = false
                gScreenInfo.setStatusBar(false);
                bIschange = true
            } else {
                addPerson.statusBarHoldEnabled = true
                gScreenInfo.setStatusBar(true);
                bIschange = false
            }
        }
    }

    function addNetworkType(network)
    {
        listModel.insert(listModel.count - 1,{"textTitle":network})
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
            leftItemComponent: leftTitleBar
            rightItemComponent: rightTitleBar
            //titleText: qsTr("LinkMan")
            titlePixelSize: 30
            titleTextColor: "#f9f9f9"
            titleAreaCentered: true
            editView: listView
            spacingBetweenLeftBorderAndLeftItem: 16
            spacingBetweenRightBorderAndRightItem: 16
            editing: false
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
            onLoaded: addPhoto.item.myIconSource = "qrc:/main/images/add_photo.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pageStack.push("qrc:/qml/ChoicePhoto.qml",{parentSource:addPerson})
                }
            }
        }

        Item {
            id: setMyName
            anchors.top: titleBar.bottom
            anchors.topMargin: 61
            anchors.left: addPhoto.right
            anchors.leftMargin: 48
            anchors.right: parent.right
            anchors.rightMargin: 48
            visible: true
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
                font.pixelSize: 30
                clip: true
                validator:RegExpValidator{regExp:/(([\xB0-\xF7][\xA1-\xFE])|([\x81-\xA0][\x40-\xFE])|([\xAA-\xFE][\x40-\xA0])|(\w))+/}
                color: "#ffffff"
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
                anchors.left: parent.left
                color: "#9b9b9b"
            }
            Text {
                id: secondName
                anchors.top: lineFirst.bottom
                anchors.topMargin: 30
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
                anchors.left: lineFirst.left
            }
        }

        ListModel {
            id: listModel
        }
        ListView {
            id: listView
            model: listModel
            width: parent.width - 96
            anchors.top: addPhoto.bottom
            anchors.bottom: parent.bottom
            clip: true
            anchors.topMargin: 24
            delegate: myLine
            cacheBuffer: listView.height * 4
            Component.onCompleted: {
                listModel.append({"textTitle":"职位","value":""})
                listModel.append({"textTitle":thenetinfo,"value":thenuminfo})
                listModel.append({"textTitle":"添加","value":""})
            }
            onContentYChanged: {
                bIsInput = false
                keyboardground.visible = false
                closeSoftKeyboard()
                console.log("2")

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
                        console.log("3")

                    }
                }
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
                    bluedialog.visible = false

                }
            }
        }
        Rectangle {
            property alias text: message.text
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
                anchors.topMargin: 80
                height: 80
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: 40
                text: qsTr("请输入完整的联系人信息!")
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
                    bluedialog.visible = false

                }
            }
        }

        Rectangle {
            id: bluedialog
            property alias dilogText: message2.text
            visible: false
            anchors.centerIn: parent
            width: 492
            height: 386
            z: 10
            Image {
                anchors.fill: parent
                source: "qrc:/bluetooth/images/bluetoothblack.png"
            }
            Text {
                id: message2
                anchors.top: parent.top
                anchors.topMargin: 52
                height: 80
                width: parent.width
                wrapMode: Text.Wrap
                lineHeight: 2
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: 40
                text: ""
            }
            CButton {
                id: deletecancel
                anchors.top: message2.bottom
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
                    bluedialog.visible = false
                    sureback.visible = false
                    suredialog.visible = false
                    sureback.opacity = 0
                }
            }
            CButton {
                id: deletesure
                anchors.top: message2.bottom
                anchors.topMargin: 76
                anchors.left: deletecancel.right
                anchors.leftMargin: 48
                width: 174
                height: 100
                backgroundEnabled: false
                iconSource: pressed ? "qrc:/history/images/sureclick.png" : "qrc:/history/images/suredelete.png"
                onClicked: {
                    bluedialog.visible = false
                    sureback.visible = false
                    suredialog.visible = false
                    sureback.opacity = 0
                    pageStack.pop()
                }
            }
        }
    }

    Component {
        id: myLine
        Item {
            id: tempItem
            height: 124
            width: listView.width - 50
            Text {
                id: lineHead
                color: "#b6b6b6"
                text: textTitle
                font.pixelSize: 30
                anchors.topMargin: 24
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 96
                height: 30
                onTextChanged: {
                    if (text == "自组网"  || text == "战互网" || text == "天通网" || text == "LTE" || text == "模块数字对讲" || text == "WIFI")
                    {
                        editDelete.visible = true
                    }
                    else if (text == "添加"){
                        editAdd.visible = true
                        lineValue.visible = false
                        line.anchors.top = lineHead.bottom
                        tempItem.height = 96
                    }
                    if (text == "天通网" || text == "模块数字对讲")
                        lineValueNum.visible = true;
                    else if (text == "自组网"  || text == "LTE" || text == "WIFI" || text == "战互网")
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
                anchors.left: lineHead.left
                verticalAlignment: Qt.AlignVCenter
                anchors.right: parent.right
                clip: true
                onTextChanged: {
                    if (bIsInput) {
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
                            keyboardground.visible = true
                            keyboardground.inputinfo.cursorVisible = true
                            keyboardground.inputinfo.text = lineValue.text
                            keyboardground.inputinfo.myindex = lineHead.text
                        } else {
                            bIsUpdate = false
                            keyboardground.visible = false
                            closeSoftKeyboard()
                            console.log("4")
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
                anchors.left: lineHead.left
                verticalAlignment: Qt.AlignVCenter
                anchors.right: parent.right
                clip: true
                onTextChanged: {
                    if (bIsInput) {
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
                            keyboardground.visible = true
                            keyboardground.inputinfo.cursorVisible = true
                            keyboardground.inputinfo.text = lineValueNum.text
                            keyboardground.inputinfo.myindex = lineHead.text
                        } else {
                            bIsUpdate = false
                            keyboardground.visible = false
                            closeSoftKeyboard()
                            console.log("5")

                        }
                    }
                }
            }
            TextInput {
                id: lineValueIp
                //validator: RegExpValidator{regExp: /[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}/}
                validator: RegExpValidator{regExp: /((?:(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(?:25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d))))/}

                visible: false
                text: value
                color: "#ffffff"
                font.pixelSize: 30
                anchors.top: lineHead.bottom
                anchors.topMargin: 16
                anchors.left: lineHead.left
                verticalAlignment: Qt.AlignVCenter
                anchors.right: parent.right
                clip: true
                onTextChanged: {
                    if (bIsInput) {
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
                    console.log("onFocusChanged",bIschange,bIsInput,index,lineValue.text)

                    if (bIschange || (!bIschange && index > 2)) {
                        bIsInput = !bIsInput
                        if (bIsInput) {
                            keyboardground.visible = true
                            keyboardground.inputinfo.cursorVisible = true
                            keyboardground.inputinfo.text = lineValueIp.text
                            keyboardground.inputinfo.myindex = lineHead.text
                        } else {
                            bIsUpdate = false
                            keyboardground.visible = false
                            closeSoftKeyboard()
                            console.log("6")

                        }
                    }
                }
            }
            CLine {
                id: line
                width: listView.width - 50
                anchors.top: lineValue.bottom
                anchors.topMargin: 24
                color: "#9b9b9b"
                anchors.left: lineValue.left
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
                anchors.verticalCenter: parent.verticalCenter
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
                anchors.right: line.left
                anchors.rightMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    pageStack.push("qrc:/qml/PersonLable.qml",{containerqml:addPerson})
                    for (var i = 1; i < listModel.count - 1; i++) {
                        getData.getCurrentNetworkInfo(listModel.get(i).textTitle)
                    }
                }
            }
        }
    }


    Component {
        id: leftTitleBar
        CButton {
            backgroundEnabled: false
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            iconSource: pressed ? "qrc:/main/images/back_click.png" : "qrc:/main/images/back.png"
            onClicked: {
                closeSoftKeyboard()
                console.log("7")

                sureback.visible = true
                sureback.opacity = 0.7
                bluedialog.visible = true
                bluedialog.dilogText = "是否退出当前编辑页面?"
            }
        }
    }



    Component {
        id: rightTitleBar
        CButton {
            id: button
            property bool checked: false
            anchors.fill: parent
            pixelSize: 30
            textColor: "white"
            backgroundEnabled: false
            text: qsTr("保存")
            iconSource: pressed ? "qrc:/main/images/font_click.png" : ""
            spacing: -68
            onClicked: {
                button.focus = true
                console.log("8")

                closeSoftKeyboard()
                var phontFlag = 0;
                var photoSource;
                var tipsText = ""
                var bIsphonenull = false;

                if (firstNameEdit.text == "")
                {
                    tipsText = "请输入联系人姓"
                    bIsphonenull = true
                }
                else if (secondNameEdit.text == "")
                {
                    tipsText = "请输入联系人名"
                    bIsphonenull = true
                }

                for (var i = 1; i < listModel.count - 1;i++) {
                    if (listModel.get(i).value == "" || listModel.get(i).value == null) {
                        if (tipsText == "")
                            tipsText = "请输入联系人" + listModel.get(i).textTitle + "号码"
                        bIsphonenull = true
                        break;
                    }
                }

                if (bIsphonenull) {
                    sureback.visible = true
                    sureback.opacity = 0.7
                    suredialog.visible = true
                    suredialog.text = tipsText
                } else {
                    if (bIsInput)
                        keyboardground.visible = false
                    for (var i = 0;i < listModel.count; i++ ){
                        if (listModel.get(i).textTitle == "添加")
                            listModel.remove(i,1)
                    }
                    for (var i = 1; i < listModel.count;i++) {
                        if (listModel.get(i).value) {
                            getData.setPhoneList(listModel.get(i).textTitle,listModel.get(i).value)
                        }
                    }
                    photoSource = addPhoto.item.myIconSource
                    if (photoSource == "qrc:/main/images/add_photo.png")
                    {
                        photoSource = "qrc:/main/images/init_photo.png"
                    }
                    getData.addContacter(m_name,listModel.get(0).value,photoSource)
                    if (containerqml.bIsStranger) {
                        containerqml.addPersonBackUp(m_name,photoSource)
                    }
                    pageStack.pop()
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("1")
            mainroot.closeSoftKeyboard()
        }
    }
}
