import QtQuick 2.0
import com.syberos.basewidgets 2.0

CToolBar {
    id: menu
    property alias history: history
    property alias home: home
    property alias group: group
    property bool bIsGroup: false
    property bool bIsHistory: false
    property bool direction: false
    anchors.bottom: parent.bottom
    height: 120
    width: parent.width
    spacing: direction ? 188 : 50
    backgroundEnabled: true
    backgroundComponent: Component {
//        Image {
//            anchors.fill: parent
//            source: "qrc:/main/images/bottom_line.png"
//        }
        Rectangle {
            anchors.fill: parent
            color: "#546e8f"
            opacity: 0.3
        }
    }
    CButton {
        id: history
        height: 106
        width: 176
        backgroundEnabled: false
        //iconSource: pressed ? "qrc:/main/images/history_click.png" : "qrc:/main/images/history.png"
        onClicked: {
            if (!bIsHistory) {
                getData.getCallHistoryList()
                mainroot.pageStack.push("qrc:/qml/CallRecord.qml")
            }
        }
    }
    CButton {
        id: home
        height: 106
        width: 176
        backgroundEnabled: false
        //iconSource: pressed ? "qrc:/main/images/person_click.png" : "qrc:/main/images/home.png"
        onClicked: {
            mainroot.clearSearchText()
            getData.getSearchContactors(0,3,"")
            mainroot.pageStack.pop(mainroot)
        }
    }
    CButton {
        id: group
        height: 106
        width: 176
        backgroundEnabled: false
        //iconSource: pressed ? "qrc:/main/images/group_click.png" : "qrc:/main/images/group.png"
        onClicked: {
            if(!bIsGroup) {
                mainroot.pageStack.push("qrc:/qml/GroupMain.qml")
            }
        }
    }
}
