import QtQuick 2.3
import com.syberos.basewidgets 2.0

Item {
    ListView {
        id: titleList
        orientation: ListView.Horizontal
        height: 96
        width: 250
        model: titleModel
        delegate: titleDelegate
        Component.onCompleted: {
            positionViewAtIndex(getData.getTabIndex(),ListView.Center)
        }
    }
    ListModel {
        id: titleModel
        Component.onCompleted: {
            var count = getData.getCachePageCount()
            for (var i = 0; i < count; i++)
            {
                titleModel.append({"groupname":getData.getCachePageValue(i,"groupname"), "groupId":getData.getCachePageValue(i,"groupId"),
                                      "groupnum":getData.getCachePageValue(i,"groupnum"), "netinfo":getData.getCachePageValue(i,"netinfo"),
                                  "syncCount":getData.getSyncCount(getData.getCachePageValue(i,"groupId"))})
            }
        }
    }
    Component {
        id: titleDelegate
        Rectangle {
            id: rect
            height: 96
            width: 120
            color: index == getData.getTabIndex() ? "black" : "#008000"
            border.color: "black"
            Text {
                id: groupText
                width: 120
                text: groupname
                anchors.centerIn: parent
                font.pixelSize: 30
                elide: Text.ElideRight
                color: "#ffffff"
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    getData.setTabIndex(index)
                    mainroot.pushCachePage(titleModel.get(index).groupId, titleModel.get(index).groupname,
                                           titleModel.get(index).groupnum, titleModel.get(index).netinfo)

                }
            }
            Rectangle {
                id: redRect
                width: 30
                height: 30
                radius: 15
                color: "red"
                visible: syncCount == 0 ? false : true
                anchors.top: parent.top
                anchors.right: parent.right
                Text {
                    id: groupSyncCount
                    text: syncCount > 99 ? "..." : syncCount
                    anchors.centerIn: parent
                    color: "#ffffff"
                    font.pixelSize: 24
                }
            }
        }
    }
}

