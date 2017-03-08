import QtQuick 2.0
import QtQuick.Dialogs 1.0
import com.syberos.basewidgets 2.0

Item {
    property alias myIconSource: photo.iconSource
    CMaskImage {
        id: photo
        width: 150
        height: 150
        iconSource: "qrc:/main/images/init_photo.png"
        maskComponent: Rectangle{
            id: msk
            parent: photo
            radius: photo.width  / 2
            anchors.fill: parent
            smooth: true
            visible: false
        }
    }
}

