import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPageStackWindow {

    property int groupId: -1

    initialPage:CPage{
        width:parent.width
        height:parent.height

        CButton{
            id: joinButton
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.leftMargin: 20
            width: 200
            height: 100
            text: "join 55"
            onClicked: {
                groupId = adp.joinGroup("224.0.0.55", 50005)
                console.debug("joinGroup: " + groupId)
                adp.setPttState(groupId, 1)
                joinButton.enabled = false
                leaveButton.enabled = true
            }
        }


        CButton{
            id: leaveButton
            anchors.left: joinButton.right
            anchors.top: parent.top
            anchors.topMargin: 100
            width: 200
            height: 100
            text: "leave"
            enabled: false
            onClicked: {
                adp.leaveGroup(groupId)
                console.debug("leaveGroup: " + groupId)
                groupId = -1;
                joinButton.enabled = true
                leaveButton.enabled = false
            }
        }



        Column
        {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            spacing: 50
            CButton{
                id: speak
                text: "speak"
                width: 500
                height: 500
                onPressed: {
                    //speak
                    if(groupId != -1)
                        adp.setPttState(groupId, 2)
                }
                onReleased: {
                    //stop speak
                    if(groupId != -1)
                        adp.setPttState(groupId, 1)
                }
            }
        }




        Connections{
            target: adp
            onSignalPttStateChanged:{
                console.debug("onSignalPttStateChanged, state: " + adp.getPttState(groupId))
            }
        }
    }
}

