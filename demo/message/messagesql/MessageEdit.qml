import QtQuick 2.0

Item {
    width: 720
    height: 100
    Row {
        Flickable {
             id: flick
             width: 520;
             height: 100;
             contentWidth: edit.paintedWidth
             contentHeight: edit.paintedHeight
             clip: true

             function ensureVisible(r)
             {
                 if (contentX >= r.x)
                     contentX = r.x;
                 else if (contentX+width <= r.x+r.width)
                     contentX = r.x+r.width-width;
                 if (contentY >= r.y)
                     contentY = r.y;
                 else if (contentY+height <= r.y+r.height)
                     contentY = r.y+r.height-height;
             }

             TextEdit {
                 id: edit
                 width: flick.width
                 height: flick.height
                 focus: true
                 wrapMode: TextEdit.Wrap
                 onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
             }
         }

        Rectangle {
            id: sendButton
            width: 200
            height: 100
            border.color: "lightsteelblue"
            border.width: 4
            radius: 8
            Text {
                id: buttonText
                anchors.centerIn: parent
                text: "Send"
                font.family: "Ubuntu"
                color: "black"
                font.pixelSize: 28
            }
            MouseArea {
                id: sendMouse
                anchors.fill: parent
                onPressed: {
                    parent.color = "blue"
                }
                onReleased: {
                    parent.color = "white"
                    var text = edit.text
                    edit.text = ""
                    msgmanager.sendTextMessage("1001", "100", text)
                }
            }
        }
    }
}

