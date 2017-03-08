import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {
    initialPage:CPage{
        width:parent.width
        height:parent.height
        Column {
            id: buttons
            anchors.top: parent.top
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            CButton {
                id: startButton
                width: 300
                height: 150
                text: "Start Talk"
                onClicked: {
                    startButton.textColor = Qt.rgba(0, 1, 0, 0.4)
                    endButton.textColor = Qt.rgba(0, 0, 0, 1)
                    getData.onStartTalkClick()
                }
            }

            CButton {
                id: endButton
                width: 300
                height: 150
                text: "End Talk"
                textColor: Qt.rgba(0, 1, 0, 0.4)
                onClicked: {
                    startButton.textColor = Qt.rgba(0, 0, 0, 1)
                    endButton.textColor = Qt.rgba(0, 1, 0, 0.4)
                    getData.onEndTalkClick();
                }
            }
        }


        TextEdit{
            id: log
            readOnly: true
            anchors.top: buttons.bottom;
            anchors.left: parent.left;
            width: parent.width
            height: 150
        }
    }

    function onTextAppend(text)
    {
        if(log.text.length > 512)
            log.text = "";
        log.text = log.text + text + "\n"
    }
}
