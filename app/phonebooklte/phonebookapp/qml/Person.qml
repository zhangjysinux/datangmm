import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.1
CPage {
    id: person
    contentAreaItem: Item {
        anchors.fill: parent
        CEditTitleBar{
            id:titleBar
            anchors.top: parent.top
            height: 96
            width: parent.width
            leftItemEnabled: true
            rightItemEnabled: true
            leftItemSize: Qt.size(93,74)
            rightItemSize: Qt.size(74,74)
            rightItemBackgroundEnabled: false
            rightItemComponent:     Component {
                id: rightTitleBar
                Item {
                    Text {
                        font.pixelSize: 30
                        anchors.centerIn: parent
                        color: "#ffffff"
                        text: qsTr("Edit")

                    }
                }
            }
            leftItemComponent:     Component {
                id: leftTitleBar
                Item {
                    Image {
                        id: allPerson
                        anchors.centerIn: parent
                        height: 38
                        width: 37
                        source: "qrc:/main/images/setting.png"
                    }
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 30
                        anchors.left: allPerson.right
                        text: qsTr("All Person")
                        color: "#ffffff"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            pageStack.pop()
                        }
                    }
                }
            }
            titleText: qsTr("")
            titlePixelSize: 30
            titleTextColor: "#f9f9f9"
            editView: listView
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
        Image {
            id: photo
            anchors.top: titleBar.bottom
            anchors.topMargin: 32
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/main/images/photo.png"
        }
        Text {
            id: name
            anchors.top: photo.bottom
            anchors.topMargin: 32
            text: qsTr("普京")
            font.pixelSize: 40
            color: "#ffffff"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        TextInput {
            id: job
            anchors.top: name.bottom
            anchors.topMargin: 32
            anchors.left: parent.left
            anchors.leftMargin: 32
            color: "white"
            readOnly: true
            font.pixelSize: 30
            text: "职位: 指挥官"
        }
        TextInput {
            id: num
            anchors.top: job.bottom
            anchors.topMargin: 32
            anchors.left: job.left
            color: "white"
            readOnly: true
            font.pixelSize: 30
            text: "编号: KGW54898"
        }
        CLine {
            id: line
            anchors.top: num.bottom
            anchors.topMargin: 32
            width: parent.width - 32
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#9b9b9b"
        }
    }
}
