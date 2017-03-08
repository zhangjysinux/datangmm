import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    function tst()
    {
        msgModel.append({"date":"dateStr", "user": "user", "msg": "text"})
    }

    function insertMainList(dateStr, user, text)
    {
        msgModel.append({"date":dateStr, "user": user, "msg": text})
    }

    visible: true
    width: 720
    height: 1280

    Column {
        anchors.top: parent.top
        spacing: 8
        Text {
            id: senderTitle
            text: "Content List"
        }

        Rectangle {
            width: 720
            height: 800
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "lightsteelblue"
                }
                GradientStop {
                    position: 1.0
                    color: "slategray"
                }
            }

            ListModel {
                id: msgModel
            }

            ListView {
                id: sender
                anchors.fill: parent
                model: msgModel
                clip: true
                delegate: Text {
                    text: date + "           " + user + ":" + msg
                }
            }

        }

        Rectangle {
            color: "#e8eef5"
            width: 720
            height: 300
            MessageEdit {
                id: edit
                anchors.fill: parent
            }
        }
    }

    Component.onCompleted: {
        tst()
    }
}

