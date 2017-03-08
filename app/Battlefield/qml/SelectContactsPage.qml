import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: root

    contentAreaItem: Item {

        Image {
            anchors.fill: parent
            fillMode: Image.Tile
            source: "qrc:/res/bg.png"
        }

        //Title
        Item {
            width: parent.width
            height: 96
            anchors.left: parent.left
            anchors.top: parent.top

            CButton {
                width: 96; height: 72
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 32
                backgroundComponent: Item {
                    Image {
                        anchors.fill: parent
                        source: "qrc:/res/back.png"
                    }
                }
                onClicked: {
                    pageStack.pop();
                }
            }
        }

        Column {
            anchors.centerIn: parent
            spacing: 65

            MainButton {
                iconSource: "qrc:/res/contact.png"
                onClicked: {

                }
            }

            MainButton {
                iconSource: "qrc:/res/group.png"
                onClicked: {

                }
            }
        }
    }
}
