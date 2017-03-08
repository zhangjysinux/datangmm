import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage{
    id: pictureView

    property var myindex: ""
    property bool isdouble: false
    property var myparent: null
    ListModel {
        id: pictureModel
        Component.onCompleted: {
            getData.getImageMessageList()
        }
    }
    Component.onCompleted: {
        console.log(myindex)
        for (var i = 0; i< pictureModel.count;i++)
        {
            if (myindex == pictureModel.get(i).sources){
                listPage.positionViewAtIndex(i,listPage.Center)
                break;
            }
        }
        console.log(listPage.height)
    }
    Connections {
        target: m_messageModel
        onSignalFindImage: {
            pictureModel.append({"sources":sources})
        }
    }

    ListView{
        id: listPage
        width: 720
        height: 1280
        model: pictureModel
        delegate: Flickable {
            id: flick
            width: 720
            height: 1280
            contentHeight: name.height
            contentWidth: name.width
            Image {
                anchors.fill: parent
                width: 720
                height: 1280
                id: name
                source: sources
                scale: 1
                PinchArea {
                    id: pinch
                    anchors.fill: parent
                    pinch.maximumScale: 3;
                    pinch.minimumScale: 1;
                    pinch.target: parent;
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            pageStack.pop(myparent)
                            //console.log("click",name.souce)
                        }
                    }
                }
                onScaleChanged: {
                    name.height = 1280 * scale
                    name.width = 720 * scale
                }
            }
        }
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        cacheBuffer: 2
        boundsBehavior: Flickable.StopAtBounds
    }

}
