import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Dialogs 1.0

Rectangle {
    id: setting
    property bool bIsLandscape: false
    width: 560//bIsLandscape ? 490 : 360
    height: bIsLandscape ? 720 : 1280
    color: "#36465a"
    Loader {
        id: photo
        width: 140
        height: 140
        anchors.top: parent.top
        anchors.topMargin: bIsLandscape ? 50 : 82
        anchors.left: parent.left
        anchors.leftMargin: 130
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
        font.pixelSize: 32
        color: "#ffffff"
        anchors.left: photo.right
        anchors.leftMargin: 52
        anchors.verticalCenter: photo.verticalCenter
    }
    Column {
        spacing: bIsLandscape ? 40 : 70
        anchors.top: photo.bottom
        anchors.topMargin: 109
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        CButton {
            text: qsTr("删除")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 28

            iconSource: "qrc:/main/images/icon_delete.png"
            width: parent.width
            spacing: 88
            onClicked: {
                mainroot.setting(1)
            }
        }
        CButton {
            text: qsTr("更新")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 28
            iconSource: "qrc:/main/images/icon_update.png"
            width: parent.width
            spacing: 88
            onClicked: {
                mainroot.setting(2)
            }
        }
        CButton {
            text: qsTr("推送")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 28
            iconSource: "qrc:/main/images/icon_tuisong.png"
            width: parent.width
            spacing: 88
            visible: false
            onClicked: {
                mainroot.setting(3)
            }
        }
        CButton {
            text: qsTr("存储情况")
            backgroundEnabled: false
            textColor: "#ffffff"
            pixelSize: 28
            iconSource: "qrc:/main/images/icon_cun.png"
            width: parent.width
            spacing: 88
            onClicked: {
                mainroot.setting(4)
            }
        }
    }
    Component.onCompleted: {
        name.text = getData.getLocalPersonName()
        var photoPath = getData.getLocalPhoto()
        if (photoPath == "null" || photoPath == ""){
            photo.item.myIconSource = "qrc:/main/images/init_photo.jpg"
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
