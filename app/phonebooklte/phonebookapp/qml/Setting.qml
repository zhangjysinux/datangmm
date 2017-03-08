import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Dialogs 1.0

Rectangle {
    id: setting
    property bool bIsLandscape: false
    width: bIsLandscape ? 490 : 360
    height: bIsLandscape ? 720 : 1280
    color: "black"
    Loader {
        id: photo
        width: 150
        height: 150
        anchors.top: parent.top
        anchors.topMargin: bIsLandscape ? 40 : 82
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/qml/Photo.qml"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                mainroot.pushPersonInfo()
            }
        }
    }

    Text {
        id: name
        text: qsTr("李军")
        font.pixelSize: 40
        color: "#ffffff"
        anchors.top: photo.bottom
        anchors.topMargin: bIsLandscape ? 30 : 32
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Column {
        spacing: bIsLandscape ? 40 : 70
        anchors.top: name.bottom
        anchors.topMargin: bIsLandscape ? 50 : 120
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        CButton {
            text: qsTr("删除")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 32
            iconSource: "qrc:/set/images/set_delete.png"
            width: parent.width
            spacing: 15
            onClicked: {
                mainroot.setting(1)
            }
        }
        CButton {
            text: qsTr("更新")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 32
            iconSource: "qrc:/set/images/set_updata.png"
            width: parent.width
            spacing: 15
            onClicked: {
                mainroot.setting(2)
            }
        }
        CButton {
            text: qsTr("推送")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 32
            iconSource: "qrc:/set/images/set_push.png"
            width: parent.width
            spacing: 15
            visible: false
            onClicked: {
                mainroot.setting(3)
            }
        }
        CButton {
            text: qsTr("存储情况")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 32
            iconSource: "qrc:/set/images/set_storage.png"
            width: parent.width
            spacing: 15
            onClicked: {
                mainroot.setting(4)
            }
        }
    }
    Component.onCompleted: {
        name.text = getData.getLocalPersonName()
        var photoPath = getData.getLocalPhoto()
        if (photoPath == "null" || photoPath == ""){
            photo.item.myIconSource = "qrc:/main/images/init_photo.png"
        }else {
            photo.item.myIconSource = photoPath
        }
    }
    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            photo.iconSource = fileDialog.fileUrls
            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
    }

}
