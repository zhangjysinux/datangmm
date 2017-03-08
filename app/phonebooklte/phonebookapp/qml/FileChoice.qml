import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.1
CPage {
    Item{
        width: parent.width
        height: parent.height
        FileDialog {
            id: fileDialog
            width: parent.width
            height: parent.height
            title: "Please choose a file"
            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                photo.iconSource = fileDialog.fileUrls
                Qt.quit()
            }
            onRejected: {
                console.log("Canceled")
                Qt.quit()
            }
        }
    }
}
