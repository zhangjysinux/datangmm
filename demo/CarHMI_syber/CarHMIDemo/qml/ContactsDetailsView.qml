import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    ListView {
        id: list

        model: contactData
        delegate: itemDrawer
        anchors.top: parent.top
        anchors.topMargin: styles.uiAppGap
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2*styles.uiAppGap
        anchors.left: parent.left
        anchors.leftMargin: styles.uiAppGap
        width: parent.width/2 - styles.uiAppGap
        clip: true
    }

    Component {
        id: itemDrawer

        Item {
            width: itemText.width
            height: itemText.height

            Text {
                id: itemText

                text: contactData[index].name
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: index === currentIndex ? styles.highlightColor : styles.lowColor
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    currentIndex = index;
                }
            }
        }
    }

    Column {
        id: currentContact

        anchors.left: list.right
        anchors.leftMargin: styles.uiAppGap
        anchors.right: parent.right
        anchors.rightMargin: styles.uiAppGap
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2*styles.uiAppGap
        spacing: 20

        Column {
            Text {
                text: "FIRST NAME"
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.highlightColor
            }

            Text {
                text: currentIndex !== -1 ? contactData[currentIndex].name.split(" ")[0] : " "
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.lowColor
                opacity: animationOpacity
            }
        }

        Column {
            Text {
                text: "LAST NAME"
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.highlightColor
            }

            Text {
                text: currentIndex !== -1 ? contactData[currentIndex].name.split(" ")[1] : " "
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.lowColor
                opacity: animationOpacity
            }
        }

        Column {
            Text {
                text: "LOCATION"
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.highlightColor
            }

            Text {
                text: currentIndex !== -1 ? contactData[currentIndex].city : " "
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.lowColor
                opacity: animationOpacity
            }
        }

        Column {
            Text {
                text: "MOBILE"
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.highlightColor
            }

            Text {
                text: currentIndex !== -1 ? contactData[currentIndex].phone : " "
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.lowColor
                opacity: animationOpacity
            }
        }

        Column {
            Text {
                text: "E-MAIL"
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.highlightColor
            }

            Text {
                text: currentIndex !== -1 ? contactData[currentIndex].email : " "
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.lowColor
                opacity: animationOpacity
            }
        }
    }
}
