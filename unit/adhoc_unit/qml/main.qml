import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        width:parent.width
        height:parent.height
        Column {
            id: buttonBox
            spacing: 30
            anchors.centerIn: parent
            CButton {
                width: 400
                height: 150
                text: "自组网"
                onClicked: {
                    myDbus.testAdhoc()
                }
            }
            CButton {
                width: 400
                height: 150
                text: "set true"
                onPressed: {
                    myDbus.onSetPtt(true)
                }
                onReleased: {
                    myDbus.onSetPtt(false)
                }
            }
        }
    }
}

