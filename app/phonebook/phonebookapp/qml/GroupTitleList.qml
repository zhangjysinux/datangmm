import QtQuick 2.3
import com.syberos.basewidgets 2.0

Item {
    function update(groupId)
    {
        titleList.currentIndex = getData.getTabIndex(groupId)
        titleList.positionViewAtIndex(getData.getTabIndex(groupId),ListView.Center)
    }

    ListView {
        id: titleList
        orientation: ListView.Horizontal
        height: 96
        width: 444
        model: titleModel
        clip: true
        delegate: titleDelegate
//        Component.onCompleted: {
//            positionViewAtIndex(getData.getTabIndex(),ListView.Beginning)
//        }

    }
    Connections {
        target: getData
        onSignalUpdateMessageCount: { // groupId, const int &messageCount
            for (var i = 0; i < titleModel.count; i++)
            {
                if (titleModel.get(i).mygroupId == groupId)
                {
                    titleModel.set(i,{"syncCount":messageCount})
                    titleList.update()
                    break
                }
            }
        }
    }


    ListModel {
        id: titleModel
        Component.onCompleted: {
            var count = getData.getCachePageCount()
            for (var i = 0; i < count; i++)
            {
                titleModel.append({"groupname":getData.getCachePageValue(i,"groupname"), "mygroupId":getData.getCachePageValue(i,"groupId"),
                                      "groupnum":getData.getCachePageValue(i,"groupnum"), "netinfo":getData.getCachePageValue(i,"netinfo"),
                                  "syncCount":getData.getSyncCount(getData.getCachePageValue(i,"groupId"))})
            }
        }
    }
    Component {
        id: titleDelegate
        Item {
            id: rect
            height: 96
            width: 148
            //color: index == getData.getTabIndex() ? "black" : "#008000"
            //border.color: "black"
            Image {
                id: back
                height: 71
                width: 148
                anchors.verticalCenter: parent.verticalCenter
                source: index == getData.getTabCurrentIndex() ? "qrc:/group/images/group-tab-click.png" : "qrc:/group/images/group-tab.png"
            }
            Text {
                id: groupText
                width: 148
                text: "  " + groupname
                anchors.centerIn: parent
                font.pixelSize: 30
                elide: Text.ElideRight
                color: "#ffffff"
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (getData.getTabCurrentIndex() != index){
                        getData.setTabIndex(titleModel.get(index).mygroupId)
                        mainroot.pushCachePage(titleModel.get(index).mygroupId, titleModel.get(index).groupname,
                                               titleModel.get(index).groupnum, titleModel.get(index).netinfo)
                    }
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

