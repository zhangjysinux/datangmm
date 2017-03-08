import QtQuick 2.0
import com.syberos.basewidgets 2.0

// 主界面
Item {
    width: parent.width
    height: parent.height

    Image {
        anchors.fill: parent
        fillMode: Image.Tile
        source: "qrc:/res/bg.png"
    }

    Image {
        anchors.fill: parent
        source: "qrc:/res/bgshadow.png"
    }

    Grid {
        anchors.centerIn: parent
        rows: 2
        columns: 2
        rowSpacing: 65
        columnSpacing: 65

        MainButton {
            iconSource: "qrc:/res/realtimesend.png"
            onClicked: {
                pageStack.push("qrc:/qml/ContactPage.qml")
            }
        }

        MainButton {
            iconSource: "qrc:/res/filesend.png"
            onClicked: {
                pageStack.push("qrc:/qml/SelectContactsPage.qml");
            }
        }

        MainButton {
            iconSource: "qrc:/res/videorecording.png"
            onClicked: {
                pageStack.push("qrc:/qml/VideoRecordPage.qml");
            }
        }

        MainButton {
            iconSource: "qrc:/res/videoview.png"
            onClicked: {
                pageStack.push("qrc:/qml/SelectVideoPage.qml");
            }
        }

    }
}
