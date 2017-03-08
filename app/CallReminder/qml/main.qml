import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        width:parent.width
        height:parent.height

        CallReminder {
            width: parent.width
            height: 200
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 40
        }

        CallOut {
            width: parent.width
            height: 200
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 40
        }

        CLineEdit {
            id: input
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            width: 200
        }

        CButton {
            id: button1
            anchors.left: input.right
            anchors.leftMargin: 10
            anchors.top: input.top
            text: "CALL"
            onClicked: {
                callOutViewModel.startCallOut(1, "李军", input.text);
            }
        }

    }
}

