import QtQuick 2.0
import "res/contacts/addressbook.js" as AddressBook

AppBase {
    id: contactsApp

    property var contactData
    property int currentIndex: -1
    property bool calling
    property real animationOpacity: 1.0

    function sortFunc(a, b)
    {
        if (a.name < b.name)
            return -1;
        else if (a.name > b.name)
            return 1;
        else
            return 0;
    }

    Component.onCompleted: {
        contactData = JSON.parse(AddressBook.addressBook);
        contactData = contactData.sort(sortFunc);
    }

    carouselView:
        Item {
            anchors.fill: parent

            Image {
                id: appIcon

                source: "res/imgs/button_contacts.png"
                anchors.centerIn: parent
            }

            Text {
                id: contactName

                text: currentIndex !== -1 ? contactData[currentIndex].name: "No contact selected"
                font.family: styles.normalFont
                font.pixelSize: styles.normalSize
                color: styles.lowColor
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 30
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: contactName.bottom
                anchors.topMargin: 20
                spacing: 40

                Image {
                    id: iconCall

                    source: "res/imgs/icon_call.png"

                    MouseArea {
                        anchors.fill: parent
                        onPressed: setHighlight(iconCall)
                        onReleased: {
                            removeHighlight(iconCall);
                            calling = !calling;
                        }
                    }
                }

                Image {
                    source: "res/imgs/icon_search.png"
                }
            }

            Text {
                id: callingText

                text: "Calling..."
                font.family: styles.normalFont
                font.pixelSize: styles.titleSize
                color: styles.orange
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: appIcon.bottom
                anchors.topMargin: 70
                opacity: calling ? 1.0 : 0.0

                Behavior on opacity {
                    PropertyAnimation {
                        duration: 800
                    }
                }
            }
        }

    detailsView:
        ContactsDetailsView {
            anchors.fill: parent
    }

    Behavior on animationOpacity {
        PropertyAnimation {
            duration: 800
        }
    }
}
